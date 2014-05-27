#include "DocumentsAnalyser.h"

DocumentsAnalyser::DocumentsAnalyser(QObject *parent) : QObject(parent) {
    init();
    sets();
    conn();
}

DocumentsAnalyser::~DocumentsAnalyser() {
    if(hEngine) {
        sol_UnloadDictionary(hEngine);
    }
}

DataModel *DocumentsAnalyser::getDBModel() const {
    return dbModel;
}

void DocumentsAnalyser::setDBModel(DataModel *value) {
    dbModel = value;
}

QString DocumentsAnalyser::getTermine() const {
    return termine;
}

void DocumentsAnalyser::setTermine(const QString value) {
    termine = value;
}

bool DocumentsAnalyser::add(std::string uri, int mark) {
    //boost::regex ex("^(http(?:s)?\\:\\/\\/[a-zA-Z0-9\\-]+(?:\\.[a-zA-Z0-9\\-]+)*\\.[a-zA-Z]{2,6}(?:\\/?|(?:\\/[\\w\\-]+)*)(?:\\/?|\\/\w+\\.[a-zA-Z]{2,4}(?:\\?[\\w]+\\=[\\w\\-]+)?)?(?:\\&[\\w]+\\=[\\w\\-]+)*)$");
    //boost::smatch exMatch;

    //if(!boost::regex_match(uri, exMatch, ex)) {  return false; }

    //Новая строка
    QList<QStandardItem*> rowVals;

    //Инизиализация элементов
    QStandardItem *uriItm = new QStandardItem(QString::fromStdString(uri));
    QStandardItem *markItm = new QStandardItem(QString("%1").arg(mark));

    //Формирование строки данных
    rowVals.append(uriItm);
    rowVals.append(markItm);
    rowVals.append(new QStandardItem());
    rowVals.append(new QStandardItem(0));
    rowVals.append(new QStandardItem());
    rowVals.append(new QStandardItem());

    //Добавление документа
    Document* document = new WebDocument(this);
    docCollection << document;

    //Соединение
    connect(document, &Document::percentCompleted, this, &DocumentsAnalyser::docPercentCompleted);
    connect(document, &Document::message, this, &DocumentsAnalyser::docMessage);
    connect(document, &Document::trySetInfo, this, &DocumentsAnalyser::recieveColumn);
    connect(document, &Document::sendTone, this, &DocumentsAnalyser::docTone);
    connect(document, &Document::textPrepared, this, &DocumentsAnalyser::docTextPrepared);

    //Добавление строки
    model->appendRow(rowVals);

    //Добавление связки документа и строки
    documentsRow.insert(document, model->rowCount() - 1);
    rowDocuments.insert(model->rowCount() - 1, document);

    return true;
}

void DocumentsAnalyser::analyse() {
    if(!termine.size()) {
        emit error("Искомый термин не определен");
        return;
    }

    if(dbModel) {
        if(!dbModel->dbLinkIsCorrect()) {
            emit error("Нет соединения с БД");
            return;
        }
    } else {
        emit error("Не определена БД");
        return;
    }

    if(hEngine) {
        qDebug() << "Запускаем анализ";
        if(sol_IsDictionaryLoaded(hEngine)) {
            foreach(Document* doc, docCollection) {
                //Извлечение url
                QModelIndex index = model->index(documentsRow[doc], 0);
                std::string uri = index.data().toString().toStdString();

                qDebug() << "URL (" << documentsRow[doc] << "): " << QString::fromStdString(uri);
                //Установки и запуск
                doc->setUri(uri);
                doc->setGrammarEngine(hEngine);
                doc->prepareText();
            }
        }
    }
}

void DocumentsAnalyser::setGrammaEnginiePath(QString path) {
    enginiePath = path;

    reloadGrammaEngine();
}

void DocumentsAnalyser::optionsAccepted() {
    mx.lock();

    OptionsDialog* opts = qobject_cast<OptionsDialog*>(sender());
    setGrammaEnginiePath(opts->getDictPath());

    mx.unlock();
}

QStandardItemModel *DocumentsAnalyser::getResourseListModel() const {
    return model;
}

void DocumentsAnalyser::setModel(QStandardItemModel *value) {
    model = value;
}

void DocumentsAnalyser::removeRow(int row) {
    //Документ
    Document* doc = rowDocuments[row];

    //Очистка перекрестных массивов
    rowDocuments.remove(row);
    documentsRow.remove(doc);

    //Удаление документа из обработки
    docCollection.remove(doc);
    model->removeRow(row);
}

void DocumentsAnalyser::init() {
    model = new QStandardItemModel(0, 5, this);
    hEngine = 0;
}

void DocumentsAnalyser::sets() {
    QStringList headers;
    headers << "URL" << "Экспертная оценка" << "ТИЦ" << "Прогресс" << "Статус" << "Оценка";

    model->setHorizontalHeaderLabels(headers);
}

void DocumentsAnalyser::conn() {
}

void DocumentsAnalyser::reloadGrammaEngine() {
    if(QFileInfo(enginiePath).exists() && QFileInfo(enginiePath).isReadable()) {
        if(hEngine) {
            if(sol_IsDictionaryLoaded(hEngine)) {
                sol_UnloadDictionary(hEngine);
            }
        }

        qDebug() << "Создание движка";
        hEngine = sol_CreateGrammarEngineA(enginiePath.toStdString().data());
        sol_LoadDictionaryA(hEngine, enginiePath.toStdString().data());
    }
}

void DocumentsAnalyser::docTextPrepared() {
    //Документ
    Document* doc = qobject_cast<Document*>(sender());

    //Анализирующий поток
    boost::thread *th = new boost::thread([this, doc]() {
        //Локальная переменная для грамматического движка
        HGREN gren = this->hEngine;

        //Извлечение леммы
        QVector<QString> docLemmas = doc->getLemmas();

        //Флаги морфологического анализа
        const int morFlags = SOL_GREN_ENABLE_RECONSTRUCTION | SOL_GREN_MODEL;
        const int constraints = (20 << 22) | 60000;

        mx.lock();

        qDebug() << "Синтаксический анализ предложений";
#pragma omp parallel for private(gren)
        for(int i = 0; i < docLemmas.size(); i++) {
            //Проводим анализ предложения
            qDebug() << "Анализ (" << i << ")";
            HGREN_RESPACK syn = sol_SyntaxAnalysis(gren, docLemmas[i].toStdWString().data(), morFlags, 0, constraints, -1);

            qDebug() << "Поиск самой короткой альтернативы (" << i << ")";
            //Количество графов
            const int graphCount = sol_CountGrafs(syn);
            //Поиск самой короткой альтернативы
            int minIndex = -1, minv = 1000000;
            for(int j = 0; j < graphCount; j++) {
                const int nroots = sol_CountRoots(syn, j);

                if(nroots < minv) {
                    minv = nroots;
                    minIndex = i;
                }
            }

            qDebug() << "Буфер (" << i << ")";
            //Создание буфера
            const int maxLexLen = sol_MaxLexemLen(gren);
            wchar_t *buf = new wchar_t[maxLexLen];
            memset(buf, 0, maxLexLen);
            //Синтаксический токен
            QString synToken;

            qDebug() << "Разбор самой короткой альтернативы (" << i << ")";
            //Разбор самой короткой альтернативы
            const int nroot = sol_CountRoots(syn, minIndex);
            for(int j = 1; j < nroot - 1; j++) {
                HGREN_TREENODE node = sol_GetRoot(syn, minIndex, j);

                //Получение количества листьев
                const int leafsCount = sol_CountLeafs(node);
                //Обход листьев
                for(int k = 0; k < leafsCount; k++) {
                    HGREN_TREENODE leaf = sol_GetLeaf(node, k);

                    //Текст узла
                    sol_GetNodeContents(leaf, buf);

                    //Дополнение синтаксической единицы
                    synToken += QString::fromWCharArray(buf);
                }

                qDebug() << synToken;
                synToken.clear();
                memset(buf, 0, maxLexLen);
            }

            qDebug() << "Очистка (" << i << ")";
            //Очистка буфера
            delete [] buf;

            //Очистка результатов разбора
            sol_DeleteResPack(syn);
        }

        mx.unlock();
//        doc->refreshTone();
    });

    th->detach();
}

void DocumentsAnalyser::docPercentCompleted(int percent) {
    int row = documentsRow[qobject_cast<Document*>(sender())];
    QModelIndex index = model->index(row, 3);
    model->setData(index, percent);
}

void DocumentsAnalyser::docMessage(QString msg) {
    int row = documentsRow[qobject_cast<Document*>(sender())];
    QModelIndex index = model->index(row, 4);
    model->setData(index, msg);
}

void DocumentsAnalyser::recieveColumn(int column, QString value) {
    int row = documentsRow[qobject_cast<Document*>(sender())];
    QModelIndex index = model->index(row, column);
    model->setData(index, value);
}

void DocumentsAnalyser::docTone(QString tone) {
    int row = documentsRow[qobject_cast<Document*>(sender())];
    QModelIndex index = model->index(row, 5);
    model->setData(index, tone);
}
