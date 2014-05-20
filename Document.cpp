#include "Document.h"

Document::Document(QObject *parent) : QObject(parent), CategoryMaster() {
    init();
}

Document::~Document() {
}

QVector<QString> Document::getLemmas() const {
    return lemmas;
}

void Document::setLemmas(const QVector<QString> &value) {
    lemmas = value;
}

DocumentState::ProcessingState Document::getState() const {
    return state;
}

void Document::setState(const DocumentState::ProcessingState &value) {
    state = value;
}

void Document::init() {
    state = DocumentState::NoError;
}

std::string Document::getUri() {
    return uri;
}

void Document::setUri(std::string uri) {
    if(this->uri != uri) {
        this->uri = uri;
    }
}

void Document::setGrammarEngine(const HFAIND &engine) {
    this->hEngine = engine;
}

HGREN Document::getGrammarEngine() const {
    return hEngine;
}

std::string Document::getDocType() {
    return docType;
}

