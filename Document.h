#include <string>
#include <vector>
#include <exception>

#ifndef __Document_h__
#define __Document_h__

#include <QObject>
#include <QVector>

#include <boost/regex.hpp>

#include <lem/solarix/solarix_grammar_engine.h>

#include "documenterrors.h"
#include "categorymaster.h"

class Document : public QObject, public CategoryMaster {
    Q_OBJECT

protected:
    std::string uri;
    QVector<QString> lemmas;
    std::string docType;

    DocumentState::ProcessingState state;

    //Движок словаря
    HGREN hEngine;

public:
    explicit Document(QObject *parent = 0);
    ~Document();
    /**
     * @brief getUri возвращает URI ресурса
     * @return URI
     */
    std::string getUri();
    /**
     * @brief setUri устанавливает URI ресурса
     * @param uri
     */
    void setUri(std::string uri);
    /**
     * @brief getDocType возвращает тип документа
     * @return тип документа
     */
    std::string getDocType();
    /**
     * @brief prepareText Выполняет подготовку текста для синтаксического разбора
     */
    virtual void prepareText() = 0;
    /**
     * @brief getTone Вычисление тональности документа
     */
    virtual void refreshTone() = 0;

    //GET и SET методы для установки и получения значения состояния документа
    DocumentState::ProcessingState getState() const;
    void setState(const DocumentState::ProcessingState &value);

    HGREN getGrammarEngine() const;
    void setGrammarEngine(const HGREN &engine);

    QVector<QString> getLemmas() const;
    void setLemmas(const QVector<QString> &value);

signals:
    /**
     * @brief percentCompleted Процентов завершено
     */
    void percentCompleted(int);
    /**
     * @brief currentTask Текущее задание
     */
    void message(QString);
    /**
     * @brief docTone Тональность документа
     */
    void sendTone(QString);
    /**
     * @brief trySetInfo Попытка отправить информацию в нужную колонку
     * @param column
     */
    void trySetInfo(int column, QString);
    /**
     * @brief textPrepared Подготовка текста закончена
     */
    void textPrepared();

protected:
    virtual void init();
    virtual void sets() = 0;
    virtual void conn() = 0;
};

#endif
