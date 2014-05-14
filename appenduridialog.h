#ifndef APPENDURIDIALOG_H
#define APPENDURIDIALOG_H

#include <QDialog>

namespace Ui {
class AppendUriDialog;
}

class AppendUriDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppendUriDialog(QWidget *parent = 0);
    ~AppendUriDialog();

    QString getUri();
    int getExpertMark();

    void clear();

private:
    Ui::AppendUriDialog *ui;
};

#endif // APPENDURIDIALOG_H
