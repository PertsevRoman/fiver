#include <string>
#include <vector>
#include <exception>

#ifndef __TriangleNumber_h__
#define __TriangleNumber_h__

#include <QObject>
#include <QMap>

#include <boost/tuple/tuple.hpp>

#include <gsl/gsl_integration.h>

class TriangleNumber;

//Пара номер -- строка-идентификатор
typedef boost::tuple<TriangleNumber, std::string> valsBind;

class TriangleNumber {
    double center;
    double right;
    double left;

    //Ключевые идентификаторы
    static QMap<int, valsBind> baseBinds;

    //обслуживающие данные для интегрирования
    int integrationCount;
    gsl_integration_workspace *w;

public:
    //Конструктор-деструктор
    explicit TriangleNumber(double c = .0, double l = .0, double r = .0);
    ~TriangleNumber();

    //GET-методы и SET-методы для параметров треугольного числа
    double getCenter();
    void setCenter(double center);
    double getRight();
    void setRight(double right);
    double getLeft();
    void setLeft(double left);

    TriangleNumber operator +(TriangleNumber &r);
    TriangleNumber& operator +=(TriangleNumber &r);
    bool operator ==(const TriangleNumber &r);
    bool operator <(const TriangleNumber &r);
    bool operator >(const TriangleNumber &r);
    /**
     * @brief getCriteria возвращает значние критерия сравнения
     * @return критерий сравнения
     */
    double getCriteria() const;
    /**
     * @brief setBaseBind Добавляет связку для значения слайдера
     * @param key Ключ -- значение слайдера
     * @param num Треугольное число -- используется в расчетах
     * @param strID Строковый идентификатор
     */
    static void addBaseBind(int key, TriangleNumber num, std::string strID);
    /**
     * @brief getBindedTriangle Возвращает сязанное с ключом треугольное число
     * @param key Ключ
     * @return Треугольное число
     */
    static TriangleNumber getBindedTriangle(int key);
    /**
     * @brief getBindedStrID Возвращает сязанный с ключом строковый идентификатор
     * @param key Ключ
     * @return
     */
    static std::string getBindedStrID(int key);
};

#endif
