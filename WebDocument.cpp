#include "WebDocument.h"

WebDocument::WebDocument(QObject *parent) : Document(parent) {
    docType = "WEB";

    init();
    sets();
    conn();
}

WebDocument::~WebDocument() {
}

void WebDocument::get() {
    QUrl url(QString::fromStdString(uri));
    man->get(QNetworkRequest(url));
}

void WebDocument::parse(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
    } else {
    }
}

void WebDocument::prepareText() {
    void get();
}

void WebDocument::init() {
    man = new QNetworkAccessManager(this);
}

void WebDocument::sets() {
}

void WebDocument::conn() {
    connect(man, &QNetworkAccessManager::finished, this, &WebDocument::parse);
}

