#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

    Ui::OptionsDialog *ui;

public:
    explicit OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();

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

    //GET-методы и SET-методы для параметров словаря
    QString getDictPath() const;
    void setDictPath(const QString &value);

private:
    void init();
    void sets();
    void conn();

private slots:
    void chooseDictPath();

    void save();
    void load();
};

#endif // OPTIONSDIALOG_H
