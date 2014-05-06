#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog) {
    ui->setupUi(this);

    init();
    sets();
    conn();
}

OptionsDialog::~OptionsDialog() {
    delete ui;
}

QString OptionsDialog::getHost() const {
    return ui->dbHostName->text();
}

void OptionsDialog::setHost(const QString &value) {
    ui->dbHostName->setText(value);
}

QString OptionsDialog::getUsername() const {
    return ui->userName->text();
}

void OptionsDialog::setUsername(const QString &value) {
    ui->userName->setText(value);
}

QString OptionsDialog::getPass() const {
    return ui->pass->text();
}

void OptionsDialog::setPass(const QString &value) {
    ui->pass->setText(value);
}

QString OptionsDialog::getDbName() const {
    return ui->dbName->text();
}

void OptionsDialog::setDbName(const QString &value) {
    ui->dbName->setText(value);
}

int OptionsDialog::getPort() const {
    return ui->dbPort->value();
}

void OptionsDialog::setPort(int value) {
    ui->dbPort->setValue(value);
}

QString OptionsDialog::getDictPath() const {
    return ui->dictFileName->text();
}

void OptionsDialog::setDictPath(const QString &value) {
    ui->dictFileName->setText(value);
}

void OptionsDialog::init() {
}

void OptionsDialog::sets() {
}

void OptionsDialog::conn() {
    connect(ui->chooseDictPathBtn, &QPushButton::released, this, &OptionsDialog::chooseDictPath);
    connect(ui->saveOptionsBtn, &QPushButton::released, this, &OptionsDialog::save);
    connect(ui->loadOptionsBtn, &QPushButton::released, this, &OptionsDialog::load);
}

void OptionsDialog::chooseDictPath() {
    QString path = QFileDialog::getOpenFileName(this, "Открыть файл словаря", "/opt", "XML (*.xml)");

    if(path.size()) {
        if(QFileInfo(path).exists() && QFileInfo(path).isReadable()) {
            ui->dictFileName->setText(path);
        }
    }
}

void OptionsDialog::save() {
    QString path = QFileDialog::getSaveFileName(this, "Сохранить настройки", "/home/joker/.opts", "Файлы опций (*.opts)");

    if(path.size()) {
        if(QFileInfo(path).dir().exists()) {
            QFile file(path);
            if(file.open(QIODevice::WriteOnly) && file.isWritable()) {
                QDataStream fileStream(&file);

                fileStream << ui->dbHostName->text();
                fileStream << ui->dbName->text();
                fileStream << ((qint32) ui->dbPort->value());
                fileStream << ui->userName->text();
                fileStream << ui->pass->text();
                fileStream << ui->dictFileName->text();

                if(fileStream.status() != QDataStream::Ok) {
                    QMessageBox::critical(this, "Ошибка", "Во время записи файла произошла ошибка");
                }

                file.close();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удается записать файл");
            }
        }
    }
}

void OptionsDialog::load() {
    QString path = QFileDialog::getOpenFileName(this, "Открыть файл словаря", "/home/joker", "Файлы опций (*.opts)");

    if(path.size()) {
        if(QFileInfo(path).exists() && QFileInfo(path).isReadable()) {
            QFile file(path);
            if(file.open(QIODevice::ReadOnly)) {
                QDataStream fileStream(&file);

                QString dbHostName, dbName, userName, pass, dictFileName;
                qint32 port;

                fileStream >> dbHostName;
                fileStream >> dbName;
                fileStream >> port;
                fileStream >> userName;
                fileStream >> pass;
                fileStream >> dictFileName;

                if(fileStream.status() != QDataStream::Ok) {
                    QMessageBox::critical(this, "Ошибка", "Не удалось корректно прочитать файл");
                    file.close();
                    return;
                }

                ui->dbHostName->setText(dbHostName);
                ui->dbName->setText(dbName);
                ui->dbPort->setValue(port);
                ui->userName->setText(userName);
                ui->pass->setText(pass);
                ui->dictFileName->setText(dictFileName);

                file.close();
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удается открыть файл");
            }
        }
    }
}
