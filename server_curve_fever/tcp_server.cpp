#include "tcp_server.h"

TcpServer::TcpServer() {
}

void TcpServer::init() {
    int nFoo = 1;

    /* address structure */
    memset(&sockServer, 0, sizeof(struct sockaddr));
    sockServer.sin_family = AF_INET;
    sockServer.sin_addr.s_addr = htonl(INADDR_ANY);
    sockServer.sin_port = htons(SERVER_PORT);

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

int TcpServer::getClientSocket() {
    return nClientSocket;
}

sockaddr_in TcpServer::getClientSockAddr() {
    return sockClient;
}

void TcpServer::clientAccept() {
    socklen_t sockLen = sizeof(struct sockaddr);
    nClientSocket = accept(nSocket, (struct sockaddr*)&sockClient, &sockLen);
    if (nClientSocket < 0)
    {
        perror ("Nie można utworzyć gniazdka przy połączeniu z klientem");
        exit(1);
    }
    printf("Połączenie z %s\n",
           inet_ntoa((struct in_addr)sockClient.sin_addr));
}

std::string TcpServer::receive(int clientSocket) {
    int size = receiveInt(clientSocket);
    char buf[BUF_SIZE];
    int left = size;
    int total = 0;
    int received;
    do {
        received = read(clientSocket, &buf[total], left);
        if (received > 0) {
            total += received;
            left -= received;
        }
    } while (left > 0);
    std:: string s(buf);
    return s;
}

int TcpServer::receiveInt(int clientSocket) {
    int32_t result;
    char *buf = (char*)&result;
    int left = sizeof(result);
    int received;
    do {
        received = read(clientSocket, buf, left);
        if (received > 0) {
            buf += received;
            left -= received;
        }
    } while (left > 0);
    return ntohl(result);
}

int TcpServer::countRoomPlayers(std::vector<Player*> connectedPlayers) {
    int result = 0;
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if((*it)->getInRoom()) {
            result++;
        }
    }
    return result;
}

void TcpServer::turnOnSend(int clientSocket, std::vector<Player*> connectedPlayers) {
    sendInt(clientSocket, TURN_ON);
}

void TcpServer::roomEventSend(int clientSocket, std::vector<Player*> connectedPlayers) {
    sendInt(clientSocket, ROOM_EVENT);
    int playersInRoom = countRoomPlayers(connectedPlayers);
    sendInt(clientSocket, playersInRoom);
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if ((*it)->getInRoom()) {
            sendString((*it)->getClientSocket(), (*it)->getNick());
        }
    }
}

std::string TcpServer::joinReceive(int clientSocket) {
    return receive(clientSocket);
}

bool TcpServer::joinSend(int clientSocket, std::vector<Player*> connectedPlayers) {
    int playersInRoom = countRoomPlayers(connectedPlayers);
    if (playersInRoom < 4) {
        sendInt(clientSocket, JOIN);
        return true;
    }
    else {
        sendInt(clientSocket, DECLINE);
        return false;
    }
}

void TcpServer::send(int clientSocket, const char* msg) {
    std::cout << "send: " << msg << std::endl;
    char buf[BUF_SIZE];
    int msgLen = sprintf(buf, "%s\n", msg);
    write(clientSocket, buf, msgLen);
}

void TcpServer::sendString(int clientSocket, std::string msg) {
    std::cout << "sendString: " << msg << std::endl;
    char buf[BUF_SIZE];
    int msgLen = sprintf(buf, "%s\n", msg.c_str());
    write(clientSocket, buf, msgLen);
}

void TcpServer::sendInt(int clientSocket, int num) {
    std::cout << "sendInt: " << num << std::endl;
    int32_t convertedNum = htonl(num);
    char *buf = (char*)&convertedNum;
    write(clientSocket, buf, sizeof(convertedNum));
}

void TcpServer::closeSocket() {
    close(nSocket);
}