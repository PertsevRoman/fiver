#include "FileDocument.h"

FileDocument::FileDocument(QObject *parent) : Document (parent) {
}

FileDocument::~FileDocument() {
    docType = "FILE";
}
