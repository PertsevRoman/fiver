#ifndef PROGRESSBARDELEGATE_H
#define PROGRESSBARDELEGATE_H

#include <QItemDelegate>
#include <QProgressBar>
#include <QStyleOptionProgressBar>
#include <QApplication>

class ProgressBarDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit ProgressBarDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void paint(QPainter *painter,const QStyleOptionViewItem &option,const QModelIndex &index);
};

#endif // PROGRESSBARDELEGATE_H
