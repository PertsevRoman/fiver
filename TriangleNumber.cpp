#include "TriangleNumber.h"

QMap<int, valsBind> TriangleNumber::baseBinds;

TriangleNumber::TriangleNumber(double c, double l, double r) :
    center(c), left(l), right(r) {

    integrationCount = 1000;
    w = gsl_integration_workspace_alloc(integrationCount);
}

TriangleNumber::~TriangleNumber() {
    gsl_integration_workspace_free(w);
}

double TriangleNumber::getCenter() {
    return this->center;
}

void TriangleNumber::setCenter(double center) {
    this->center = center;
}

double TriangleNumber::getRight() {
    return this->right;
}

void TriangleNumber::setRight(double right) {
    this->right = right;
}

double TriangleNumber::getLeft() {
    return this->left;
}

void TriangleNumber::setLeft(double left) {
    this->left = left;
}

TriangleNumber TriangleNumber::operator+(TriangleNumber &r) {
    return TriangleNumber(this->center + r.center, this->left + r.left, this->right + r.right);
}

TriangleNumber& TriangleNumber::operator+=(TriangleNumber &r) {
    this->center += r.center;
    this->left += r.left;
    this->right += r.right;

    return *this;
}

bool TriangleNumber::operator==(const TriangleNumber &r) {
    return this->center == r.center && this->left == r.left && this->right == r.right;
}

bool TriangleNumber::operator <(const TriangleNumber &r) {
    return this->getCriteria() < r.getCriteria();
}

bool TriangleNumber::operator >(const TriangleNumber &r) {
    return this->getCriteria() < r.getCriteria();
}

/**
 * @brief getCriteriaFuncValue Вычисляет значение критериальной функции
 * @param x значение x
 * @param data данные (не используются)
 * @return значение критериальной функции
 */
double getCriteriaFuncValue(double x, void * data) {
    double* dData = (double*) data;

    double center   = dData[0];
    double left     = dData[1];
    double right    = dData[2];

    return x * ((left * x + center - left) + (center + right - right * x));
}

double TriangleNumber::getCriteria() const {
    double result, error;

    double data[3];
    data[0] = center;
    data[1] = left;
    data[2] = right;

    gsl_function mag;
    mag.function = &getCriteriaFuncValue;
    mag.params = &data[0];

    gsl_integration_qags(&mag, 0, 1, 0, 1e-7, integrationCount, w, &result, &error);

    return result;
}

void TriangleNumber::addBaseBind(int key, TriangleNumber num, std::string strID) {
    if(!baseBinds.contains(key)) {
        baseBinds[key] = boost::make_tuple(num, strID);
    }
}

TriangleNumber TriangleNumber::getBindedTriangle(int key) {
    TriangleNumber res;

    if(baseBinds.contains(key)) {
        res = boost::get<0>(baseBinds[key]);
    }

    return res;
}

std::string TriangleNumber::getBindedStrID(int key) {
    std::string res;

    if(baseBinds.contains(key)) {
        res = boost::get<1>(baseBinds[key]);
    }

    return res;
}

