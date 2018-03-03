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
#include "SafeQueue.h"
#include "Message.h"
#include <vector>
#include <string>
using namespace std;

class Player {
private:
    const double LINEAR_SPEED = 2.8;
    const double CIRCULAR_SPEED = 0.12;

    int tcpSocket;
    sockaddr_in clientSockAddr;
    SafeQueue <Message> *tcpQueue;
    SafeQueue <string> *udpQueue;

    std::string nick;
    vector<Point> visited;
    bool inRoom;
    bool ready;
    bool draw;
    bool nowPlaying;
    bool threadAlive;

    double currentX;
    double currentY;
    double angle;
    int turn;

public:
    Player(int tcpSocket, sockaddr_in clientSockAddr, SafeQueue <Message> *tcpQueue);
    Point init(double startX, double startY, double angle);

    sockaddr_in getSockAddr();
    void setSockAddr(sockaddr_in clientSockAddr);
    SafeQueue <Message> *getTcpQueue();
    SafeQueue <string> *getUdpQueue();
    void setUdpQueue(SafeQueue <string> *udpQueue);

    Point generateNextLine();
    vector<Point> getVisited();
    int getVisitedSize();
    void markGap();

    Point getPoint(int index);
    void setTurn(int turn);
    void setDraw(bool draw);
    bool isThreadAlive();
    void setThreadAlive(bool threadAlive);
    bool isNowPlaying();
    void setNowPlaying(bool nowPlaying);
    void setNick(std::string nick);
    std::string getNick();
    int getTcpSocket();
    void setInRoom(bool inRoom);
    bool isInRoom();
    void setReady(bool ready);
    bool isReady();

};
#endif