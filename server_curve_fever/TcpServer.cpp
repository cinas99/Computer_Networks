#include "TcpServer.h"

TcpServer::TcpServer() {
}

void TcpServer::init() {
    int nFoo = 1;

    /* address structure */
    memset(&sockServer, 0, sizeof(struct sockaddr));
    sockServer.sin_family = AF_INET;
    sockServer.sin_addr.s_addr = htonl(INADDR_ANY);
    sockServer.sin_port = htons(TCP_PORT);

    /* create a socket */
    nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nSocket < 0)
    {
        perror ("Nie można utworzyć gniazdka");
        exit(1);
    }
    setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nFoo, sizeof(nFoo));

    /* bind a name to a socket */
    int nBind = bind(nSocket, (struct sockaddr*)&sockServer, sizeof(struct sockaddr));
    if (nBind < 0)
    {
        perror ("Błąd funkcji bind");
        exit(1);
    }
    /* specify queue size */
    int nListen = listen(nSocket, QUEUE_SIZE);
    if (nListen < 0)
    {
        perror ("Nie można ustawić wielkości kolejki");
    }
}

int TcpServer::clientAccept(sockaddr_in *clientSockAddr) {
    socklen_t sockLen = sizeof(struct sockaddr);
    int nClientSocket = accept(nSocket, (struct sockaddr*)clientSockAddr, &sockLen);
    if (nClientSocket < 0)
    {
        perror ("Nie można utworzyć gniazdka przy połączeniu z klientem");
        exit(1);
    }
    printf("TCP connection to %s\n",
           inet_ntoa((struct in_addr)clientSockAddr->sin_addr));
    return nClientSocket;
}

void TcpServer::turnOnSend(int tcpSocket) {
    sendInt(tcpSocket, TURN_ON);
}

void TcpServer::roomEventSend(int tcpSocket, int playersInRoom, std::vector<Player*> connectedPlayers) {
    sendInt(tcpSocket, ROOM_EVENT);
    sendInt(tcpSocket, playersInRoom);
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if ((*it)->isInRoom()) {
            sendString(tcpSocket, (*it)->getNick());
            if ((*it)->isReady())
                sendInt(tcpSocket, TRUE);
            else
                sendInt(tcpSocket, FALSE);
        }
    }
}

std::string TcpServer::joinReceive(int tcpSocket) {
    return receive(tcpSocket);
}

void TcpServer::joinSend(int tcpSocket) {
    sendInt(tcpSocket, JOIN);
}

void TcpServer::unjoinSend(int tcpSocket) {
    sendInt(tcpSocket, UNJOIN);
}

void TcpServer::startSend(int tcpSocket) {
    sendInt(tcpSocket, START);
}

std::string TcpServer::receive(int tcpSocket) {
    int size = receiveInt(tcpSocket);
    char buf[BUF_SIZE];
    int left = size;
    int total = 0;
    int received;
    do {
        received = read(tcpSocket, &buf[total], left);
        if (received > 0) {
            total += received;
            left -= received;
        }
    } while (left > 0);
    std:: string s(buf);
    return s;
}

int TcpServer::receiveInt(int tcpSocket) {
    int32_t result;
    char *buf = (char*)&result;
    int left = sizeof(result);
    int received;
    do {
        received = read(tcpSocket, buf, left);
        if (received > 0) {
            buf += received;
            left -= received;
        }
    } while (left > 0);
    return ntohl(result);
}

void TcpServer::send(int tcpSocket, const char* msg) {
    //std::cout << "send: (clientSocket) " << clientSocket << " (msg) " <<  msg << std::endl;
    char buf[BUF_SIZE];
    int msgLen = sprintf(buf, "%s", msg);
    sendInt(tcpSocket, msgLen);
    write(tcpSocket, buf, msgLen);
}

void TcpServer::sendString(int tcpSocket, std::string msg) {
    //std::cout << "sendString: (clientSocket) " << clientSocket << " (msg) " <<  msg << std::endl;
    char buf[BUF_SIZE];
    int msgLen = sprintf(buf, "%s", msg.c_str());
    sendInt(tcpSocket, msgLen);
    write(tcpSocket, buf, msgLen);
}

void TcpServer::sendInt(int tcpSocket, int num) {
    //std::cout << "sendInt: (clientSocket) " << clientSocket << " (num) " <<  num << std::endl;
    int32_t convertedNum = htonl(num);
    char *buf = (char*)&convertedNum;
    write(tcpSocket, buf, sizeof(convertedNum));
}

void TcpServer::closeSocket() {
    close(nSocket);
}