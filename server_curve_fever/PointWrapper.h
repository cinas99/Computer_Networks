#ifndef SERVER_CURVE_FEVER_POINTWRAPPER_H
#define SERVER_CURVE_FEVER_POINTWRAPPER_H


#include "Point.h"

class PointWrapper {
private:
    int playerNumber;
    int pointNumber;
    Point point;
public:
    PointWrapper(int playerNumber, int pointNumber, Point point);
    int getPlayerNumber();
    int getPointNumber();
    Point getPoint();
};


#endif //SERVER_CURVE_FEVER_POINTWRAPPER_H
