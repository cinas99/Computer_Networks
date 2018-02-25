#include "GameServer.h"

GameServer::GameServer() {
    isGameStarted = false;
}

/*
 * TODO:
 * Message when player disconnects, then delete it from server
 * Check if nick is unique
 */

void GameServer::tcpReceive(int tcpSocket, sockaddr_in clientSockAddr) {
    Player player(tcpSocket, clientSockAddr);
    SafeQueue <Message> queue;
    while (true) {
        int msg = tcpServer.receiveInt(tcpSocket);
        cout << "TCP Receive: (tcpSocket) " << tcpSocket << " (msg) " << msg << endl;
        switch (msg) {
            case TURN_ON: {
                m.lock();
                connectedPlayers.push_back(&player);
                cout << "TCP Receive: Turn on (tcpSocket)" << tcpSocket << endl << endl;
                m.unlock();
                thread tcpSender(&GameServer::tcpSend, this, &player, &queue);
                tcpSender.detach();
                queue.push(TURN_ON);
                break;
            }
            case JOIN: {
                string nick = tcpServer.joinReceive(tcpSocket);
                m.lock();
                player.setNick(nick);
                cout << "TCP Receive: Join (tcpSocket) " << tcpSocket << " (nick) " << nick << endl << endl;
                m.unlock();
                queue.push(JOIN);
                break;
            }
            case LEAVE:
                m.lock();
                player.setInRoom(false);
                player.setReady(false);
                cout << "TCP Receive: Leave (tcpSocket) " << tcpSocket << " (nick) " << player.getNick() << endl
                     << endl;
                m.unlock();
                queue.push(ROOM_EVENT);
                break;
            case READY:
                m.lock();
                player.setReady(true);
                queue.push(ROOM_EVENT);
                if (isEveryoneReady())
                    queue.push(START);
                cout << "TCP Receive: Ready (tcpSocket) " << tcpSocket << endl << endl;
                m.unlock();
                break;
            case UNREADY:
                m.lock();
                player.setReady(false);
                cout << "TCP Receive: Unready (tcpSocket) " << tcpSocket << endl << endl;
                m.unlock();
                queue.push(ROOM_EVENT);
                break;
            case UDP_CONNECT:
                m.lock();
                thread udpReceiver(&GameServer::udpReceive, this, &player);
                m.unlock();
                udpReceiver.detach();
                cout << "TCP Receive: Udp connect (tcpSocket) " << tcpSocket << endl << endl;
                break;
        }
    }
}

void GameServer::tcpSend(Player *player, SafeQueue<Message> *queue) {
    const int tcpSocket = player->getTcpSocket();
    while (true) {
        Message message = queue->get();
        cout << "TCP Send: (tcpSocket) " << tcpSocket << " (message) " << message << endl;
        switch (message) {
            case TURN_ON:
                tcpServer.turnOnSend(tcpSocket);
                queue->push(ROOM_EVENT);
                cout << "TCP Send: Turn on (tcpSocket) " << tcpSocket << endl << endl;
                break;
            case JOIN: {
                m.lock();
                const bool isInRoom = isFreeSeat() && !isGameStarted;
                player->setInRoom(isInRoom);
                m.unlock();
                if (isInRoom) {
                    queue->push(ROOM_EVENT);
                    tcpServer.joinSend(tcpSocket);
                    cout << "TCP Send: Join (tcpSocket) " << tcpSocket << " (nick) " << player->getNick() << endl << endl;
                }
                else {
                    tcpServer.unjoinSend(tcpSocket);
                    cout << "TCP Send: Unjoin (tcpSocket) " << tcpSocket << " (nick) " << player->getNick() <<
                                                                                                                endl << endl;
                }
                break;
            }
            case ROOM_EVENT: {
                m.lock();
                const int playersInRoom = countRoomPlayers();
                for (vector<Player *>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    tcpServer.roomEventSend((*it)->getTcpSocket(), playersInRoom, connectedPlayers);
                }
                cout << "TCP Send: Room event (tcpSocket) " << tcpSocket << endl << endl;
                m.unlock();
                break;
            }
            case START:
                m.lock();
                for (vector<Player *>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    if ((*it)->isReady() && (*it)->isInRoom()) {
                        tcpServer.startSend((*it)->getTcpSocket());
                    }
                }
                isGameStarted = true;
                udpServer.init();
                cout << "TCP Send: Start (tcpSocket) " << tcpSocket << endl << endl;
                m.unlock();
                break;
        }
    }
}

void GameServer::udpReceive(Player *player) {
    string msg = udpServer.receive(player->getSockAddr());
    cout << "UDP Receive: (msg) " << msg << endl << endl;
}

void GameServer::run() {
    cout << "Server started running!" << endl;
    tcpServer.init();
    while(true) {
        sockaddr_in clientSockAddr;
        int clientSocket = tcpServer.clientAccept(&clientSockAddr);
        thread tcpReceiver(&GameServer::tcpReceive, this, clientSocket, clientSockAddr);
        tcpReceiver.detach();
    }
}

bool GameServer::isEveryoneReady() {
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if((*it)->isInRoom()) {
            if ((*it)->isReady())
                continue;
            else
                return false;
        }
    }
    return true;
}

bool GameServer::isFreeSeat() {
    return countRoomPlayers() < MAX_ROOM_SEATS;
}

int GameServer::countRoomPlayers() {
    int result = 0;
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if((*it)->isInRoom()) {
            result++;
        }
    }
    return result;
}