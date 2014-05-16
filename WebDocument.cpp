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
    qDebug() << "Наладка загрузчика: " << QString::fromStdString(uri);
    QUrl url(QString::fromStdString(uri));
    man->get(QNetworkRequest(url));
}

void WebDocument::parse(QNetworkReply *reply) {
    if(reply->error() == QNetworkReply::NoError) {
        boost::thread *th = new boost::thread([=](){
            qDebug() << "Текст загружен, распознаем";
            QDomDocument xmlDoc;
            xmlDoc.setContent(reply->readAll());

            qDebug() << "Выборка тега <text>";
            QString textData = xmlDoc.firstChildElement("results").firstChildElement("text").text();
            textData.replace("\n", "");
            textData.replace("\n\r", "");
            textData.replace("\r", "");
            textData.replace("\t", " ");
            textData.replace("$", "");
            textData.replace("[\s]{2,}", " ");

            qDebug() << "Текст: " << textData.left(100);
            qDebug() << "Разбор на предложения";
            if(sol_IsDictionaryLoaded(hEngine)) {
                //Извлечение предложений из текста
                HGREN_SBROKER broker = sol_CreateSentenceBrokerMemW(hEngine, textData.toStdWString().data(), sol_FindLanguage(hEngine, L"Russian"));

                //Перебор предложений
                int sentLength = 0;
                do {
                    sentLength = sol_FetchSentence(broker);

                    wchar_t *buf = new wchar_t[sentLength + 1];
                    sol_GetFetchedSentenceW(broker, buf);

                    QString lemma = QString::fromWCharArray(buf, sentLength + 1);

                    delete [] buf;

                    cache.append(lemma);
                    qDebug() << lemma;
                } while(sentLength > -1);
            } else {
                //TODO Ошибка загрузки словаря
            }
        });

        th->detach();
    } else {
        qDebug() << "Ошибка при загрузке текста";
    }
}

void WebDocument::prepareText() {
    qDebug() << "Подготовка текста";
    get();
}

void WebDocument::init() {
    man = new QNetworkAccessManager(this);
}

void WebDocument::sets() {
}

void WebDocument::conn() {
    connect(man, &QNetworkAccessManager::finished, this, &WebDocument::parse);
}
