#include "player.h"

Player::Player(int clientSocket, sockaddr_in sockAddrClient) {
    this->sockAddrClient = sockAddrClient;
    this->clientSocket = clientSocket;
    this->inRoom = false;
    this->ready = false;
}

void Player::setNick(std::string nick) {
    this->nick = nick;
}

std::string Player::getNick() {
    return nick;
}

int Player::getClientSocket() {
    return clientSocket;
}

void Player::setInRoom(bool inRoom) {
    this->inRoom = inRoom;
}

bool Player::isInRoom() {
    return inRoom;
}

void Player::setReady(bool ready) {
    this->ready = ready;
}

bool Player::isReady() {
    return ready;
}