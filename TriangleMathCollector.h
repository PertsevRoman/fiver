#include <string>
#include <exception>
using namespace std;

#ifndef __TriangleMathCollector_h__
#define __TriangleMathCollector_h__

#include <QVector>
#include "TriangleNumber.h"

class TriangleNumber;
class TriangleMathCollector;

class TriangleMathCollector
{
    QVector<TriangleNumber> triangles;

public:
    /**
     * @brief getSum возвращает сумму треугольных чисел в коллекторе
     * @return сумма треугольных чисел
     */
    TriangleNumber getSum();
    /**
     * @brief getMedian возвращает медиану треугольных чисел в коллекторе
     * @return
     */
    TriangleNumber getMedian();
    /**
     * @brief add добавляет треугольное число в коллектор
     * @param val треугольное число
     */
    void add(TriangleNumber val);
    /**
     * @brief size возвращает размер коллектора
     * @return размер
     */
    int size();
    /**
     * @brief clear очищает коллектор
     */
    void clear();
    /**
     * @brief exists определяет, присутствует ли данное треугольное число в коллекторе
     * @param val число
     * @return true если присутствует, иначе false
     */
    bool exists(TriangleNumber val);
    /**
     * @brief count подсчитывает количество чисел в коллекторе
     * @param val число
     * @return количество вхождений числа в коллектор
     */
    int count(TriangleNumber val);
};

#endif
