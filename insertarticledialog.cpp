#include "insertarticledialog.h"
#include "ui_insertarticledialog.h"

InsertArticleDialog::InsertArticleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertArticleDialog) {
    ui->setupUi(this);

    init();
    sets();
    conn();
}

InsertArticleDialog::~InsertArticleDialog() {
    delete ui;
}

void InsertArticleDialog::init() {
}

void InsertArticleDialog::sets() {
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
}

void InsertArticleDialog::conn() {
    connect(ui->voteValue, &QSlider::valueChanged, this, &InsertArticleDialog::setVoteLabel);
}

void InsertArticleDialog::setVoteLabel(int val) {
    ui->voteLabel->setText(voteDefends[val]);
}
