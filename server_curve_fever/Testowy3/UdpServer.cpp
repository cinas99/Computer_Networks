//
// Created by marcin on 15.02.18.
//

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
#include "UdpServer.h"
#include "Player.h"
#include <thread>

using namespace std;

#define PORT 9008
#define BUFSIZE 1000

UdpServer::UdpServer() {
    port = PORT;

    /* address structure */
    memset(&sockServer, 0, sizeof(struct sockaddr));
    sockServer.sin_family = AF_INET;
    sockServer.sin_addr.s_addr = htonl(INADDR_ANY);
    sockServer.sin_port = htons(port);

    /* create a socket */
    nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (nSocket < 0) {
        perror ("Nie można utworzyć gniazdka");
        exit (EXIT_FAILURE);
    }

    /* bind a name to a socket */
    if (bind(nSocket, (struct sockaddr*)&sockServer, sizeof(struct sockaddr)) < 0) {
        perror ("Błąd funkcji bind");
        exit (EXIT_FAILURE);
    }
    sockSize = sizeof(struct sockaddr);

    /*
   printf("Odebrano pakiet od %s:%d\n", inet_ntoa(sockClient.sin_addr), ntohs(sockClient.sin_port));
   printf("Długość wiadomości: %d, Treść: %s\n" , rcvLen, buf);
   for(int x =0 ; x < 11 ; x = x+1) {
       sprintf(buf, "Wspolrzedna: %d ,Wartosc: %d \n",x,x+1);
       sendto(nSocket, buf, BUF_SIZE, 0, (struct sockaddr *) &sockClient, sockSize);
   }
   */
}

void UdpServer::thread_give(int i) {
     //thread_table.push_back(thread(first_receive, i));
}

void UdpServer::first_receive(int i) {

}

string UdpServer::receive() {
    char buf[BUFSIZE];
    //sockaddr_in sockClient; // jest juz pole w klasie
    //socklen_t sockClientLen = sizeof(sockClient);

    sockClientLen = sizeof(sockClient);
    recvfrom(nSocket,buf,BUFSIZE,0,(struct sockaddr*)&sockClient, &sockClientLen);
    string s(buf);
    return s;
}

string UdpServer::send(Player player){
    const char *buf = player.getNick().c_str();
    sendto(nSocket, buf, BUFSIZE, 0, (struct sockaddr *) &sockClient, sockSize);
}

void UdpServer::turn_on(const char *confrim) {
    const char confirm2[8] = "turn_on";
    vector<Player> players; // wektor aktualnych playersów
        if(strcmp(confrim,confirm2))
        {
            if(numberOfPlayers > 0){
                for(int z=0; z<numberOfPlayers; z++){
                    send(players[z]);
                }}
            else{
                // wyslij info o braku graczy
            }
        }

    //players_counter = player.checkPlayersNumber();
    //sprintf(buf, "%d", players_counter);
    //sendto(nSocket, buf, BUF_SIZE, 0, (struct sockaddr *) &sockClient, sockSize); //wysylam liczbe graczy

}

void UdpServer::join() {
    //send handshake
    //receive nick
    //send handshake dobry nick
}

void UdpServer::ready() {
    //send handshake
}

void UdpServer::start() {//send 4
    //receive all handshakes
    //while(gra) {...}}
}
