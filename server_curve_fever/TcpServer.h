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
#include "Player.h"
#include "Message.h"

class TcpServer {
private:
    sockaddr_in sockServer;
    int nSocket;

public:
    TcpServer();
    void init();
    int clientAccept(sockaddr_in *clientSockAddr);

    std::string receive(int tcpSocket);
    int receiveInt(int tcpSocket);
    void send(int tcpSocket, const char* msg);
    void sendString(int tcpSocket, std::string msg);
    void sendInt(int tcpSocket, int num);
    void closeSocket();

};

#endif //SERVER_CURVE_FEVER_TCP_SERVER_H
