#include <stdexcept>
#include <cmath>
#include "include/point.hpp"

Point::Point(double x_coord, double y_coord, double z_coord): x(x_coord), y(y_coord), z(z_coord) {}
Point::Point() : x(0.0), y(0.0), z(1.0) {}

double Point::getX() const {
    return this->x;
}

double Point::getY() const {
    return this->y;
}

double Point::getZ() const {
    return this->z;
}

double Point::get(int i) const {
    switch (i) {
        case 0:
            return this->getX();
        case 1:
            return this->getY();
        case 2:
            return this->getZ();
        default:
            throw std::out_of_range("Index inválido em Point::get(), índices válidos são {1, 2, 3}");
    }
}

void Point::setX(double n_x) {
    this->x = n_x;
}

void Point::setY(double n_y) {
    this->y = n_y;
}

void Point::setZ(double n_z) {
    this->z = n_z;
}

void Point::set(int i, double value) {
    switch (i) {
        case 1:
            this->setX(value);
            break;
        case 2:
            this->setY(value);
            break;
        case 3:
            this->setZ(value);
            break;
        default:
            throw std::out_of_range("Index inválido em Point::set(), índices válidos são {1, 2, 3}");
    }
}

void Point::set(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Point::normalize() {
    double norm = sqrt(x * x + y * y + z * z);
    x /= norm;
    y /= norm;
    y /= norm; 
}
