#include "Point.h"

Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
    this->gap = false;
}

Point::Point(double x, double y, bool gap) {
    this->x = x;
    this->y = y;
    this->gap = gap;
}

void Point::setGap(bool gap) {
    this->gap = gap;
}

bool Point::isGap() {
    return gap;
}

double Point::getX() {
    return x;
}

double Point::getY() {
    return y;
}