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

using namespace std;

class GameServer {
private:
    TcpServer tcpServer;
    UdpServer udpServer;
    vector <Player*> connectedPlayers;
    vector <Player*> gamePlayers;
    bool isGameStarted;
    mutex mPlayers;
    mutex mBoard;

public:
    GameServer();
    void tcpReceive(int tcpSocket, sockaddr_in clientSockAddr);
    void tcpSend(Player *player);
    void udpReceive(Player *player);
    void udpSend(Player *player);
    void run();

    void roomEventSend(int tcpSocket, int playersInRoom);
    bool isEveryoneReady();
    bool isFreeSeat();
    int countRoomPlayers();
    std::vector <Player*> getGamePlayers();
    void deletePlayer(Player player);
};

#endif //SERVER_CURVE_FEVER_GAME_SERVER_H
