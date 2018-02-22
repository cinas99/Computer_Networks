#include "Player.h"

Player::Player(int tcpSocket, sockaddr_in clientSockAddr) {
    this->tcpSocket = tcpSocket;
    this->clientSockAddr = clientSockAddr;
    this->inRoom = false;
    this->ready = false;
}

void Player::setNick(std::string nick) {
    this->nick = nick;
}

std::string Player::getNick() {
    return nick;
}

int Player::getTcpSocket() {
    return tcpSocket;
}

sockaddr_in Player::getSockAddr() {
    return clientSockAddr;
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