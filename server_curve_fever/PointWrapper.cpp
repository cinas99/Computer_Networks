#include "PointWrapper.h"

PointWrapper::PointWrapper(int playerNumber, int pointNumber, Point point) : point(point) {
    this->playerNumber = playerNumber;
    this->pointNumber = pointNumber;
    this->point = point;
}

int PointWrapper::getPlayerNumber() {
    return playerNumber;
}

int PointWrapper::getPointNumber() {
    return pointNumber;
}

Point PointWrapper::getPoint() {
    return point;
}