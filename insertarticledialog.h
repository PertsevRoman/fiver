#ifndef INSERTARTICLEDIALOG_H
#define INSERTARTICLEDIALOG_H

#include <QDialog>
#include <QMap>

namespace Ui {
class InsertArticleDialog;
}

class InsertArticleDialog : public QDialog
{
    Q_OBJECT

    //Оценки тональности
    QMap<int, QString> voteDefends;

public:
    explicit InsertArticleDialog(QWidget *parent = 0);
    ~InsertArticleDialog();

    QString getArticleName();
    QString getArticleDescription();
    int getVote();

private:
    Ui::InsertArticleDialog *ui;

    void init();
    void sets();
    void conn();

private slots:
    void setVoteLabel(int val);
};

#endif // INSERTARTICLEDIALOG_H
