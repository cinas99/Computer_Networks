 //
// Created by marcin on 13.02.18.
//

#ifndef SERVERUDPSKIBA_PLAYER_H
#define SERVERUDPSKIBA_PLAYER_H

#include "Point.h"
#include <vector>
#include <string>

 using namespace std;

class Player {
private:
    double LINEAR_SPEED = 2.8;
    double CIRCULAR_SPEED = 0.12;
    vector<Point> visited;
    Point cur_point = setCurPoint(currentX,currentY);
    string color; // zamiast wektora moze tablica ??
    double currentX;
    double currentY;
    double angle;
    int turn;
    bool draw;
    bool nowPlaying;
public:
    Player();
    Player(double startX, double startY, double angle);
    Player(double startX, double startY, double angle, std::string color);
    void generateNextLine();
    void markGap();
    double getCurrentX();
    double getCurrentY();
    void setTurn(int turn);
    void setDraw(bool draw);
    Point setCurPoint(double x,double y);
    vector<Point> getVisited();
    vector<double>getColor();
    bool isNowPlaying();
    void setNowPlaying(bool nowPlaying);
};

#endif //SERVERUDPSKIBA_PLAYER_H
