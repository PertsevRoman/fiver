#include <string>
#include <vector>
#include <exception>

#ifndef __Document_h__
#define __Document_h__

#include <QObject>

#include <boost/regex.hpp>

#include "documenterrors.h"

class Document : public QObject {
    Q_OBJECT

protected:
    std::string uri;
    std::string text;
    std::string docType;

    DocumentState::ProcessingState state;

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
     * @brief getText возвращает содержимое ресурса
     * @return содержимое ресурса
     */
    std::string getText();
    /**
     * @brief getDocType возвращает тип документа
     * @return тип документа
     */
    std::string getDocType();
    /**
     * @brief prepareText Выполняет подготовку текста для синтаксического разбора
     */
    virtual void prepareText() = 0;

    //GET и SET методы для установки и получения значения состояния документа
    DocumentState::ProcessingState getState() const;
    void setState(const DocumentState::ProcessingState &value);

protected:
    virtual void init();
    virtual void sets() = 0;
    virtual void conn() = 0;
};

#endif
