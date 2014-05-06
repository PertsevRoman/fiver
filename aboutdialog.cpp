#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    connect(ui->okBtn, &QPushButton::released, this, &AboutDialog::accept);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
