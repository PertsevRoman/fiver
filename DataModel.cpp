#include "DataModel.h"
#include "TriangleNumber.h"

DataModel::DataModel(QObject *parent) : QObject(parent) {
    init();
    sets();
    conn();
}

DataModel::~DataModel() {
}

MainWindow *DataModel::getMainWindow() const {
    return mainWindow;
}

void DataModel::setMainWindow(MainWindow *value) {
    mainWindow = value;

    connMainWindow();
}

QString DataModel::getNextRandomTag() {
    QString res = "";

    if(dbLinkIsCorrect()) {
        emit recieveInfo(Errors::NoError, "Запрос тега");

        if(db.driver()->hasFeature(QSqlDriver::Transactions)) {
            db.transaction();
        }

        QSqlQuery query(db);
        if(query.exec("SELECT article FROM map ORDER BY RAND() LIMIT 1")) {
            query.next();
            res = query.value(0).toString();
        } else {
            emit recieveInfo(Errors::Error, "Не удалось запросить тег");
        }

        if(db.driver()->hasFeature(QSqlDriver::Transactions)) {
            db.commit();
        }

        emit recieveInfo(Errors::NoError, "Тег запрошен");
    } else {
        emit recieveInfo(Errors::Error, "При запросе тега определена некорректность соединения с БД");
    }

    return res;
}

bool DataModel::dbLinkIsCorrect() {
    return db.isOpen() && db.isValid();
}

std::pair<std::string, std::vector<std::string> > DataModel::getNextImageData() {
    std::pair<std::string, std::vector<std::string> > res;

    res.first   = imageConnector->getImageURL();
    res.second  = imageConnector->getTagCollection();

    return res;
}

void DataModel::appendMark() {
    std::vector<std::string> data = getNextImageData().second;
    int vote = mainWindow->getCurrentMark();

    boost::thread *th = new boost::thread([this, data, vote]() {
        QSqlDatabase cloned = QSqlDatabase::cloneDatabase(db, "CLONED_CONNECT");
        cloned.open();

        QSqlQuery queryIDS(cloned);
        std::string articles = "\"" + data.at(0);

        if(data.size() > 1) {
            for(int i = 1; i < data.size(); i++) {
                articles = articles + QString("\",\"").toStdString() + QString::fromStdString(data.at(i)).toUpper().toStdString();
            }
        }

        articles += "\"";

        mx.lock();

        if(cloned.driver()->hasFeature(QSqlDriver::Transactions)) {
            cloned.transaction();
        }

        queryIDS.exec("SELECT id FROM map WHERE article IN (" + QString::fromStdString(articles) + ")");

        QSqlQuery queryInsert(cloned);
        queryInsert.prepare("INSERT INTO votes(article_id, vote) VALUES (:article_id, \'" + QString::fromStdString(TriangleNumber::getBindedStrID(vote)) + "\')");

        while (queryIDS.next()) {
            queryInsert.bindValue(":article_id", queryIDS.value(0).toInt());

            if(!queryInsert.exec()) {
                if(cloned.driver()->hasFeature(QSqlDriver::Transactions)) {
                    cloned.rollback();
                }
            }

            qDebug() << "Последний запрос: " << queryInsert.lastQuery();
        }

        if(cloned.driver()->hasFeature(QSqlDriver::Transactions)) {
            cloned.commit();
        }

        mx.unlock();

        cloned.close();
    });

    th->detach();
}

TriangleNumber DataModel::getMark(std::string article) {
	throw "Not yet implemented";
}

bool DataModel::articleExists(std::string article) {
	throw "Not yet implemented";
}

void DataModel::refreshMarks() {
    throw "Not yet implemented";
}

void DataModel::onImageInfoLoaded() {
    if(imageConnector->getImageURL().size() && imageConnector->getTagCollection().size()) {
        emit imageInfoLoaded();
        emit recieveInfo(Errors::NoError, "Информация загружена");
    }
}

void DataModel::loadImageInfo() {
    if(sender()) {
        QPushButton* app = qobject_cast<QPushButton*>(sender());
        app->setEnabled(false);

        appendMark();
    }

    loadImageInfoLoop(false);
}

void DataModel::reconn() {
    if(db.isOpen()) {
        db.close();
    }

    //Запись парметров
    db.setHostName(getHost());
    db.setPort(getPort());
    db.setPassword(getPass());
    db.setUserName(getUsername());
    db.setDatabaseName(getDbName());

    //Открытие базы
    db.open();

    if(dbLinkIsCorrect()) {
        emit recieveInfo(Errors::NoError, "Доступ к БД открыт");

        //Создание модели
        createModel();
    } else {
        emit recieveInfo(Errors::Error, "Не удалось получить доступ к БД");
    }

    emit dbConnectedStatus(dbLinkIsCorrect());
}

void DataModel::optionsAccepted() {
    OptionsDialog* dlg = qobject_cast<OptionsDialog*>(sender());

    setHost(dlg->getHost());
    setPort(dlg->getPort());
    setDbName(dlg->getDbName());
    setUsername(dlg->getUsername());
    setPass(dlg->getPass());

    reconn();

    if(mainWindow) {
        mainWindow->stateChanged();
    }
}

void DataModel::optionsRejected() {
    OptionsDialog* dlg = qobject_cast<OptionsDialog*>(sender());

    dlg->setHost(getHost());
    dlg->setPort(getPort());
    dlg->setDbName(getDbName());
    dlg->setUsername(getUsername());
    dlg->setPass(getPass());

    emit dbConnectedStatus(db.isOpen() && db.isValid());
}

void DataModel::loadImageInfoLoop(bool isLoaded) {
    if(!isLoaded) {
        imageConnector->request(getNextRandomTag().toLower().toStdString());
    }
}

void DataModel::init() {
    imageConnector = new YandexImageConnector(this);
    db = QSqlDatabase::addDatabase("QMYSQL");
}

void DataModel::sets() {
}

void DataModel::conn() {
    connect(imageConnector, &YandexImageConnector::finished, this, &DataModel::onImageInfoLoaded);
    connect(imageConnector, &YandexImageConnector::infoLoadedCorrectly, this, &DataModel::loadImageInfoLoop);
}

void DataModel::connMainWindow() {
    if(mainWindow) {
        connect(imageConnector, &YandexImageConnector::recieveInfo, mainWindow, &MainWindow::recieveMessage);
        connect(imageConnector, &YandexImageConnector::loaded, mainWindow, &MainWindow::downloadProgress);
        connect(this, &DataModel::recieveInfo, mainWindow, &MainWindow::recieveMessage);
    }
}

QString DataModel::getHost() const {
    return host;
}

void DataModel::setHost(const QString &value) {
    host = value;
}

QString DataModel::getUsername() const {
    return username;
}

void DataModel::setUsername(const QString &value) {
    username = value;
}

QString DataModel::getPass() const {
    return pass;
}

void DataModel::setPass(const QString &value) {
    pass = value;
}

QString DataModel::getDbName() const {
    return dbName;
}

void DataModel::setDbName(const QString &value) {
    dbName = value;
}

int DataModel::getPort() const {
    return port;
}

void DataModel::setPort(int value) {
    port = value;
}

QSqlTableModel *DataModel::getMapTableModel() const {
    return sqlModel;
}

void DataModel::setSqlModel(QSqlTableModel *value) {
    sqlModel = value;
}

void DataModel::createModel() {
    sqlModel = new QSqlTableModel(this, db);
    sqlModel->setTable("map");
    sqlModel->setHeaderData(0, Qt::Horizontal, "Идентификатор");
    sqlModel->setHeaderData(1, Qt::Horizontal, "Имя статьи");
    sqlModel->setHeaderData(2, Qt::Horizontal, "Определение");
    sqlModel->setHeaderData(3, Qt::Horizontal, "Оценка тональности");
}
