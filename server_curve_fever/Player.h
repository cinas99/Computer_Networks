#ifndef SERVER_CURVE_FEVER_PLAYER_H
#define SERVER_CURVE_FEVER_PLAYER_H

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include "Point.h"
#include <vector>
#include <string>
using namespace std;

class Player {
private:
    int tcpSocket;
    sockaddr_in clientSockAddr;
    std::string nick;
    bool inRoom;
    bool ready;
    double LINEAR_SPEED = 2.8;
    double CIRCULAR_SPEED = 0.12;
    vector<Point> visited;
    Point cur_point = setCurPoint(currentX, currentY);
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

    Point setCurPoint(double x, double y);

    vector<Point> getVisited();

    vector<double> getColor();

    bool isNowPlaying();

    void setNowPlaying(bool nowPlaying);

    Player(int tcpSocket, sockaddr_in clientSockAddr);

    void setNick(std::string nick);

    std::string getNick();

    int getTcpSocket();

    sockaddr_in getSockAddr();

    void setInRoom(bool inRoom);

    bool isInRoom();

    void setReady(bool ready);

    bool isReady();

};
#endif