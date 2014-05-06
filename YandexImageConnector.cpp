#include "YandexImageConnector.h"

YandexImageConnector::YandexImageConnector(QObject *parent) : QObject(parent) {
    init();
    sets();
    conn();
}

YandexImageConnector::~YandexImageConnector() {
    delete parser;
}

std::string YandexImageConnector::getTag() {
    return this->tag;
}

void YandexImageConnector::setTag(std::string tag) {
    if(this->tag != tag) {
        YandexImageConnector::precompileTag(tag);

        qDebug() << "Тег: " << QString::fromStdString(tag);

        this->tag = tag;
    }
}

void YandexImageConnector::request() {
    tagCollection.clear();
    imageURL.clear();

    emit started();

    if(tag.size()) {
        //Начало операции - извлечение пользоватлей
        emit recieveInfo(Errors::NoError, "Запрос имен пользователей");
        operating = true;
        QNetworkReply* repl = usersConnectManager->get(QNetworkRequest(QUrl
            ("http://fotki.yandex.ru/search.xml?text=" + QString::fromStdString(tag) + "&tags=1")));
        connect(repl, &QNetworkReply::downloadProgress, this, &YandexImageConnector::dataLoaded);
    } else {
        emit finished();
        //emit recieveInfo(Errors::Error, "Не установлен тег");
    }
}

bool YandexImageConnector::onOperating() {
    return this->operating;
}

std::vector<std::string> YandexImageConnector::getTagCollection() {
    return this->tagCollection;
}

std::string YandexImageConnector::getImageURL() {
    return this->imageURL;
}

void YandexImageConnector::request(std::string tag) {
    setTag(tag);
    request();
}

void YandexImageConnector::extractUsers(QNetworkReply *reply) {
    disconnect(reply, &QNetworkReply::downloadProgress, this, &YandexImageConnector::dataLoaded);
    if(reply->error() == QNetworkReply::NoError) {
        //Извлечение данных
        QByteArray byteData = reply->readAll();

        //Анализ данных
        boost::thread *th = new boost::thread([=](){
            //Набор пользователей
            emit recieveInfo(Errors::NoError, "Вырезка имен пользователей");
            std::vector<std::string> users;
            tree<htmlcxx::HTML::Node> domNode = parser->parseTree(QString(byteData).toStdString());

            tree<htmlcxx::HTML::Node>::iterator it      = domNode.begin();
            tree<htmlcxx::HTML::Node>::iterator end     = domNode.end();

            for(; it != end; ++it) {
                if(it->tagName() == "a") {
                    it->parseAttributes();

                    if(it->attribute("class").first && it->attribute("class").second == "b-user__link") {
                        std::string hRef = it->attribute("href").second;

                        boost::regex    ex("http://fotki.yandex.ru/users/([a-z0-9]+)/");
                        boost::smatch   cutRes;

                        std::string::const_iterator st = hRef.begin();
                        std::string::const_iterator ed = hRef.end();

                        while(boost::regex_search(st, ed, cutRes, ex)) {
                            users.push_back(cutRes[1]);
                            st = cutRes[1].second;
                        }
                    }
                }
            }

            if(users.size()) {
                boost::random::uniform_int_distribution<> dist(0, users.size() - 1);
                std::string lucky = users.at(dist(gen));

                emit thisALucky(QString::fromStdString(lucky));
            } else {
                operating = false;
                emit finished();
                //emit recieveInfo(Errors::Error, "Документ пришел, но парсинг пользователей не дал результатов. Проверьте соединение");
                emit infoLoadedCorrectly(false);
            }

            reply->deleteLater();
        });

        th->detach();
    } else {
        operating = false;
        emit finished();
        //emit recieveInfo(Errors::Error, "Не извлечено имя пользователя");
        emit infoLoadedCorrectly(false);
        reply->deleteLater();
    }
}

void YandexImageConnector::extractPhoto(QNetworkReply *reply) {
    disconnect(reply, &QNetworkReply::downloadProgress, this, &YandexImageConnector::dataLoaded);
    if(reply->error() == QNetworkReply::NoError) {
        boost::thread *th = new boost::thread([=](){
            std::string strData = QString(reply->readAll()).toStdString();

            //Вырезка entrie-вхождений
            emit recieveInfo(Errors::NoError, "Вырезка entrie-вхождений");
            std::vector<std::string> entries;

            boost::regex ex("<entry>(.*?)</entry>");

            boost::sregex_iterator it(strData.begin(), strData.end(), ex);
            boost::sregex_iterator invalid;

            while(it != invalid) {
                entries.push_back((*it++).str());
            }

            //Выбор entrie-вхождения
            emit recieveInfo(Errors::NoError, "Выбор entrie-вхождения");
            std::string entr;
            if(entries.size()) {
                boost::random::uniform_int_distribution<> dist(0, entries.size() - 1);
                entr = entries.at(dist(gen));
            } else {
                operating = false;
                emit finished();
                //emit recieveInfo(Errors::Error, "Коллекция загружена, но поиск по регулярному выражению не удался");
                emit infoLoadedCorrectly(false);
                return;
            }

            if(tagCollection.size()) {
                tagCollection.clear();
            }

            //Инициализация дерева
            emit recieveInfo(Errors::NoError, "Инициализация дерева");
            tree<htmlcxx::HTML::Node> domNode = parser->parseTree(entr);

            //Вырезка тегов
            emit recieveInfo(Errors::NoError, "Вырезка тегов");
            tree<htmlcxx::HTML::Node>::iterator st  = domNode.begin();
            tree<htmlcxx::HTML::Node>::iterator end = domNode.end();
            for(; st != end; ++st) {
                if(st->tagName() == "category") {
                    st->parseAttributes();

                    if(st->attribute("scheme").first) {
                        if(st->attribute("term").first) {
                            tagCollection.push_back(st->attribute("term").second);
                        }
                    }
                }
            }

            //Вырезка ссылки на изображение
            emit recieveInfo(Errors::NoError, "Вырезка ссылки на изображение");
            st  = domNode.begin();
            end = domNode.end();
            for(; st != end; ++st) {
                if(st->tagName() == "content") {
                    st->parseAttributes();

                    if(st->attribute("src").first) {
                        imageURL = st->attribute("src").second;
                        break;
                    }
                }
            }

            emit recieveInfo(Errors::NoError, "Извлечение информации заврешено");
            emit infoLoadedCorrectly(true);
            domNode.clear();
            emit finished();
            operating = false;

            reply->deleteLater();
        });

        th->detach();
    } else {
        emit finished();
        //emit recieveInfo(Errors::Error, "Не извлечена информация об изображении");
        operating = false;
        emit infoLoadedCorrectly(false);
        reply->deleteLater();
    }
}

void YandexImageConnector::getLuckyEntries(QString lucky) {
    qDebug() << "Счастливчик: " << lucky;
    QString url = "http://api-fotki.yandex.ru/api/users/" + lucky + "/tag/" + QString::fromStdString(tag) + "/photos/";
    qDebug() << "URL: " << url;

    emit recieveInfo(Errors::NoError, "Запрос коллекции фото");
    QNetworkReply* repl = paramsConnectManager->get(QNetworkRequest(QUrl(url)));
    connect(repl, &QNetworkReply::downloadProgress, this, &YandexImageConnector::dataLoaded);
}

void YandexImageConnector::dataLoaded(qint64 bytes, qint64 total) {
    emit loaded(bytes, total);
}

void YandexImageConnector::init() {
    usersConnectManager = new QNetworkAccessManager(this);
    paramsConnectManager = new QNetworkAccessManager(this);

    parser = new htmlcxx::HTML::ParserDom();
}

void YandexImageConnector::sets() {
}

void YandexImageConnector::conn() {
    connect(usersConnectManager, &QNetworkAccessManager::finished,
                this, &YandexImageConnector::extractUsers);
    connect(paramsConnectManager, &QNetworkAccessManager::finished,
                this, &YandexImageConnector::extractPhoto);
    connect(this, &YandexImageConnector::thisALucky,
                this, &YandexImageConnector::getLuckyEntries);
}

void YandexImageConnector::debugResults() {
    qDebug() << "Вывод коллекции тегов";

    for(int i = 0; i < tagCollection.size(); i++) {
        qDebug() << QString::fromStdString(tagCollection.at(i));
    }
    qDebug() << "Ссылка на изображение: " << QString::fromStdString(imageURL);
}

void YandexImageConnector::precompileTag(std::string & tag) {
    std::replace(tag.begin(), tag.end(), ' ', '+');
}

