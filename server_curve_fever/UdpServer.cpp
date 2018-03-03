#include "UdpServer.h"

UdpServer::UdpServer() {
}

int UdpServer::init() {
    int nFoo = 1;

    /* address structure */
    memset(&sockServer, 0, sizeof(struct sockaddr));
    sockServer.sin_family = AF_INET;
    sockServer.sin_addr.s_addr = htonl(INADDR_ANY);
    sockServer.sin_port = htons(UDP_PORT);

    /* create a socket */
    nSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (nSocket < 0) {
        perror ("Nie można utworzyć gniazdka");
        exit (EXIT_FAILURE);
    }

    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    /* bind a name to a socket */
    if (bind(nSocket, (struct sockaddr*)&sockServer, sizeof(struct sockaddr)) < 0) {
        perror ("Błąd funkcji bind");
        exit (EXIT_FAILURE);
    }
    return nSocket;
}

std::string UdpServer::receive(Player *player) {
    char buf[BUF_SIZE];
    sockaddr_in clientSockAddr = player->getSockAddr();
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);
    recvfrom(nSocket, buf, BUF_SIZE, 0, (struct sockaddr*)&clientSockAddr, &clientSockAddrLen);
    std::string s(buf);
    return s;
}

int UdpServer::receiveInt(Player *player) {
    int32_t result;
    sockaddr_in clientSockAddr = player->getSockAddr();
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);
    recvfrom(nSocket, &result, sizeof(int), 0, (struct sockaddr*)&clientSockAddr, &clientSockAddrLen);
    player->setSockAddr(clientSockAddr);
    return ntohl(result);
}

void UdpServer::send(Player *player, std::string msg) {
    char buf[BUF_SIZE];
    sockaddr_in clientSockAddr = player->getSockAddr();
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);
    msg += "\0";
    int msgLen = sprintf(buf, "%s", msg.c_str());
    sendto(nSocket, buf, msgLen, 0, (struct sockaddr *)&clientSockAddr, clientSockAddrLen);
}

void UdpServer::closeSocket() {
    close(nSocket);
}