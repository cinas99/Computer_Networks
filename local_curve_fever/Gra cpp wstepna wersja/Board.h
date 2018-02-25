//
// Created by marcin on 13.02.18.
//
#ifndef SERVERUDPSKIBA_BOARD_H
#define SERVERUDPSKIBA_BOARD_H

#include <iostream>
#include <vector>
#include "Point.h"
#include "Player.h"
#include <Time.h>
#include <fci.h>

class Board {
private:
    static Player player[4];
    static string colors[4];
    long double KEYFRAME_DURATION_TIME = 0.026; // seconds
    long START_DRAWING_DELAY = 1500; // milliseconds
    static const long MIN_TIME_OF_DRAWING = 4000; // milliseconds
    static const long MAX_TIME_OF_DRAWING = 6000; // milliseconds
    static const long MIN_TIME_OF_DELAY = 200; // milliseconds
    static const long MAX_TIME_OF_DELAY = 400; // milliseconds
    static const int NUMBER_OF_PLAYERS = 4;
    int currentNumberOfPlayers;
    static const int WIDTH = 900;
    static const int HEIGHT = 700;
    bool STILL_TIMING = FALSE;
    bool STILL_PLAYING = FALSE;
    //const Timer timer;
    //const Timeline timeline;

public:
    Board();
    Board(int maxNumberOfPlayers);
    static int getNumberOfPlayers();
    static bool outOfBounds(Point p);
    static bool areIntersecting(Point p1, Point p2, Point q1, Point q2);
    void startDrawing();
    void stopDrawing();
    void initPlayers(int maxNumberOfPlayers);
    void checkCollision();
    void start();
    void TimerTask(int interval, bool executor);

    bool checkStillTiming();
    bool checkStillPlaying();
    void setStillTiming(bool cond);
    void setStillPlaying(bool cond);
    //void showResults(); // TODO LATER

};
#endif //SERVERUDP SKIBA_BOARD_H
