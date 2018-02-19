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

class Player {
private:
    sockaddr_in sockAddrClient;
    int clientSocket;
    std::string nick;
    bool inRoom;
    bool ready;

public:
    Player(int clientSocket, sockaddr_in sockAddrClient);
    void setNick(std::string nick);
    std::string getNick();
    int getClientSocket();
    void setInRoom(bool inRoom);
    bool isInRoom();
    void setReady(bool ready);
    bool isReady();
};

#endif