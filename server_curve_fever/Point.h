#ifndef SERVER_CURVE_FEVER_POINT_H
#define SERVER_CURVE_FEVER_POINT_H

class Point {
private:
    bool gap; // parallel access
    double x;
    double y;
public:
    Point(double x, double y);
    Point(double x, double y, bool gap);
    void setGap(bool gap);
    bool isGap();
    double getX();
    double getY();
};



#endif //SERVER_CURVE_FEVER_POINT_H
