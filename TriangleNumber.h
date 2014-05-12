#include <string>
#include <vector>
#include <exception>

#ifndef __TriangleNumber_h__
#define __TriangleNumber_h__

#include <QObject>
#include <QMap>
#include <QDebug>

#include <boost/tuple/tuple.hpp>

#include <o2scl/funct.h>
#include <o2scl/inte_qag_gsl.h>

class TriangleNumber;

//Пара номер -- строка-идентификатор
typedef boost::tuple<TriangleNumber, std::string> valsBind;

class TriangleNumber {
    double center;
    double right;
    double left;

    //Ключевые идентификаторы
    static QMap<int, valsBind> baseBinds;
public:
    //Конструктор-деструктор
    TriangleNumber(double c = .0, double l = .0, double r = .0);
    TriangleNumber(const TriangleNumber & c);
    ~TriangleNumber();

    //GET-методы и SET-методы для параметров треугольного числа
    double getCenter();
    void setCenter(double center);
    double getRight();
    void setRight(double right);
    double getLeft();
    void setLeft(double left);

    TriangleNumber & operator = (const TriangleNumber & r);
    TriangleNumber operator +(TriangleNumber &r);
    TriangleNumber operator -(TriangleNumber &r);
    TriangleNumber& operator +=(TriangleNumber &r);
    bool operator ==(const TriangleNumber &r) const;
    bool operator <(const TriangleNumber &r) const;
    bool operator >(const TriangleNumber &r) const;

    /**
     * @brief getCriteriaFuncValue Возвращает значение функции критерия
     * @param x параметр функции
     */
    double getCriteriaFuncValue(double x);
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
    /**
     * @brief getBindedStrID Возвращает строку, связаную с числом
     * @param num Треугольное число
     * @return Строковый идентификатор
     */
    static std::string getBindedStrID(const TriangleNumber & num);
};

#endif
