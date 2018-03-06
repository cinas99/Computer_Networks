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

void UdpServer::receive(std::vector <Player *> player) {
    char buf[BUF_SIZE];
    sockaddr_in clientSockAddr;// = player->getSockAddr();
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);
    recvfrom(nSocket, buf, BUF_SIZE, 0, (struct sockaddr*)&clientSockAddr, &clientSockAddrLen);
    //m.lock();
    for (std::vector<Player*>::iterator it = player.begin(); it != player.end(); ++it) {
        if ((*it)->getSockAddr().sin_addr.s_addr == clientSockAddr.sin_addr.s_addr) {
            std::string s(buf);
            (*it)->getUdpReceiveQueue()->push(s);
            std::cout << s << " " << clientSockAddr.sin_addr.s_addr << std::endl;
            (*it)->setSockAddr(clientSockAddr);
            if (!(*it)->isUdpSet()) {
                //m.lock();
                (*it)->setSockAddr(clientSockAddr);
                (*it)->setUdpSet(true);
                //m.unlock();
            }
            //int playerTcpSocket = (*it)->getTcpSocket();
            //tcpServer.sendInt(playerTcpSocket, START);
            //tcpServer.sendInt(playerTcpSocket, gamePlayers.size());
        }
    }
    //m.unlock();
    //std::string s(buf);
    //return s;
}

/*int UdpServer::receiveInt(Player *player) {
    int32_t result;
    sockaddr_in clientSockAddr = player->getSockAddr();
    socklen_t clientSockAddrLen = sizeof(clientSockAddr);
    recvfrom(nSocket, &result, sizeof(int), 0, (struct sockaddr*)&clientSockAddr, &clientSockAddrLen);
    m.lock();
    player->setSockAddr(clientSockAddr);
    m.unlock();
    return ntohl(result);
}*/

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