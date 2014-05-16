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
        boost::thread *th = new boost::thread([=](){
            QString textData(reply->readAll());

            //TODO Выборка тега <text>

            if(sol_IsDictionaryLoaded(hEngine)) {
                //Извлечение предложений из текста
                HGREN_SBROKER broker = sol_CreateSentenceBrokerMemW(hEngine, textData.toStdWString().data(), sol_FindLanguage(hEngine, L"Russian"));

                //Перебор предложений
                int sentLength = 0;
                do {
                    sentLength = sol_FetchSentence(broker);
                } while(sentLength > -1);
            } else {
                //TODO Ошибка загрузки словаря
            }
        });

        th->detach();
    } else {
        //TODO Ошибка загрузки данных
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

void WebDocument::setGrammarEngine(const HFAIND &engine) {
    this->hEngine = engine;
}

HGREN WebDocument::getGrammarEngine() const {
    return hEngine;
}

void WebDocument::conn() {
    connect(man, &QNetworkAccessManager::finished, this, &WebDocument::parse);
}
