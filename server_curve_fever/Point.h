#ifndef SERVER_CURVE_FEVER_POIN_H
#define SERVER_CURVE_FEVER_POIN_H

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



#endif //SERVER_CURVE_FEVER_POIN_H
