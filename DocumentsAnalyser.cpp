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
    Document* doc = qobject_cast<Document*>(sender());
    boost::thread *th = new boost::thread([doc]() {
        doc->refreshTone();
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
