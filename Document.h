#include <string>
#include <vector>
#include <exception>

#ifndef __Document_h__
#define __Document_h__

class Document
{
private:
    std::string uri;
    std::string text;
    std::string docType;

public:
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
};

#endif
