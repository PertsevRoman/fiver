#include "MainWindow.h"
#include "TriangleNumber.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //Заполнение треугольных чисел
    TriangleNumber::addBaseBind(-4, TriangleNumber(-1.0, 0.0, 0.25), QString("NB").toStdString());
    TriangleNumber::addBaseBind(-3, TriangleNumber(-0.75, 0.25, 0.25), QString("NM").toStdString());
    TriangleNumber::addBaseBind(-2, TriangleNumber(-0.5, 0.25, 0.25), QString("NS").toStdString());
    TriangleNumber::addBaseBind(-1, TriangleNumber(-0.25, 0.25, 0.25), QString("NO").toStdString());
    TriangleNumber::addBaseBind(0, TriangleNumber(.0, 0.25, 0.25), QString("ZO").toStdString());
    TriangleNumber::addBaseBind(1, TriangleNumber(0.25, 0.25, 0.25), QString("PO").toStdString());
    TriangleNumber::addBaseBind(2, TriangleNumber(0.5, 0.25, 0.25), QString("PS").toStdString());
    TriangleNumber::addBaseBind(3, TriangleNumber(0.75, 0.25, 0.25), QString("PM").toStdString());
    TriangleNumber::addBaseBind(4, TriangleNumber(1, 0.25, 0), QString("PB").toStdString());

    //Главное окно
    MainWindow w;
    w.show();

    return a.exec();
}
