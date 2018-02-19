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
#include "tcp_server.h"
#include "player.h"
#include "safe_queue.h"

using namespace std;

class GameServer {
private:
    TcpServer tcpServer;
    vector <Player*> connectedPlayers; //parallel access
    mutex m;

public:
    GameServer();
    void clientReceive(int clientSocket, sockaddr_in sockAddrClient);
    void clientSend(Player *player, SafeQueue <Message> *queue);
    void run();
};

#endif //SERVER_CURVE_FEVER_GAME_SERVER_H
