#include <string>
using namespace std;

#ifndef __DocumentsAnalyser_h__
#define __DocumentsAnalyser_h__

#include <QVector>

#include "Document.h"
#include "FileDocument.h"
#include "WebDocument.h"

class DocumentsAnalyser
{
    QVector<Document*> docCollection;

public:
    /**
     * @brief add добавляет ресурс для анализа
     * @param uri URI ресурса
     */
    void add(std::string uri);
    void analyse();
};

#endif
