#include <string>
#include <vector>
#include <exception>

#ifndef __DataModel_h__
#define __DataModel_h__

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QPushButton>

#include "MainWindow.h"
#include "YandexImageConnector.h"
#include "TriangleNumber.h"
#include "TriangleMathCollector.h"

#include "codes.h"
#include "optionsdialog.h"

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/variate_generator.hpp>

class MainWindow;

class DataModel : public QObject
{
    Q_OBJECT

    MainWindow              *mainWindow;
    YandexImageConnector    *imageConnector;

    QSqlDatabase    db;
    QSqlTableModel  *sqlModel;

    boost::mutex mx;

    //Параметры БД
    QString host;
    int port;
    QString username;
    QString pass;
    QString dbName;

    //Количество строк в словаре
    int mapRowCount;

public:
    //Конструктор, деструктор
    explicit DataModel(QObject *parent = 0);
    ~DataModel();

    /**
     * @brief getNextImageData возвращает новые данные для заполнения (теги и ссылку на тематическое изображение)
     * @return пара ссылка-набор тегов
     */
    std::pair<std::string, std::vector<std::string> > getNextImageData();
    /**
     * @brief appendMark устанавливает оценку для набора статей
     * @param articles набор статей
     * @param mark оценка
     */
    void appendMark();
    /**
     * @brief getMark возвращает оценку для статьи
     * @param article имя статьи
     * @return оценка
     */
    TriangleNumber getMark(std::string article);
    /**
     * @brief articleExists проверка существования статьи
     * @param article имя статьи
     * @return true если статья существует, иначе false
     */
    bool articleExists(std::string article);
    /**
     * @brief refreshMarks обновляет оценки в словаре
     */
    void refreshMarks();
    /**
     * @brief getMainWindow Извлечение значения MainWindow
     * @return Указатель на виджет главного окна
     */
    MainWindow *getMainWindow() const;
    /**
     * @brief setMainWindow Установка значения MainWindow
     * @param value Указатель на виджет главного окна
     */
    void setMainWindow(MainWindow *value);
    /**
     * @brief getNextTag Возвращает следующий тег из БД
     * @return тег
     */
    void getNextRandomTag();
    /**
     * @brief dbLinkIsCorrect проверка базы данных
     * @return true если соединение с БД корректно, иначе false
     */
    bool dbLinkIsCorrect();


    //GET-методы и SET-методы для параметров БД
    QString getHost() const;
    void setHost(const QString &value);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPass() const;
    void setPass(const QString &value);

    QString getDbName() const;
    void setDbName(const QString &value);

    int getPort() const;
    void setPort(int value);

    QSqlTableModel *getMapTableModel() const;
    void setSqlModel(QSqlTableModel *value);

    int getMapRowCount() const;
    void setMapRowCount(int value);

public slots:
    /**
     * @brief onImageInfoLoaded При загрузке данных
     */
    void onImageInfoLoaded();
    /**
     * @brief loadImageInfo Загрузка набора данных
     */
    void loadImageInfo();
    /**
     * @brief reconn Выполняет пересоединение с базой данных
     */
    void reconn();
    /**
     * @brief optionsAccepted Опции установлены
     */
    void optionsAccepted();
    /**
     * @brief optionsRejected Опции отклонены
     */
    void optionsRejected();
    /**
     * @brief loadImageInfoLoop Запускает петлю запросов
     * @param isLoaded true, если предыдущий запрос завершился удачно, иначе false
     */
    void loadImageInfoLoop(bool isLoaded);

private slots:
    /**
     * @brief onTagLoaded Вызывается при загрузке тега
     * @param tag
     */
    void onTagLoaded(QString tag);

signals:
    /**
     * @brief imageInfoLoaded При загрузке изображения
     */
    void imageInfoLoaded();
    /**
     * @brief recieveInfo Отправка информации об ошибке
     */
    void recieveInfo(Errors::ImgErrorCode, QString);
    /**
     * @brief dbConnectedStatus Проверка доступа к БД
     * @return true если соединение с БД установлено и корректно, иначе false
     */
    void dbConnectedStatus(bool);
    /**
     * @brief loadThisTagm Просит загрузить тег
     * @param tag Тег
     */
    void loadThisTag(QString tag);

private:
    void init();
    void sets();
    void conn();

    void connMainWindow();

    void createModel();

    void rebuildLimitsDistribution();
};

#endif
