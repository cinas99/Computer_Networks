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
#include "Board.h"

#define MAX_ROOM_SEATS 4

class GameServer {
private:
    TcpServer tcpServer;
    UdpServer udpServer;
    std::vector <Player*> connectedPlayers; // parallel access
    std::vector <Player*> gamePlayers; // parallel access
    bool isGameStarted; // parallel access
    std::mutex mConnected;
    std::mutex mGamePlayers;
    std::mutex mGameStarted;

    void roomEventSend(int tcpSocket, int playersInRoom);
    bool isEveryoneReady();
    bool isFreeSeat();
    int countRoomPlayers();
    std::vector <Player*> getGamePlayers();
    void deletePlayer(Player *player);

public:
    GameServer();
    void tcpReceive(int tcpSocket, sockaddr_in clientSockAddr);
    void tcpSend(Player *player);
    void udpAction(Player *player);
    void udpReceive();
    void udpSend(Player *player);
    void run();
};

#endif //SERVER_CURVE_FEVER_GAME_SERVER_H
