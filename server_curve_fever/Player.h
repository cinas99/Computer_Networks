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
#include <thread>
#include <mutex>
//using namespace std;

class Player {
private:
    const double LINEAR_SPEED = 1.6;
    const double CIRCULAR_SPEED = 0.07;

    int tcpSocket; // parallel access
    sockaddr_in clientSockAddr; // parallel access
    SafeQueue <Message> *tcpQueue; // parallel access
    SafeQueue <std::string> *udpSendQueue; // parallel access
    SafeQueue <std::string> *udpReceiveQueue; // parallel access

    std::mutex mClientSockAddr;
    std::mutex mUdpSet;
    std::mutex mInRoom;
    std::mutex mReady;
    std::mutex mDraw;
    std::mutex mNowPlaying;
    std::mutex mThreadAlive;
    std::mutex mTurn;

    std::string nick; // parallel access
    std::vector<Point> visited; // parallel access
    bool udpSet; // parallel access
    bool inRoom; // parallel access
    bool ready; // parallel access
    bool draw; // parallel access
    bool nowPlaying; // parallel access
    bool threadAlive; // parallel access

    double currentX; // parallel access
    double currentY; // parallel access
    double angle; // parallel access
    int turn; // parallel access
    //mutex m;

public:
    Player(int tcpSocket, sockaddr_in clientSockAddr, SafeQueue <Message> *tcpQueue);
    Point init(double startX, double startY, double angle);

    sockaddr_in getSockAddr();
    void setSockAddr(sockaddr_in clientSockAddr);
    SafeQueue <Message> *getTcpQueue();
    SafeQueue <std::string> *getUdpSendQueue();
    void setUdpSendQueue(SafeQueue<std::string> *udpQueue);
    SafeQueue <std::string> *getUdpReceiveQueue();
    void setUdpReceiveQueue(SafeQueue<std::string> *udpQueue);

    Point generateNextLine();
    std::vector<Point> getVisited();
    int getVisitedSize();
    void markGap();

    void setTurn(int turn);
    void setDraw(bool draw);
    void setUdpSet(bool set);
    bool isUdpSet();
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