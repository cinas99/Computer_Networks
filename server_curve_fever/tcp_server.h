#ifndef SERVER_CURVE_FEVER_TCP_SERVER_H
#define SERVER_CURVE_FEVER_TCP_SERVER_H

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
#include <string>
#include <arpa/inet.h>
#include <vector>
#include <iostream>
#include "player.h"

#define QUEUE_SIZE 20
#define BUF_SIZE 1024
#define SERVER_PORT 9022

enum Message{
    UNJOIN,
    TURN_ON,
    JOIN,
    LEAVE,
    READY,
    UNREADY,
    START,
    ROOM_EVENT,
    TRUE,
    FALSE
};

class TcpServer {
private:
    sockaddr_in sockServer;
    sockaddr_in sockClient;
    int nSocket;
    int nClientSocket;

public:
    TcpServer();
    void init();
    int getClientSocket();
    sockaddr_in getClientSockAddr();
    void clientAccept();

    void turnOnSend(int clientSocket, std::vector<Player*> connectedPlayers);
    std::string joinReceive(int clientSocket);
    bool joinSend(int clientSocket, std::vector<Player*> connectedPlayers);
    int countRoomPlayers(std::vector<Player*> connectedPlayers);
    void ready();
    void roomEventSend(int clientSocket, std::vector<Player *> connectedPlayers);

    std::string receive(int clientSocket);
    int receiveInt(int clientSocket);
    void send(int clientSocket, const char* msg);
    void sendString(int clientSocket, std::string msg);
    void sendInt(int clientSocket, int num);
    void closeSocket();
    //~TcpServer();

};

#endif //SERVER_CURVE_FEVER_TCP_SERVER_H
