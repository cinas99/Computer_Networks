#ifndef SERVER_CURVE_FEVER_UDP_SERVER_H
#define SERVER_CURVE_FEVER_UDP_SERVER_H

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
#include "Message.h"
#include "Player.h"

class UdpServer {
private:
    sockaddr_in sockServer;
    int nSocket;
    //std::mutex m;

public:
    UdpServer();
    int init();
    void receive(std::vector <Player *> player);
    //int receiveInt(Player *player);
    void send(Player *player, std::string msg);
    void closeSocket();
};


#endif //SERVER_CURVE_FEVER_UDP_SERVER_H
