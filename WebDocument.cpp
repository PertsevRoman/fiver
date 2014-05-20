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
//    qDebug() << "Наладка загрузчика: " << QString::fromStdString(uri);
    emit message("Загрузка ресурса...");

    //Запуск загрузки текста
    QUrl url("http://access.alchemyapi.com/calls/url/URLGetText?url=" + QString::fromStdString(uri)  + "&apikey=84325e4232e9f5825a6815117d86895dd98eb592");
    QNetworkReply* reply = man->get(QNetworkRequest(url));
    //Соединение с отслеживанием процентов
    connect(reply, &QNetworkReply::downloadProgress, this, &WebDocument::onNRBytesLoaded);

    //Запуск загрузки Яндекс-тИЦ
    QUrl tycUrl("http://bar-navig.yandex.ru/u?ver=2&show=32&url=" + QString::fromStdString(uri));
    QNetworkReply* tycReply = tycAccess->get(QNetworkRequest(tycUrl));
}

void WebDocument::parse(QNetworkReply *reply) {
    disconnect(reply, &QNetworkReply::uploadProgress, this, &WebDocument::onNRBytesLoaded);

    //Извлечение текста
    if(reply->error() == QNetworkReply::NoError) {
        //Очистка буфера
        lemmas.clear();

        boost::thread *th = new boost::thread([=](){
//            qDebug() << "Текст загружен, распознаем";
            QDomDocument xmlDoc;
            xmlDoc.setContent(reply->readAll());

//            qDebug() << "Выборка тега <text>";
            QString textData = xmlDoc.firstChildElement("results").firstChildElement("text").text();

//            qDebug() << "Разбор на предложения";
            emit message("Разбор на предложения...");
            if(textData.size()) {
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

                        lemmas.append(lemma);
//                        qDebug() << lemma;
                    } while(sentLength > -1);

                    emit message("Разбор завершен");
                    emit textPrepared();
                    sol_DeleteSentenceBroker(broker);
                } else {
                    emit message("Ошибка загрузки словаря");
//                    qDebug() << "Ошибка загрузки словаря";
                }
            } else {
                emit message("Текст не получен");
//                qDebug() << "Текст не получен";
            }
        });

        th->detach();
    } else {
        emit message("Ошибка при загрузке текста");
//        qDebug() << "Ошибка при загрузке текста";
    }
}

void WebDocument::prepareText() {
    qDebug() << "Подготовка текста";
    get();
}

void WebDocument::refreshTone() {
    boost::thread *th = new boost::thread([this](){
        QVector<uint> uintCode = QString::fromStdString(uri).toUcs4();
        int stSum = 0;

        foreach (uint val, uintCode) {
            stSum += val;
        }

        int sum = 0;
        while(stSum > 10) {
            while(stSum != 0) {
                sum += stSum % 10;
                stSum /= 10;
            }

            stSum = sum;
            sum = 0;
        }

        if(!stSum) {
            emit sendTone("Нет возможности определить тональность");
        } else {
            emit sendTone(voteDefends[stSum - 5]);
        }
    });

    th->detach();
}

void WebDocument::parseTYC(QNetworkReply *reply) {
    QString tyc = 0;

    QDomDocument xmlDoc;
    xmlDoc.setContent(reply->readAll());

    tyc = xmlDoc.firstChildElement("urlinfo").firstChildElement("tcy").attribute("rang");

    emit trySetInfo(2, tyc);
}

void WebDocument::onNRBytesLoaded(qint64 loaded, qint64 total) {
    if(total != 0) {
        emit percentCompleted(loaded * 100 / total);
    }
}

void WebDocument::init() {
    man = new QNetworkAccessManager(this);
    tycAccess = new QNetworkAccessManager(this);

    tycRanc = 0;
    gpRanc = 0;
}

void WebDocument::sets() {
}

void WebDocument::conn() {
    connect(man, &QNetworkAccessManager::finished, this, &WebDocument::parse);
    connect(tycAccess, &QNetworkAccessManager::finished, this, &WebDocument::parseTYC);
}
