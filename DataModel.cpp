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
            qDebug() << "Начало транзакции";
            db.transaction();
        }

        mx.lock();

        QSqlQuery query(db);
        if(query.exec("SELECT COUNT(*) FROM map")) {
            query.next();
            int rowCount = query.value(0).toInt();

            boost::random::uniform_int_distribution<> limits(1, rowCount);
            int randRow = limits(rng);

            if(query.exec("SELECT article FROM map LIMIT " + QString("%1").arg(randRow) + ", 1;")) {
                query.next();
                res = query.value(0).toString();
            } else {
                emit recieveInfo(Errors::Error, "Не удалось запросить тег");
            }
        } else {
            emit recieveInfo(Errors::Error, "Не удалось запросить количество записей");
        }

        if(db.driver()->hasFeature(QSqlDriver::Transactions)) {
            qDebug() << "Конец транзакции";
            db.commit();
        }

        mx.unlock();

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
            qDebug() << "Начало транзакции";
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
            qDebug() << "Конец транзакции";
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
    QString request = "SELECT count(*) FROM map WHERE article = \"" + QString::fromStdString(article) + "\"";

    QSqlQuery qu(db);
    if(qu.exec(request)) {
        qu.next();

        int res = qu.value(0).toInt();

        return res > 0;
    } else {
        emit recieveInfo(Errors::Error, "Не прошел запрос количества вхождений статьи");
        return false;
    }
}

void DataModel::refreshMarks() {
    boost::thread *th = new boost::thread([this](){
        emit recieveInfo(Errors::NoError, "Обновление оценок");
        QSqlDatabase cloned = QSqlDatabase::cloneDatabase(db, "CLONED_CONNECT2");

        cloned.open();

        QString queryStr = QString("SELECT article_id, sum(vote = \"NB\") as NB, ") +
                QString("sum(vote = \"NM\") as NM, sum(vote = \"NS\") as NS,") +
                QString("sum(vote = \"NO\") as NO, sum(vote = \"ZO\") as ZO,") +
                QString("sum(vote = \"PO\") as PO, sum(vote = \"PS\") as PS,") +
                QString("sum(vote = \"PM\") as PM, sum(vote = \"PB\") as PB") +
                QString(" FROM marks.votes where article_id in") +
                QString(" (select distinct(article_id) FROM marks.votes) group by article_id;");

        if(cloned.driver()->hasFeature(QSqlDriver::Transactions)) {
            qDebug() << "Начало транзакции";
            cloned.transaction();
        }

        QSqlQuery votes(queryStr, cloned);
        votes.exec();

        TriangleMathCollector collector;

        QString updStr = "UPDATE map SET artvote = :vote_val WHERE id = :id_val;";
        QSqlQuery update(cloned);
        update.prepare(updStr);

        while(votes.next()) {
            collector.clear();

            int id = votes.value(0).toInt();

            update.bindValue(":id_val", id);

            for(int i = 1; i < 10; i++) {
                int count = votes.value(i).toInt();

                for(int j = 0; j < count; j++) {
                    TriangleNumber tr = TriangleNumber::getBindedTriangle(i - 5);
                    collector.add(tr);
                }
            }

            QString voteStr = QString::fromStdString(TriangleNumber::getBindedStrID(collector.getMedian()));
            if(voteStr.size()) {
                update.bindValue(":vote_val", voteStr);
                if(!update.exec()) {
                    qDebug() << "Последний провалившийся запрос: " << update.executedQuery() << "; vote, id = " << voteStr << ", " << id;
                    if(cloned.driver()->hasFeature(QSqlDriver::Transactions)) {
                        cloned.rollback();
                    }

                    emit recieveInfo(Errors::Error, "Во время выполнения обновления произошла ошибка");
                    break;
                } else {
                    qDebug() << "Последний успешный запрос: " << update.executedQuery();
                }
            }
        }

        if(cloned.driver()->hasFeature(QSqlDriver::Transactions)) {
            qDebug() << "Конец транзакции";
            cloned.commit();
        }

        cloned.close();
        emit recieveInfo(Errors::NoError, "Обновление оценок завершено");
    });

    th->detach();
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
        emit recieveInfo(Errors::Error, "Не удалось получить доступ к БД: " + db.lastError().databaseText() + ", " + db.lastError().driverText());
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
