#include "DocumentsAnalyser.h"

DocumentsAnalyser::DocumentsAnalyser(QObject *parent) : QObject(parent) {
    init();
    sets();
    conn();
}

DocumentsAnalyser::~DocumentsAnalyser() {
}

bool DocumentsAnalyser::add(std::string uri, int mark) {
    //boost::regex ex("^(http(?:s)?\\:\\/\\/[a-zA-Z0-9\\-]+(?:\\.[a-zA-Z0-9\\-]+)*\\.[a-zA-Z]{2,6}(?:\\/?|(?:\\/[\\w\\-]+)*)(?:\\/?|\\/\w+\\.[a-zA-Z]{2,4}(?:\\?[\\w]+\\=[\\w\\-]+)?)?(?:\\&[\\w]+\\=[\\w\\-]+)*)$");
    //boost::smatch exMatch;

    //if(!boost::regex_match(uri, exMatch, ex)) {  return false; }

    QList<QStandardItem*> rowVals;

    QStandardItem *uriItm = new QStandardItem(QString::fromStdString(uri));
    QStandardItem *markItm = new QStandardItem(QString("%1").arg(mark));

    rowVals.append(uriItm);
    rowVals.append(markItm);
    rowVals.append(new QStandardItem());
    rowVals.append(new QStandardItem());
    rowVals.append(new QStandardItem());

    Document* document = new WebDocument(this);
    document->setUri(QString("http://access.alchemyapi.com/calls/url/URLGetText?url=" + QString::fromStdString(uri) + "&apikey=84325e4232e9f5825a6815117d86895dd98eb592").toStdString());
    docCollection.append(document);

    model->appendRow(rowVals);

    return true;
}

void DocumentsAnalyser::analyse() {
    if(hEngine) {
        qDebug() << "Запускаем анализ";
        if(sol_IsDictionaryLoaded(hEngine)) {
            foreach(Document* doc, docCollection) {
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
    model->removeRow(row);
}

void DocumentsAnalyser::init() {
    model = new QStandardItemModel(0, 5, this);
    hEngine = 0;
}

void DocumentsAnalyser::sets() {
    QStringList headers;
    headers << "URL" << "Экспертная оценка" << "PR/ТИЦ" << "Прогресс" << "Оценка";

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
}
