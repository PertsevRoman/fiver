#include <string>
using namespace std;

#ifndef __DocumentsAnalyser_h__
#define __DocumentsAnalyser_h__

#include <QVector>
#include <QObject>
#include <QStandardItemModel>
#include <QStringList>
#include <QList>
#include <QStandardItem>

#include "Document.h"
#include "FileDocument.h"
#include "WebDocument.h"

#include <boost/regex.hpp>

class DocumentsAnalyser : public QObject
{
    Q_OBJECT

    //Коллекция документов
    QVector<Document*> docCollection;

    //Модель данных
    QStandardItemModel *model;

public:
    explicit DocumentsAnalyser(QObject *parent = 0);
    ~DocumentsAnalyser();

    /**
     * @brief add добавляет ресурс для анализа
     * @param uri URI ресурса
     */
    bool add(std::string uri, int mark);
    /**
     * @brief analyse Анализ данных
     */
    void analyse();

    //GET и SET метода для модели
    QStandardItemModel *getResourseListModel() const;
    void setModel(QStandardItemModel *value);

    /**
     * @brief removeCurrentRow удаляет строку (если полностью выделена)
     */
    void removeRow(int row);

private:
    void init();
    void sets();
    void conn();
};

#endif
