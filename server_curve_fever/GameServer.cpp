#include "GameServer.h"

GameServer::GameServer() {
    isGameStarted = false;
}

using namespace std;

/*
 * TODO:
 * Check if nick is unique
 */

void GameServer::tcpReceive(int tcpSocket, sockaddr_in clientSockAddr) {
    SafeQueue <Message> tcpQueue;
    Player player(tcpSocket, clientSockAddr, &tcpQueue);
    SafeQueue <string> udpReceiveQueue;
    player.setUdpReceiveQueue(&udpReceiveQueue);
    cout << player.getNick() << " " << player.getSockAddr().sin_addr.s_addr << endl;
    bool isThreadAlive = true;
    while (isThreadAlive) {
        int msg = tcpServer.receiveInt(tcpSocket);
        //cout << "TCP Receive: (tcpSocket) " << tcpSocket << " (msg) " << msg << endl;
        switch (msg) {
            case TURN_ON: {
                mConnected.lock();
                connectedPlayers.push_back(&player);
                mConnected.unlock();
                cout << "TCP Receive: Turn on (tcpSocket) " << tcpSocket << endl << endl;
                thread tcpSender(&GameServer::tcpSend, this, &player);//, &tcpQueue);
                tcpSender.detach();
                tcpQueue.push(TURN_ON);
                break;
            }
            case JOIN: {
                string nick = tcpServer.receive(tcpSocket);
                player.setNick(nick);
                cout << "TCP Receive: Join (tcpSocket) " << tcpSocket << " (nick) " << nick << endl << endl;
                tcpQueue.push(JOIN);
                break;
            }
            case LEAVE:
                //mPlayer.lock();
                player.setInRoom(false);
                player.setReady(false);
                cout << "TCP Receive: Leave (tcpSocket) " << tcpSocket << " (nick) " << player.getNick() << endl
                     << endl;
                //mPlayer.unlock();
                tcpQueue.push(ROOM_EVENT);
                break;
            case DISCONNECT:
                mConnected.lock();
                deletePlayer(&player);
                mConnected.unlock();
                cout << "TCP Receive: Disconnect, receiving thread is saying bye-bye! (tcpSocket) " << tcpSocket << endl
                     << endl;
                tcpQueue.push(DISCONNECT);
                tcpQueue.push(ROOM_EVENT);
                //mPlayer.lock();
                player.setThreadAlive(false);
                //mPlayer.unlock();
                break;
            case READY: {
                //mPlayer.lock();
                player.setReady(true);
                //mPlayer.unlock();
                tcpQueue.push(ROOM_EVENT);
                mConnected.lock();
                bool isEverybodyReady = isEveryoneReady();
                mConnected.unlock();
                if (isEverybodyReady)
                    tcpQueue.push(START);
                cout << "TCP Receive: Ready (tcpSocket) " << tcpSocket << endl << endl;
                break;
            }
            case UNREADY:
                //mPlayer.lock();
                player.setReady(false);
                cout << "TCP Receive: Unready (tcpSocket) " << tcpSocket << endl << endl;
                //mPlayer.unlock();
                tcpQueue.push(ROOM_EVENT);
                break;
            case UDP_CONNECT:
                thread udpReceiver(&GameServer::udpAction, this, &player);
                udpReceiver.detach();
                cout << "TCP Receive: Udp connect (tcpSocket) " << tcpSocket << endl << endl;
                break;
        }
        isThreadAlive = player.isThreadAlive();
    }
}

void GameServer::tcpSend(Player *player) {
    //mPlayer.lock();
    const int tcpSocket = player->getTcpSocket();
    SafeQueue<Message> *tcpQueue = player->getTcpQueue();
    //mPlayer.unlock();
    bool isThreadAlive = true;
    while (isThreadAlive) {
        Message message = tcpQueue->get();
        cout << "TCP Send: (tcpSocket) " << tcpSocket << " (message) " << message << endl;
        switch (message) {
            case TURN_ON:
                tcpServer.sendInt(tcpSocket, TURN_ON);
                tcpQueue->push(ROOM_EVENT);
                cout << "TCP Send: Turn on (tcpSocket) " << tcpSocket << endl << endl;
                break;
            case JOIN: {
                mConnected.lock();
                bool checkFreeSeat = isFreeSeat();
                mConnected.unlock();
                mGameStarted.lock();
                const bool isInRoom = checkFreeSeat && !isGameStarted;
                mGameStarted.unlock();
                //mPlayer.lock();
                player->setInRoom(isInRoom);
                //mPlayer.unlock();
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
                mConnected.lock();
                const int playersInRoom = countRoomPlayers();
                for (vector<Player *>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    roomEventSend((*it)->getTcpSocket(), playersInRoom);
                }
                mConnected.unlock();
                cout << "TCP Send: Room event (tcpSocket) " << tcpSocket << endl << endl;
                break;
            }
            case DISCONNECT:
                cout << "TCP Send: Disconnect, sending thread is saying bye-bye! (tcpSocket) " << tcpSocket << endl <<
                                                                                                                 endl;
                //mPlayer.lock();
                player->setThreadAlive(false);
                //mPlayer.unlock();
                break;
            case START: {
                mConnected.lock();
                gamePlayers = getGamePlayers();
                for (vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    if ((*it)->isReady() && (*it)->isInRoom()) {
                        int playerTcpSocket = (*it)->getTcpSocket();
                        tcpServer.sendInt(playerTcpSocket, START);
                        mGamePlayers.lock();
                        tcpServer.sendInt(playerTcpSocket, gamePlayers.size());
                        mGamePlayers.unlock();
                    }
                }
                mConnected.unlock();
                mGameStarted.lock();
                isGameStarted = true;
                mGameStarted.unlock();
                udpServer.init();
                cout << "TCP Send: Start (tcpSocket) " << tcpSocket << endl << endl;
                break;
            }
            case CONFIRM_UDP_MESSAGE:
                tcpServer.sendInt(tcpSocket, CONFIRM_UDP_MESSAGE);
                cout << "TCP Send: Udp message is confirmed (tcpSocket) " << tcpSocket << endl << endl;
                break;
            case RESULTS:
                //cout << "RESULTS" << endl;
                tcpServer.sendInt(tcpSocket, RESULTS);
                tcpServer.sendString(tcpSocket, Board::getInstance().getWinner());
                cout << "TCP Send: Winner nick is send! (tcpSocket) " << tcpSocket << " (nick) " <<
                     Board::getInstance().getWinner() << endl << endl;
                break;
        }
        isThreadAlive = player->isThreadAlive();
    }
}

void GameServer::udpReceive() {
    //cout << "UDP Receive: is running!" << endl << endl;
    while(true) {
        udpServer.receive(gamePlayers);
    }
}

void GameServer::udpAction(Player *player) {
    SafeQueue <string> udpSendQueue;
    player->setUdpSendQueue(&udpSendQueue);
    thread udpSender(&GameServer::udpSend, this, player);
    udpSender.detach();

    SafeQueue <string> *udpReceiveQueue = player->getUdpReceiveQueue();

    string firstMsg;
    do {
        firstMsg = udpReceiveQueue->get();
    } while (firstMsg.substr(0, 1) != "F");
    cout << "UDP Receive: Udp connection is established! (nick) " << player->getNick() << endl << endl;
    player->getTcpQueue()->push(CONFIRM_UDP_MESSAGE);

    //cout << "PLAYER: " << player->getNick() << endl;

    //mBoard.lock();
    Board &board = Board::getInstance();
    mGamePlayers.lock();
    board.start(getGamePlayers());
    mGamePlayers.unlock();
    // can cause a problem
    //mBoard.unlock();

    bool isNowPlaying = true;
    while (isNowPlaying) {
        string msg = udpReceiveQueue->get();
        //cout << player->getNick() << " " << player->getSockAddr().sin_addr.s_addr << " " << msg << endl;
        if (msg.substr(0, 1) == "L") {
            player->setTurn(-1);
            //cout << "UDP RECEIVER LEFT " << player->getNick() << endl;
        }
        else if (msg.substr(0, 1) == "R") {
            player->setTurn(1);
            //cout << "UDP RECEIVER RIGHT " << player->getNick() << endl;
        }
        else if (msg.substr(0, 1) == "S") {
            player->setTurn(0);
            //cout << "UDP RECEIVER STRAIGHT " << player->getNick() << endl;
        }
        isNowPlaying = player->isNowPlaying();
        //cout << "UDP Receive: (msg) " << msg << " (length) " << msg.length() << endl << endl;
    }
}

void GameServer::udpSend(Player *player) {
    //mPlayer.lock();
    SafeQueue<string> *udpQueue = player->getUdpSendQueue();
    //mPlayer.unlock();
    bool isNowPlaying = true;
    while (isNowPlaying) {
        string msg = udpQueue->get();
        udpServer.send(player, msg);
        isNowPlaying = player->isNowPlaying();
        //cout << "UDP Send: (nick) " << player->getNick() << " (msg) " << msg << endl << endl;
    }
}


void GameServer::run() {
    cout << "Server started running!" << endl;
    tcpServer.init();
    thread udpReceiver(&GameServer::udpReceive, this);
    udpReceiver.detach();
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

void GameServer::deletePlayer(Player *player) {
    std::vector<Player *>::iterator foundPlayer;
    int tcpSocket = player->getTcpSocket();
    for (std::vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
        if((*it)->getTcpSocket() == tcpSocket) {
            foundPlayer = it;
            break;
        }
    }
    connectedPlayers.erase(foundPlayer);
    cout << "DeletePlayer: current number of players " << connectedPlayers.size() << endl << endl;
}