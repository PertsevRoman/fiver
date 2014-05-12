#include "TriangleNumber.h"

QMap<int, valsBind> TriangleNumber::baseBinds;

TriangleNumber::TriangleNumber(double c, double l, double r)
{
    this->center = c;
    this->left = l;
    this->right = r;
}

TriangleNumber::TriangleNumber(const TriangleNumber &c) {
    this->center = c.center;
    this->left = c.left;
    this->right = c.right;
}

TriangleNumber::~TriangleNumber() {
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

TriangleNumber &TriangleNumber::operator =(const TriangleNumber &r) {
    if(this != &r) {
        this->left = r.left;
        this->right = r.right;
        this->center = r.center;
    }

    return *this;
}

TriangleNumber TriangleNumber::operator+(TriangleNumber &r) {
    return TriangleNumber(this->center + r.center, this->left + r.left, this->right + r.right);
}

TriangleNumber TriangleNumber::operator -(TriangleNumber &r) {
    return TriangleNumber(this->center - r.center, this->left - r.left, this->right - r.right);
}

TriangleNumber& TriangleNumber::operator+=(TriangleNumber &r) {
    this->center += r.center;
    this->left += r.left;
    this->right += r.right;

    return *this;
}

bool TriangleNumber::operator==(const TriangleNumber &r) const {
    return this->center == r.center && this->left == r.left && this->right == r.right;
}

bool TriangleNumber::operator <(const TriangleNumber &r) const {
    return this->getCriteria() < r.getCriteria();
}

bool TriangleNumber::operator >(const TriangleNumber &r) const {
    return this->getCriteria() < r.getCriteria();
}

/**
 * @brief getCriteriaFuncValue Вычисляет значение критериальной функции
 * @param x значение x
 * @param data данные (не используются)
 * @return значение критериальной функции
 */
double TriangleNumber::getCriteriaFuncValue(double x) {
    return x * ((left * x + center - left) + (center + right - right * x));
}

double TriangleNumber::getCriteria() const {
    o2scl::funct_mfptr<TriangleNumber> functor(const_cast<TriangleNumber*>(this), &TriangleNumber::getCriteriaFuncValue);

    o2scl::inte_qag_gsl<> g;

    double res = g.integ(functor, 0.0, 1.0);

    return res;
}

void TriangleNumber::addBaseBind(int key, TriangleNumber num, std::string strID) {
    if(!baseBinds.contains(key)) {
        baseBinds[key] = boost::make_tuple(num, strID);
    }
}

TriangleNumber TriangleNumber::getBindedTriangle(int key) {
    TriangleNumber res;

    qDebug() << "Индекс: " << key;

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

std::string TriangleNumber::getBindedStrID(const TriangleNumber &num) {
    QList<int> keyVals = baseBinds.keys();

    foreach (int val, keyVals) {
        if(boost::get<0>(baseBinds[val]) == num) {
            return boost::get<1>(baseBinds[val]);
        }
    }

    return "";
}
