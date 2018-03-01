#include "GameServer.h"

GameServer::GameServer() {
    isGameStarted = false;
}

/*
 * TODO:
 * Check if nick is unique
 */

void GameServer::tcpReceive(int tcpSocket, sockaddr_in clientSockAddr) {
    Player player(tcpSocket, clientSockAddr);
    SafeQueue <Message> queue;
    bool isThreadAlive = true;
    while (isThreadAlive) {
        int msg = tcpServer.receiveInt(tcpSocket);
        cout << "TCP Receive: (tcpSocket) " << tcpSocket << " (msg) " << msg << endl;
        switch (msg) {
            case TURN_ON: {
                mPlayers.lock();
                connectedPlayers.push_back(&player);
                cout << "TCP Receive: Turn on (tcpSocket)" << tcpSocket << endl << endl;
                mPlayers.unlock();
                thread tcpSender(&GameServer::tcpSend, this, &player, &queue);
                tcpSender.detach();
                queue.push(TURN_ON);
                break;
            }
            case JOIN: {
                string nick = tcpServer.joinReceive(tcpSocket);
                mPlayers.lock();
                player.setNick(nick);
                cout << "TCP Receive: Join (tcpSocket) " << tcpSocket << " (nick) " << nick << endl << endl;
                mPlayers.unlock();
                queue.push(JOIN);
                break;
            }
            case LEAVE:
                mPlayers.lock();
                player.setInRoom(false);
                player.setReady(false);
                cout << "TCP Receive: Leave (tcpSocket) " << tcpSocket << " (nick) " << player.getNick() << endl
                     << endl;
                mPlayers.unlock();
                queue.push(ROOM_EVENT);
                break;
            case DISCONNECT:
                mPlayers.lock();
                deletePlayer(player);
                //player.setInRoom(false);
                //player.setReady(false);
                cout << "TCP Receive: Disconnect, receiving thread is saying bye-bye! (tcpSocket) " << tcpSocket << endl
                     << endl;
                queue.push(DISCONNECT);
                queue.push(ROOM_EVENT);
                mPlayers.unlock();
                isThreadAlive = false;
                break;
            case READY:
                mPlayers.lock();
                player.setReady(true);
                queue.push(ROOM_EVENT);
                if (isEveryoneReady())
                    queue.push(START);
                cout << "TCP Receive: Ready (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                break;
            case UNREADY:
                mPlayers.lock();
                player.setReady(false);
                cout << "TCP Receive: Unready (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                queue.push(ROOM_EVENT);
                break;
            case UDP_CONNECT:
                mPlayers.lock();
                thread udpReceiver(&GameServer::udpReceive, this, &player);
                mPlayers.unlock();
                udpReceiver.detach();
                cout << "TCP Receive: Udp connect (tcpSocket) " << tcpSocket << endl << endl;
                break;
        }
    }
}

void GameServer::tcpSend(Player *player, SafeQueue<Message> *queue) {
    const int tcpSocket = player->getTcpSocket();
    bool isThreadAlive = true;
    while (isThreadAlive) {
        Message message = queue->get();
        cout << "TCP Send: (tcpSocket) " << tcpSocket << " (message) " << message << endl;
        switch (message) {
            case TURN_ON:
                tcpServer.turnOnSend(tcpSocket);
                queue->push(ROOM_EVENT);
                cout << "TCP Send: Turn on (tcpSocket) " << tcpSocket << endl << endl;
                break;
            case JOIN: {
                mPlayers.lock();
                const bool isInRoom = isFreeSeat() && !isGameStarted;
                player->setInRoom(isInRoom);
                mPlayers.unlock();
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
                mPlayers.lock();
                const int playersInRoom = countRoomPlayers();
                for (vector<Player *>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    tcpServer.roomEventSend((*it)->getTcpSocket(), playersInRoom, connectedPlayers);
                }
                cout << "TCP Send: Room event (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                break;
            }
            case DISCONNECT:
                cout << "TCP Send: Disconnect, sending thread is saying bye-bye! (tcpSocket) " << tcpSocket << endl <<
                                                                                                                 endl;
                isThreadAlive = false;
                break;
            case START:
                mPlayers.lock();
                int numberOfGamePlayers = 0;
                for (vector<Player *>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    if ((*it)->isReady() && (*it)->isInRoom()) {
                        tcpServer.startSend((*it)->getTcpSocket());
                        numberOfGamePlayers++;
                    }
                }
                isGameStarted = true;
                udpServer.init();
                cout << "TCP Send: Start (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                mBoard.lock();
                Board &board = Board::getInstance();
                board.start(numberOfGamePlayers, tcpServer, udpServer);
                mBoard.unlock();
                break;
        }
    }
}

void GameServer::udpReceive(Player *player) {
    SafeQueue <string> queue;
    thread udpSender(&GameServer::udpSend, this, player, &queue);
    udpSender.detach();
    while (true) {
        string msg = udpServer.receive(player);
        cout << "UDP Receive: (msg) " << msg << endl;
        queue.push(msg);
    }
}

void GameServer::udpSend(Player *player, SafeQueue<string> *queue) {
    while (true) {
        string msg = queue->get();
        udpServer.send(player, msg);
        cout << "UDP Send: (tcpSocket) " << player->getTcpSocket() << " (msg) " << msg << endl;
    }
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

void GameServer::deletePlayer(Player player) {
    std::vector<Player *>::iterator foundPlayer;
    int tcpSocket = player.getTcpSocket();
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if((*it)->getTcpSocket() == tcpSocket) {
            foundPlayer = it;
            break;
        }
    }
    connectedPlayers.erase(foundPlayer);
    cout << "DeletePlayer: current number of players " << connectedPlayers.size() << endl << endl;
}