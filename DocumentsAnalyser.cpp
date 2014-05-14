#include "DocumentsAnalyser.h"

DocumentsAnalyser::DocumentsAnalyser(QObject *parent) : QObject(parent) {
    init();
    sets();
    conn();
}

DocumentsAnalyser::~DocumentsAnalyser() {
}

bool DocumentsAnalyser::add(std::string uri, int mark) {
    boost::regex ex("^(http(?:s)?\\:\\/\\/[a-zA-Z0-9\\-]+(?:\\.[a-zA-Z0-9\\-]+)*\\.[a-zA-Z]{2,6}(?:\\/?|(?:\\/[\\w\\-]+)*)(?:\\/?|\\/\w+\\.[a-zA-Z]{2,4}(?:\\?[\\w]+\\=[\\w\\-]+)?)?(?:\\&[\\w]+\\=[\\w\\-]+)*)$");
    boost::smatch exMatch;

    if(!boost::regex_match(uri, exMatch, ex)) {  return false; }

    QList<QStandardItem*> rowVals;

    QStandardItem *uriItm = new QStandardItem(QString::fromStdString(uri));
    QStandardItem *markItm = new QStandardItem(QString("%1").arg(mark));

    rowVals.append(uriItm);
    rowVals.append(markItm);
    rowVals.append(new QStandardItem());
    rowVals.append(new QStandardItem());

    model->appendRow(rowVals);

    return true;
}

void DocumentsAnalyser::analyse() {
	throw "Not yet implemented";
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
    model = new QStandardItemModel(0, 4, this);
}

void DocumentsAnalyser::sets() {
    QStringList headers;
    headers << "URL" << "Экспертная оценка" << "PR/ТИЦ" << "Оценка";

    model->setHorizontalHeaderLabels(headers);
}

void DocumentsAnalyser::conn() {
}
