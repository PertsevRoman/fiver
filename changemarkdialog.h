#ifndef CHANGEMARKDIALOG_H
#define CHANGEMARKDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class ChangeMarkDialog;
}

class ChangeMarkDialog : public QDialog
{
    Q_OBJECT

    //Оценки тональности
    QMap<int, QString> voteDefends;

public:
    explicit ChangeMarkDialog(QWidget *parent = 0);
    ~ChangeMarkDialog();

    void init();
    void sets();
    void conn();

    int getVoteMark();

private:
    Ui::ChangeMarkDialog *ui;

private slots:
    void voteValueChanged(int val);
};

#endif // CHANGEMARKDIALOG_H
