//
// Created by marcin on 13.02.18.
//

#ifndef SERVERUDPSKIBA_POINT_H
#define SERVERUDPSKIBA_POINT_H


class Point {
private:
    bool gap;
    double x;
    double y;
public:
    Point(double x, double y);
    Point(double x, double y, bool gap);
    bool isGap();
    double getX();
    double getY();
};

#endif //SERVERUDPSKIBA_POINT_H
