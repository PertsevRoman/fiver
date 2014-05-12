#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    init();
    sets();
    conn();
}

MainWindow::~MainWindow() {
}

void MainWindow::resizeEvent(QResizeEvent *) {
    imageResize();
}

int MainWindow::getCurrentMark() {
    return ui->voteValue->value();
}

void MainWindow::init() {
    imageNam = new QNetworkAccessManager(this);
    mysqlDataModel = new DataModel(this);

    opts = new OptionsDialog(this);
    about = new AboutDialog(this);
    insertDlg = new InsertArticleDialog(this);

    progress    = new QProgressBar();
    procInfo    = new QLabel("Инициализация завершена");
    dbInfo      = new QLabel("Соединение с БД не инициализировано");

    oldStackWidget = ui->operationsStack->currentIndex();
}

void MainWindow::sets() {
    ui->status->addWidget(procInfo, 0);
    ui->status->addWidget(progress, 1);
    ui->status->addWidget(dbInfo, 0);

    mysqlDataModel->setMainWindow(this);

    //Инициализация меток
    voteDefends[-4] = "Сильно негативное";
    voteDefends[-3] = "Средне негативное";
    voteDefends[-2] = "Слегка негативное";
    voteDefends[-1] = "Негативное, близкое к нейтральному";
    voteDefends[0] = "Нейтральное";
    voteDefends[1] = "Положительное, близкое к нейтральному";
    voteDefends[2] = "Слегка положительное";
    voteDefends[3] = "Средне положительное";
    voteDefends[4] = "Сильно положительное";

    ui->appendVote->setText(voteDefends[0]);
}

void MainWindow::conn() {
    connect(ui->exitAction, &QAction::triggered, &QApplication::quit);
    connect(ui->vocEditAction, &QAction::triggered, this, &MainWindow::vocActionPerformed);
    connect(ui->vocFillAction, &QAction::triggered, this, &MainWindow::vocActionPerformed);
    connect(ui->analyseAction, &QAction::triggered, this, &MainWindow::toneActionPerformed);
    connect(ui->optionsAction, &QAction::triggered, opts, &OptionsDialog::show);

    //Диалог и модель данных
    connect(opts, &OptionsDialog::accepted, mysqlDataModel, &DataModel::optionsAccepted);
    connect(opts, &OptionsDialog::rejected, mysqlDataModel, &DataModel::optionsRejected);

    //Диалог "О программе.."
    connect(ui->aboutAction, &QAction::triggered, about, &AboutDialog::show);

    //Страница заполнения словаря
    connect(ui->appendVote, &QPushButton::released, mysqlDataModel, &DataModel::loadImageInfo);
    connect(mysqlDataModel, &DataModel::imageInfoLoaded, this, &MainWindow::loadImage);
    connect(mysqlDataModel, &DataModel::dbConnectedStatus, this, &MainWindow::dbCheck);
    connect(imageNam, &QNetworkAccessManager::finished, this, &MainWindow::imageLoaded);
    connect(ui->voteValue, &QSlider::valueChanged, this, &MainWindow::voteValChanged);
    connect(ui->operationsStack, &QStackedWidget::currentChanged, this, &MainWindow::operationChange);

    //Страница редактирования словаря
    connect(ui->deleteArticle, &QPushButton::released, this, &MainWindow::removeSelectedData);
    connect(ui->addArticle, &QPushButton::released, this, &MainWindow::appendDataAfterCurrent);
}

void MainWindow::imageResize() {
    if(!currentPixmap.isNull()) {
        int h = ui->image->height();
        int w = ui->image->width();

        ui->image->setPixmap(currentPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::vocActionPerformed() {
    if(sender() == ui->vocEditAction) {
        ui->operationsStack->setCurrentIndex(2);
        stateChanged();
    } else if(sender() == ui->vocFillAction) {
        ui->operationsStack->setCurrentIndex(0);
        stateChanged();
    }
}

void MainWindow::toneActionPerformed() {
    if(sender() == ui->analyseAction) {
        ui->operationsStack->setCurrentIndex(1);
    }
}

void MainWindow::recieveMessage(Errors::ImgErrorCode err, QString msg) {
    if(err == Errors::Error) {
        QMessageBox::critical(this, "Ошибка", msg);
    } else {
        procInfo->setText(msg);
    }
}

void MainWindow::downloadProgress(qint64 bytes, qint64 total) {
    if(total) {
        progress->setValue(bytes * 100 / total);
    }
}

void MainWindow::stateChanged() {
    if(mysqlDataModel->dbLinkIsCorrect()) {
        //Страница заполнения словаря
        if(ui->operationsStack->currentIndex() == 0) {
            ui->appendVote->setEnabled(true);
            mysqlDataModel->loadImageInfoLoop(false);
        } else if(ui->operationsStack->currentIndex() == 2) {
            if(mysqlDataModel->getMapTableModel() != ui->articleList->model()) {
                ui->articleList->setModel(mysqlDataModel->getMapTableModel());
                mysqlDataModel->getMapTableModel()->select();
            }
        }
    }
}

void MainWindow::loadImage() {
    if(ui->operationsStack->currentIndex() == 0) {
        //Инициализация изображения
        QString imgUrl = QString::fromStdString(qobject_cast<DataModel*>(sender())->getNextImageData().first);

        recieveMessage(Errors::NoError, "Загрузка изображения");

        //Попытка загрузить изображение
        QNetworkReply* repl = imageNam->get(QNetworkRequest(QUrl(imgUrl)));
        connect(repl, &QNetworkReply::downloadProgress, this, &MainWindow::downloadProgress);
    }
}

void MainWindow::imageLoaded(QNetworkReply *reply) {
    disconnect(reply, &QNetworkReply::downloadProgress, this, &MainWindow::downloadProgress);
    progress->setValue(0);

    ui->appendVote->setEnabled(true);

    if(ui->operationsStack->currentIndex() == 0) {
        if(QNetworkReply::NoError == reply->error()) {
            //Данные
            QImage img;
            QImageReader reader(reply);

            reader.read(&img);

            //Чтение изображения
            currentPixmap = QPixmap::fromImage(img);

            imageResize();
            recieveMessage(Errors::NoError, "Изображение загружено");
        } else {
            recieveMessage(Errors::Error, "Не удалось загрузить изображение");
        }
    }
}

void MainWindow::dbCheck(bool ch) {
    if(ch) {
        dbInfo->setText("Соединено с БД");
    } else {
        dbInfo->setText("Соединение с БД отсутствует");
    }
}

void MainWindow::voteValChanged(int val) {
    ui->appendVote->setText(voteDefends[val]);
}

void MainWindow::operationChange(int val) {
    if(oldStackWidget == 0) {
        if(mysqlDataModel->dbLinkIsCorrect()) {
            mysqlDataModel->refreshMarks();
        }
    }

    oldStackWidget = val;
}

void MainWindow::removeSelectedData() {
    if(ui->articleList->currentIndex().isValid()) {
        int res = QMessageBox::warning(this, "Удаление данных", "Уверены, что хотите удалить данные?", QMessageBox::Ok | QMessageBox::Cancel);

        if(res == QMessageBox::Ok) {
            mysqlDataModel->getMapTableModel()->removeRow(ui->articleList->currentIndex().row());
            mysqlDataModel->getMapTableModel()->submitAll();
            mysqlDataModel->getMapTableModel()->select();
        }
    }
}

void MainWindow::appendDataAfterCurrent() {
    if(ui->articleList->currentIndex().isValid()) {
        insertDlg->show();
    }
}

