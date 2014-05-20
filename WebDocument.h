#include <string>
#include <vector>
#include <exception>

#ifndef __WebDocument_h__
#define __WebDocument_h__

#include <QtNetwork>
#include <QtXml>

#include <boost/thread.hpp>

#include "Document.h"

class WebDocument: public Document
{
    Q_OBJECT

    //Менеджер соединения
    QNetworkAccessManager* man;
    QNetworkAccessManager* tycAccess;

    int tycRanc;
    int gpRanc;

public:
    explicit WebDocument(QObject *parent = 0);
    ~WebDocument();

    /**
     * @brief get запрашивает исходник
     */
    void get();
    /**
     * @brief parse парсит исходник
     */
    void parse(QNetworkReply *reply);
    /**
     * @brief prepareText Подготовка текста
     */
    void prepareText();
    /**
     * @brief getTone Оценка тональности документа
     */
    void refreshTone();
    /**
     * @brief parseTYC Извлечение тИЦ сайта
     * @param reply Ответ
     */
    void parseTYC(QNetworkReply *reply);
    
private slots:
    void onNRBytesLoaded(qint64 loaded, qint64 total);

private:
    void init();
    void sets();
    void conn();
};

#endif
