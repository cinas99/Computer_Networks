#ifndef SERVER_CURVE_FEVER_BOARD_H
#define SERVER_CURVE_FEVER_BOARD_H

#include <iostream>
#include <vector>
#include "Point.h"
#include "Player.h"
#include "Message.h"
#include "TcpServer.h"
#include "UdpServer.h"
#include "PointWrapper.h"
#include <string.h>
#include <mutex>

class Board {
private:
    const long START_DRAWING_DELAY = 1500; // milliseconds
    const long MIN_TIME_OF_DRAWING = 2000; // milliseconds
    const long MAX_TIME_OF_DRAWING = 4000; // milliseconds
    const long MIN_TIME_OF_DELAY = 200; // milliseconds
    const long MAX_TIME_OF_DELAY = 400; // milliseconds
    const int KEYFRAME_DURATION_TIME = 26; // milliseconds
    static const int WIDTH = 900;
    static const int HEIGHT = 700;

    Board();
    Board(Board const&);
    void operator=(Board const&);

    int numberOfPlayers;
    int currentNumberOfPlayers;
    std::vector <Player*> player;
    bool run;
    std::string winner;
public:
    static Board& getInstance() {
        static Board instance;
        return instance;
    }
    void start(std::vector <Player*> player);
    void initPlayers(int maxNumberOfPlayers);
    void threadDrawing();
    void threadGenerateLines();
    void sendPoints(std::vector <PointWrapper> newPoints);
    static std::string prepareMessage(int playerNumber, int pointNumber, Point point);
    std::string getWinner();

    void sendResults();
    static double random();
    void startDrawing();
    void stopDrawing();

    //static int getNumberOfPlayers();
    static bool outOfBounds(Point p);
    static bool areIntersecting(Point p1, Point p2, Point q1, Point q2);
    void checkCollision();
};

#endif //SERVER_CURVE_FEVER_BOARD_H