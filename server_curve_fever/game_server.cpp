#include "game_server.h"

GameServer::GameServer() {
}

void GameServer::clientReceive(int clientSocket, sockaddr_in sockAddrClient) {
    Player player(clientSocket, sockAddrClient);
    SafeQueue <Message> queue;
    while (true) {
        int msg = tcpServer.receiveInt(clientSocket);
        cout << "Receive: (clientsocket) " << clientSocket << " (msg) " << msg << endl;
        switch(msg) {
            case TURN_ON:
            {
                cout << "Receive: Turn on " << clientSocket << endl << endl;
                m.lock();
                connectedPlayers.push_back(&player);
                m.unlock();
                thread clientSender(&GameServer::clientSend, this, &player, &queue);
                clientSender.detach();
                queue.push(TURN_ON);
                break;
            }
            case JOIN:
            {
                string nick = tcpServer.joinReceive(clientSocket);
                m.lock();
                player.setNick(nick);
                m.unlock();
                queue.push(JOIN);
                cout << "Receive: Join (clientsocket) " << clientSocket << " (nick) " << nick << endl << endl;
                break;
            }
            case LEAVE:
                m.lock();
                player.setInRoom(false);
                player.setReady(false);
                m.unlock();
                queue.push(ROOM_EVENT);
                cout << "Receive: Leave (clientsocket) " << clientSocket << " (nick) " << player.getNick() << endl << endl;
                break;
            case READY:
                m.lock();
                player.setReady(true);
                m.unlock();
                cout << "Receive: Ready (clientsocket) " << clientSocket << endl << endl;
                break;
        }
    }
}

void GameServer::clientSend(Player *player, SafeQueue <Message> *queue) {
    const int clientSocket = player->getClientSocket();
    while(true) {
        Message message = queue->get();
        cout << "Send: (clientsocket) " << clientSocket << " (message) " << message << endl;
        switch (message) {
            case TURN_ON:
                tcpServer.turnOnSend(clientSocket, connectedPlayers);
                cout << "Send: Turn on (clientsocket) " << clientSocket << endl << endl;
                break;
            case JOIN:
            {
                const bool isInRoom = tcpServer.joinSend(clientSocket, connectedPlayers);
                m.lock();
                player->setInRoom(isInRoom);
                m.unlock();
                if (isInRoom)
                    queue->push(ROOM_EVENT);
                cout << "Send: Join (clientsocket) " << clientSocket << " (nick) " << player->getNick() << endl << endl;
                break;
            }
            case ROOM_EVENT:
                m.lock();
                for (vector<Player*>::iterator it = connectedPlayers.begin(); it != connectedPlayers.end(); ++it) {
                    tcpServer.roomEventSend((*it)->getClientSocket(), connectedPlayers);
                }
                m.unlock();
                cout << "Send: Room event (clientsocket) " << clientSocket << endl << endl;
                break;
        }
    }
}

void GameServer::run() {
    cout << "Server started running!" << endl;
    tcpServer.init();
    while(true) {
        tcpServer.clientAccept();
        thread clientReceiver(&GameServer::clientReceive, this, tcpServer.getClientSocket(), tcpServer.getClientSockAddr());
        clientReceiver.detach();
    }
}