//
// Created by marcin on 15.02.18.
//

#ifndef TESTOWY3_UDPSERVER_H
#define TESTOWY3_UDPSERVER_H
#include <string.h>
#include <vector>
#include "Player.h"

using namespace std;

class UdpServer {

private:
    sockaddr_in sockServer;
    int nSocket;
    int port;
    int sockSize;
    int numberOfPlayers;
    //vector <thread> thread_table;
    sockaddr_in sockClient;
    socklen_t sockClientLen;

public:
    UdpServer();
    void thread_give(int i);
    void first_receive(int i);
    string receive();
    string send(Player player);
    void turn_on(const char *confrim);
    void join();
    void ready();
    void start();


    ~UdpServer();

};


#endif //TESTOWY3_UDPSERVER_H
