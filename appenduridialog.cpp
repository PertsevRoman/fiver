#include "appenduridialog.h"
#include "ui_appenduridialog.h"

AppendUriDialog::AppendUriDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppendUriDialog) {
    ui->setupUi(this);
}

AppendUriDialog::~AppendUriDialog() {
    delete ui;
}

QString AppendUriDialog::getUri() {
    return ui->uri->text();
}

int AppendUriDialog::getExpertMark() {
    return ui->expertMark->value();
}

void AppendUriDialog::clear() {
    ui->uri->setText("");
    ui->expertMark->setValue(ui->expertMark->minimum());
}
