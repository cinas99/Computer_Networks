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
#include <mutex>

class Board {
private:
    const long START_DRAWING_DELAY = 1500; // milliseconds
    const long MIN_TIME_OF_DRAWING = 4000; // milliseconds
    const long MAX_TIME_OF_DRAWING = 6000; // milliseconds
    const long MIN_TIME_OF_DELAY = 200; // milliseconds
    const long MAX_TIME_OF_DELAY = 400; // milliseconds
    const int KEYFRAME_DURATION_TIME = 26; // milliseconds
    static const int WIDTH = 900;
    static const int HEIGHT = 700;

    Board(Board const&);
    void operator=(Board const&);

    int numberOfPlayers;
    int currentNumberOfPlayers;
    std::vector <Player*> player;
    bool run;
    //mutex m;
    //string colors[4];

    //static const int NUMBER_OF_PLAYERS = 4;
    bool STILL_TIMING = FALSE;
    bool STILL_PLAYING = FALSE;
public:
    Board();
    static Board& getInstance() {
        static Board instance;
        return instance;
    }
    //void setServers(TcpServer tcpServer, UdpServer udpServer);
    void start(std::vector <Player*> player);//, TcpServer tcpServer, UdpServer udpServer);
    void initPlayers(int maxNumberOfPlayers);
    void threadDrawing();
    void threadGenerateLines();

    void showResults();
    static double random();
    void startDrawing();
    void stopDrawing();

    //static int getNumberOfPlayers();
    static bool outOfBounds(Point p);
    static bool areIntersecting(Point p1, Point p2, Point q1, Point q2);
    //void startDrawing();
    //void stopDrawing();
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