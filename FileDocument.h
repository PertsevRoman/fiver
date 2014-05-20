#ifndef __FileDocument_h__
#define __FileDocument_h__

#include "Document.h"

class FileDocument: public Document {
    Q_OBJECT

public:
    FileDocument(QObject *parent = 0);
    ~FileDocument();
};

#endif
