#ifndef SERVER_CURVE_FEVER_GAME_SERVER_H
#define SERVER_CURVE_FEVER_GAME_SERVER_H

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include <ctime>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include "TcpServer.h"
#include "Player.h"
#include "SafeQueue.h"
#include "UdpServer.h"

#define MAX_ROOM_SEATS 4

using namespace std;

class GameServer {
private:
    TcpServer tcpServer;
    UdpServer udpServer;
    vector <Player*> connectedPlayers; //parallel access
    bool isGameStarted;
    mutex m;

public:
    GameServer();
    void tcpReceive(int tcpSocket, sockaddr_in clientSockAddr);
    void tcpSend(Player *player, SafeQueue<Message> *queue);
    void udpReceive(Player *player);
    void run();

    bool isEveryoneReady();
    bool isFreeSeat();
    int countRoomPlayers();
};

#endif //SERVER_CURVE_FEVER_GAME_SERVER_H
