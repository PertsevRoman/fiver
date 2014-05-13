#include <string>
#include <vector>
#include <exception>

#ifndef __WebDocument_h__
#define __WebDocument_h__

#include "Document.h"

class WebDocument: public Document
{

public:
    explicit Document(QObject *parent = 0);
    ~Document();
    /**
     * @brief get запрашивает исходник
     */
    void get();
    /**
     * @brief parse парсит исходник
     */
    void parse();
};

#endif
