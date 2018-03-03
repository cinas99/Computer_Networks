#include "GameServer.h"

GameServer::GameServer() {
    isGameStarted = false;
}

/*
 * TODO:
 * Check if nick is unique
 */

void GameServer::tcpReceive(int tcpSocket, sockaddr_in clientSockAddr) {
    SafeQueue <Message> tcpQueue;
    Player player(tcpSocket, clientSockAddr, &tcpQueue);
    bool isThreadAlive = true;
    while (isThreadAlive) {
        int msg = tcpServer.receiveInt(tcpSocket);
        //cout << "TCP Receive: (tcpSocket) " << tcpSocket << " (msg) " << msg << endl;
        switch (msg) {
            case TURN_ON: {
                mPlayers.lock();
                connectedPlayers.push_back(&player);
                cout << "TCP Receive: Turn on (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                thread tcpSender(&GameServer::tcpSend, this, &player);//, &tcpQueue);
                tcpSender.detach();
                tcpQueue.push(TURN_ON);
                break;
            }
            case JOIN: {
                string nick = tcpServer.receive(tcpSocket);
                mPlayers.lock();
                player.setNick(nick);
                cout << "TCP Receive: Join (tcpSocket) " << tcpSocket << " (nick) " << nick << endl << endl;
                mPlayers.unlock();
                tcpQueue.push(JOIN);
                break;
            }
            case LEAVE:
                mPlayers.lock();
                player.setInRoom(false);
                player.setReady(false);
                cout << "TCP Receive: Leave (tcpSocket) " << tcpSocket << " (nick) " << player.getNick() << endl
                     << endl;
                mPlayers.unlock();
                tcpQueue.push(ROOM_EVENT);
                break;
            case DISCONNECT:
                mPlayers.lock();
                deletePlayer(player);
                cout << "TCP Receive: Disconnect, receiving thread is saying bye-bye! (tcpSocket) " << tcpSocket << endl
                     << endl;
                tcpQueue.push(DISCONNECT);
                tcpQueue.push(ROOM_EVENT);
                mPlayers.unlock();
                isThreadAlive = false;
                break;
            case READY:
                mPlayers.lock();
                player.setReady(true);
                tcpQueue.push(ROOM_EVENT);
                if (isEveryoneReady())
                    tcpQueue.push(START);
                cout << "TCP Receive: Ready (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                break;
            case UNREADY:
                mPlayers.lock();
                player.setReady(false);
                cout << "TCP Receive: Unready (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                tcpQueue.push(ROOM_EVENT);
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

void GameServer::tcpSend(Player *player) {
    const int tcpSocket = player->getTcpSocket();
    SafeQueue<Message> *tcpQueue = player->getTcpQueue();
    bool isThreadAlive = true;
    while (isThreadAlive) {
        Message message = tcpQueue->get();
        //cout << "TCP Send: (tcpSocket) " << tcpSocket << " (message) " << message << endl;
        switch (message) {
            case TURN_ON:
                tcpServer.sendInt(tcpSocket, TURN_ON);
                tcpQueue->push(ROOM_EVENT);
                cout << "TCP Send: Turn on (tcpSocket) " << tcpSocket << endl << endl;
                break;
            case JOIN: {
                mPlayers.lock();
                bool checkFreeSeat = isFreeSeat();
                const bool isInRoom = checkFreeSeat && !isGameStarted;
                player->setInRoom(isInRoom);
                mPlayers.unlock();
                if (isInRoom) {
                    tcpQueue->push(ROOM_EVENT);
                    tcpServer.sendInt(tcpSocket, JOIN);
                    cout << "TCP Send: Join (tcpSocket) " << tcpSocket << " (nick) " << player->getNick() << endl << endl;
                }
                else {
                    tcpServer.sendInt(tcpSocket, UNJOIN);
                    cout << "TCP Send: Unjoin (tcpSocket) " << tcpSocket << " (nick) " << player->getNick() << " "
                            "(checkFreeSeat) " << checkFreeSeat << " (isGameStarted) " << isGameStarted <<
                                                                                                                endl << endl;
                }
                break;
            }
            case ROOM_EVENT: {
                mPlayers.lock();
                const int playersInRoom = countRoomPlayers();
                for (vector<Player *>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    roomEventSend((*it)->getTcpSocket(), playersInRoom);
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
            case START: {
                mPlayers.lock();
                gamePlayers = getGamePlayers();
                for (vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    if ((*it)->isReady() && (*it)->isInRoom()) {
                        int playerTcpSocket = (*it)->getTcpSocket();
                        tcpServer.sendInt(playerTcpSocket, START);
                        tcpServer.sendInt(playerTcpSocket, gamePlayers.size());
                    }
                }
                isGameStarted = true;
                udpServer.init();
                cout << "TCP Send: Start (tcpSocket) " << tcpSocket << endl << endl;
                mPlayers.unlock();
                break;
            }
            case CONFIRM_UDP_MESSAGE:
                tcpServer.sendInt(tcpSocket, CONFIRM_UDP_MESSAGE);
                cout << "TCP Send: Udp message is confirmed (tcpSocket) " << tcpSocket << endl << endl;
                break;
        }
    }
}

void GameServer::udpReceive(Player *player) {
    //const int tcpSocket = player->getTcpSocket();
    SafeQueue<Message> *tcpQueue = player->getTcpQueue();
    SafeQueue <string> udpQueue;
    player->setUdpQueue(&udpQueue);
    thread udpSender(&GameServer::udpSend, this, player);
    udpSender.detach();

    mPlayers.lock();
    int num;
    do {
        num = udpServer.receiveInt(player);
    } while (num != FIRST_UDP_MESSAGE);
    cout << "UDP Receive: Udp connection is established! (nick) " << player->getNick() << endl << endl;
    tcpQueue->push(CONFIRM_UDP_MESSAGE);
    mPlayers.unlock();

    mBoard.lock();
    Board &board = Board::getInstance();
    board.start(getGamePlayers());
    mBoard.unlock();

    while (true) {
        string msg = udpServer.receive(player);
        if (msg.substr(0, 1) == "L") {
            mPlayers.lock();
            player->setTurn(-1);
            mPlayers.unlock();
        }
        else if (msg.substr(0, 1) == "R") {
            mPlayers.lock();
            player->setTurn(1);
            mPlayers.unlock();
        }
        else if (msg.substr(0, 1) == "S") {
            mPlayers.lock();
            player->setTurn(0);
            mPlayers.unlock();
        }
        //cout << "UDP Receive: (msg) " << msg << " (length) " << msg.length() << endl << endl;
    }
}

void GameServer::udpSend(Player *player) {
    SafeQueue<Message> *tcpQueue = player->getTcpQueue();
    SafeQueue<string> *udpQueue = player->getUdpQueue();
    while (true) {
        string msg = udpQueue->get();
        udpServer.send(player, msg);
        //cout << "UDP Send: (nick) " << player->getNick() << " (msg) " << msg << endl << endl;
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

void GameServer::roomEventSend(int tcpSocket, int playersInRoom) {
    tcpServer.sendInt(tcpSocket, ROOM_EVENT);
    tcpServer.sendInt(tcpSocket, playersInRoom);
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if ((*it)->isInRoom()) {
            tcpServer.sendString(tcpSocket, (*it)->getNick());
            if ((*it)->isReady())
                tcpServer.sendInt(tcpSocket, TRUE);
            else
                tcpServer.sendInt(tcpSocket, FALSE);
        }
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

std::vector <Player*> GameServer::getGamePlayers() {
    std::vector <Player*> result;
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if((*it)->isInRoom() && (*it)->isReady()){
            result.emplace_back(*it);
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