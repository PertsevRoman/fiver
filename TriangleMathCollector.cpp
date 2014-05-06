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
	throw "Not yet implemented";
}

void TriangleMathCollector::add(TriangleNumber val) {
    triangles.push_back(val);
}

void TriangleMathCollector::remove(TriangleNumber val) {
    triangles.remove(val);
}

//void TriangleMathCollector::remove(int index) {
//    triangles.remove(triangles[index]);
//}

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

