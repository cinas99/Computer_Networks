#ifndef SERVER_CURVE_FEVER_BOARD_H
#define SERVER_CURVE_FEVER_BOARD_H

#include <iostream>
#include <vector>
#include "Point.h"
#include "Player.h"
#include "Message.h"
#include "TcpServer.h"
#include "UdpServer.h"
#include <string.h>

class Board {
private:
    Board(Board const&);
    void operator=(Board const&);
    TcpServer tcpServer;
    UdpServer udpServer;
    int numberOfPlayers;
    //string colors[4];

    std::vector <Player> player;
    long double KEYFRAME_DURATION_TIME = 0.026; // seconds
    long START_DRAWING_DELAY = 1500; // milliseconds
    static const long MIN_TIME_OF_DRAWING = 4000; // milliseconds
    static const long MAX_TIME_OF_DRAWING = 6000; // milliseconds
    static const long MIN_TIME_OF_DELAY = 200; // milliseconds
    static const long MAX_TIME_OF_DELAY = 400; // milliseconds
    static const int NUMBER_OF_PLAYERS = 4;
    static const int WIDTH = 900;
    static const int HEIGHT = 700;
    bool STILL_TIMING = FALSE;
    bool STILL_PLAYING = FALSE;
public:
    Board();
    static Board& getInstance() {
        static Board instance;
        return instance;
    }
    //void setServers(TcpServer tcpServer, UdpServer udpServer);
    void start(int numberOfPlayers, TcpServer tcpServer, UdpServer udpServer);
    void initPlayers(int maxNumberOfPlayers);

    static int getNumberOfPlayers();
    static bool outOfBounds(Point p);
    static bool areIntersecting(Point p1, Point p2, Point q1, Point q2);
    void startDrawing();
    void stopDrawing();
    void checkCollision();
    void TimerTask(int interval, bool executor);

    bool checkStillTiming();
    bool checkStillPlaying();
    void setStillTiming(bool cond);
    void setStillPlaying(bool cond);
    //void showResults(); // TODO LATER
    Board(int maxNumberOfPlayers);
};

#endif //SERVER_CURVE_FEVER_BOARD_H