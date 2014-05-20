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
#include <QMap>
#include <QSet>

#include "Document.h"
#include "FileDocument.h"
#include "WebDocument.h"
#include "DataModel.h"

#include "optionsdialog.h"

#include <boost/regex.hpp>
#include <boost/thread/mutex.hpp>

#include <lem/solarix/solarix_grammar_engine.h>

class DataModel;

class DocumentsAnalyser : public QObject
{
    Q_OBJECT

    //Коллекция документов
    QSet<Document*> docCollection;
    QString termine;

    //Модель данных
    QStandardItemModel *model;

    //Модель доступа к БД
    DataModel *dbModel;

    //Движок
    QString enginiePath;
    HGREN hEngine;

    //Соответствия строк таблицы и документа
    QMap<Document*, int> documentsRow;
    QMap<int, Document*> rowDocuments;

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
    /**
     * @brief getTermine Возвращает значение анализируемого термина
     * @return
     */
    QString getTermine() const;

    DataModel *getDBModel() const;
    void setDBModel(DataModel *value);

signals:
    /**
     * @brief error Ошибка
     */
    void error(QString);

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
    /**
     * @brief setTermine Установка анализируемого термина
     * @param value Термин
     */
    void setTermine(const QString value);

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
    /**
     * @brief docPercentCompleted Документ провел некоторую часть работ
     * @param percent Процент
     */
    void docPercentCompleted(int percent);
    /**
     * @brief docMessage Документ послал сообщение
     * @param msg Сообщение
     */
    void docMessage(QString msg);
    /**
     * @brief recieveColumn Документ убедительно просит установить именно эту колонку
     * @param column Колонка
     * @param value Значение
     */
    void recieveColumn(int column, QString value);
    /**
     * @brief docTone Установка тональности
     * @param tone Тональность
     */
    void docTone(QString tone);
};

#endif
