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
    std::sort(triangles.begin(), triangles.end());

    TriangleNumber res;
    if(triangles.size() / 2) {
        res = triangles.at(triangles.size() / 2);
    } else {
        TriangleNumber first = triangles.at(triangles.size() / 2 - 1);
        TriangleNumber second = triangles.at(triangles.size() / 2);

        if(first == second) {
            res = second;
        } else {
            res = first + second;
            res.setCenter(res.getCenter() / 2);
            res.setLeft(res.getLeft() / 2);
            res.setRight(res.getRight() / 2);
        }
    }

    return res;
}

void TriangleMathCollector::add(TriangleNumber val) {
    triangles.push_back(val);
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
	throw "Not yet implemented";
}

