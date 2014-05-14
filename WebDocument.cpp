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
	throw "Not yet implemented";
}

void WebDocument::parse(QNetworkReply *reply) {
    if(reply->error() != QNetworkReply::NoError) {
    }
}

void WebDocument::init() {
    man = new QNetworkAccessManager(this);
}

void WebDocument::sets() {
}

void WebDocument::conn() {
    connect(man, &QNetworkAccessManager::finished, this, &WebDocument::parse);
}

