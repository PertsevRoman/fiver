#include <string>
#include <vector>
#include <exception>
using namespace std;

#ifndef __DocumentsAnalyser_h__
#define __DocumentsAnalyser_h__

#include "Document.h"
#include "FileDocument.h"
#include "WebDocument.h"

class DocumentsAnalyser
{
    std::vector<Document*> unnamed_Document_;

public:
    /**
     * @brief add добавляет ресурс для анализа
     * @param uri URI ресурса
     */
    void add(std::string uri);
    void analyse();
};

#endif
