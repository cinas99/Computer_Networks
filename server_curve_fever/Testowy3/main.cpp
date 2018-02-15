#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <iostream>
#include <thread>
#include "PrepareGame.h"
#include "Player.h"
#include "UdpServer.h"

#define BUF_SIZE 1000
#define PORT 9020


int main(int argc, char* argv[])
{

    std::string str = "string";
    const char *cstr = str.c_str();

    string take_receive;
    UdpServer go;
    take_receive = go.receive();
    go.turn_on(take_receive); // ogar porzadek miedzy string a const char *

    return(0)
}