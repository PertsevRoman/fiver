#include "changemarkdialog.h"
#include "ui_changemarkdialog.h"

ChangeMarkDialog::ChangeMarkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeMarkDialog) {
    ui->setupUi(this);

    init();
    sets();
    conn();
}

ChangeMarkDialog::~ChangeMarkDialog() {
    delete ui;
}

void ChangeMarkDialog::init() {
    voteDefends[-4] = "Сильно негативное";
    voteDefends[-3] = "Средне негативное";
    voteDefends[-2] = "Слегка негативное";
    voteDefends[-1] = "Негативное, близкое к нейтральному";
    voteDefends[0] = "Нейтральное";
    voteDefends[1] = "Положительное, близкое к нейтральному";
    voteDefends[2] = "Слегка положительное";
    voteDefends[3] = "Средне положительное";
    voteDefends[4] = "Сильно положительное";

    voteValueChanged(0);
}

void ChangeMarkDialog::sets() {
}

void ChangeMarkDialog::conn() {
    connect(ui->voteValue, &QSlider::valueChanged, this, &ChangeMarkDialog::voteValueChanged);
}

int ChangeMarkDialog::getVoteMark() {
    return ui->voteValue->value();
}

void ChangeMarkDialog::voteValueChanged(int val) {
    ui->voteLabel->setText(voteDefends[val]);
}
