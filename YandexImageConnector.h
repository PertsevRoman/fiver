#include <string>
#include <vector>
#include <exception>

#ifndef __YandexImageConnector_h__
#define __YandexImageConnector_h__

#include <QObject>
#include <QtNetwork>
#include <QUrl>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/thread.hpp>
#include <boost/regex.hpp>

#include <htmlcxx/html/ParserDom.h>

#include "codes.h"

class YandexImageConnector : public QObject {
    Q_OBJECT

private:
    std::string tag;
    bool        operating;
    std::string imageURL;
    std::vector<std::string> tagCollection;

    //Менеджер доступа
    QNetworkAccessManager *usersConnectManager;
    QNetworkAccessManager *paramsConnectManager;

    //Генератор псевдослучайных чисел
    boost::random::mt19937 gen;

    //Персер DOM
    htmlcxx::HTML::ParserDom *parser;

public:
    /**
     * @brief YandexImageConnector Констуктор
     * @param parent Родитель
     */
    explicit YandexImageConnector(QObject* parent = 0);
    ~YandexImageConnector();

    /**
     * @brief getTag возвращает обрабатываемый тег
     * @return тег
     */
    std::string getTag();
    /**
     * @brief setTag устанавливает обрабатываемый тег
     * @param tag тег
     */
    void setTag(std::string tag);
    /**
     * @brief onOperating проверка занятости объекта
     * @return true - объект выполняет какую-либо операцию, иначе false
     */
    bool onOperating();
    /**
     * @brief getTagCollection возвращает полученную коллекцию тегов
     * @return коллекция тегов
     */
    std::vector<std::string> getTagCollection();
    /**
     * @brief getImageURL возвращает ссылку на изображение
     * @return
     */
    std::string getImageURL();

public slots:
    /**
     * @brief request запрашивает данные по тегу
     * @param tag тег
     */
    void request(std::string tag);
    /**
     * @brief request запрашивает данные
     */
    void request();

private:
    void init();
    void sets();
    void conn();

    void debugResults();

    /**
     * @brief precompileTag Преобразует тег для выполнения запроса
     * @param tag тег
     * @return преобразованный тег
     */
    static void precompileTag(std::string &tag);

private slots:
    /**
     * @brief extractUsers Проводит извлечение имен пользователей и запрос фото
     * @param reply Ответ сервера
     */
    void extractUsers(QNetworkReply* reply);
    /**
     * @brief extractPhoto Проводит извлечение фотографии
     * @param reply Ответ сервера
     */
    void extractPhoto(QNetworkReply* reply);
    /**
     * @brief getLuckyEntries Возвращает набор объектов
     * @param lucky Имя пользователя
     */
    void getLuckyEntries(QString lucky);
    /**
     * @brief dataLoaded Индикация загрузки данных
     * @param bytes Принято байт
     * @param total Всего байт
     */
    void dataLoaded(qint64 bytes, qint64 total);

signals:
    /**
     * @brief recieveInfo Отправка информации об ошибке
     */
    void recieveInfo(Errors::ImgErrorCode, QString);
    /**
     * @brief thisALucky Отправляет имя пользователя
     */
    void thisALucky(QString);
    void loaded(qint64, qint64);

    //Старт, финиш
    void started();
    void finished();

    /**
     * @brief loadedCorrectly true, если информацию удалось корректно загрузить
     */
    void infoLoadedCorrectly(bool);
};

#endif
