//
// Created by marcin on 13.02.18.
//

#include "Point.h"

//// ALL THE SAME /////

Point::Point(double x, double y) {
        this->x = x;
        this->y = y;
    }

Point::Point(double x, double y, bool gap) {
        this->x = x;
        this->y = y;
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
