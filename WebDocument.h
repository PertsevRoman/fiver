#include <string>
#include <vector>
#include <exception>

#ifndef __WebDocument_h__
#define __WebDocument_h__

#include <QtNetwork>

#include <lem/solarix/solarix_grammar_engine.h>

#include <boost/thread.hpp>

#include "Document.h"

class WebDocument: public Document
{
    Q_OBJECT

    //Кэш страницы
    QString cache;

    //Менеджер соединения
    QNetworkAccessManager* man;

    //Движок словаря
    HGREN hEngine;

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
    void prepareText();

    HGREN getGrammarEngine() const;
    void setGrammarEngine(const HGREN &engine);

private:
    void init();
    void sets();
    void conn();
};

#endif
