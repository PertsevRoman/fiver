#include <string>
#include <vector>
#include <exception>
using namespace std;

#include "Document.h"
#include "DocumentsAnalyser.h"

Document::Document(QObject *parent) : QObject(parent){
}

Document::~Document() {
}

std::string Document::getUri() {
    return uri;
}

void Document::setUri(std::string uri) {
    if(this->uri != uri) {
        this->uri = uri;
    }
}

std::string Document::getText() {
	throw "Not yet implemented";
}

std::string Document::getDocType() {
    return docType;
}

