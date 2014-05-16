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

#include "optionsdialog.h"

#include <boost/regex.hpp>
#include <boost/thread/mutex.hpp>

#include <lem/solarix/solarix_grammar_engine.h>

class DocumentsAnalyser : public QObject
{
    Q_OBJECT

    //Коллекция документов
    QVector<Document*> docCollection;

    //Модель данных
    QStandardItemModel *model;

    //Движок
    QString enginiePath;
    HGREN hEngine;

    //Мютекс
    boost::mutex mx;

public:
    explicit DocumentsAnalyser(QObject *parent = 0);
    ~DocumentsAnalyser();

    /**
     * @brief add добавляет ресурс для анализа
     * @param uri URI ресурса
     */
    bool add(std::string uri, int mark);

    //GET и SET метода для модели
    QStandardItemModel *getResourseListModel() const;
    void setModel(QStandardItemModel *value);

    /**
     * @brief removeCurrentRow удаляет строку (если полностью выделена)
     */
    void removeRow(int row);

public slots:
    /**
     * @brief analyse Анализ данных
     */
    void analyse();
    /**
     * @brief setGrammaEnginiePath Устанавливает путь для файла словаря
     * @param path
     */
    void setGrammaEnginiePath(QString path);
    /**
     * @brief optionsAccepted Добавление опций
     */
    void optionsAccepted();

private:
    void init();
    void sets();
    void conn();

    void reloadGrammaEngine();

private slots:
    /**
     * @brief docTextPrepared Документ закончил подготовку текста
     */
    void docTextPrepared();
};

#endif
