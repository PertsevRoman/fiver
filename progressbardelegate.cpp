#include "progressbardelegate.h"

ProgressBarDelegate::ProgressBarDelegate(QObject *parent) :
    QItemDelegate(parent) {
}

QWidget *ProgressBarDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QProgressBar* editor = new QProgressBar(parent);
    return editor;
}

void ProgressBarDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    int value = index.model()->data(index, Qt::EditRole | Qt::DisplayRole).toInt();

    QProgressBar *progress = static_cast<QProgressBar*>(editor);
    progress->setValue(value);
}

void ProgressBarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QProgressBar *progress = static_cast<QProgressBar*>(editor);
    int value = progress->value();

    model->setData(index, value, Qt::EditRole | Qt::DisplayRole);
}

void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) {
    int progress = index.data(Qt::DisplayRole).toInt();
    QStyleOptionProgressBar pbOption;

    pbOption.rect = option.rect;
    pbOption.minimum = 0;
    pbOption.maximum = 100;
    pbOption.progress = progress;
    pbOption.textAlignment = Qt::AlignHCenter;
    pbOption.textVisible = true;

    QApplication::style()->drawControl(QStyle::CE_ProgressBar, &pbOption, painter);
}
