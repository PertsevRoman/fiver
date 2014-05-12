#include <string>
#include <vector>
#include <exception>
using namespace std;

#include "TriangleMathCollector.h"
#include "TriangleNumber.h"

TriangleNumber TriangleMathCollector::getSum() {
    TriangleNumber res;

    foreach (TriangleNumber num, triangles) {
        res += num;
    }

    return res;
}

TriangleNumber TriangleMathCollector::getMedian() {
    qSort(triangles.begin(), triangles.end());
    TriangleNumber res;

    int halfSize = triangles.size() / 2;

    if(triangles.size() % 2) {
        res = triangles.at(halfSize);
    } else {
        TriangleNumber first = triangles.at(halfSize - 1);
        TriangleNumber second = triangles.at(halfSize);

        if(first == second) {
            res = second;
        } else {
            if(triangles.size() > 2) {
                TriangleNumber firstSum;
                TriangleNumber secondSum;

                for(int i = 0; i < halfSize - 1; i++) {
                    firstSum += triangles[i];
                }

                for(int i = halfSize + 1; i < triangles.size(); i++) {
                    secondSum += triangles[i];
                }

                firstSum = firstSum - first;
                secondSum = secondSum - second;

                if(firstSum < secondSum) {
                    res = second;
                } else if(firstSum > secondSum) {
                    res = first;
                } else {
                    res = first;
                }
            } else {
                res = second;
            }
        }
    }

    return res;
}

void TriangleMathCollector::add(TriangleNumber val) {
    qDebug() << "Добавление значения";
    triangles.append(val);
}

int TriangleMathCollector::size() {
    return triangles.size();
}

void TriangleMathCollector::clear() {
    triangles.clear();
}

bool TriangleMathCollector::exists(TriangleNumber val) {
    return true;
}

int TriangleMathCollector::count(TriangleNumber val) {
    return triangles.size();
}

