#include "Player.h"
#include "Point.h"
#include "Message.h"

#include <math.h>
#include <vector>
#include <string>
#include <iostream>

//Player::Player(){};

Player::Player(int tcpSocket, sockaddr_in clientSockAddr, SafeQueue <Message> *tcpQueue) {
    this->tcpSocket = tcpSocket;
    this->clientSockAddr = clientSockAddr;
    this->inRoom = false;
    this->ready = false;
    this->tcpQueue = tcpQueue;
    this->threadAlive = true;
}

Point Player::init(double startX, double startY, double angle) {
    this->currentX = startX;
    this->currentY = startY;
    this->angle = angle;
    this->draw = false;
    this->nowPlaying = true;
    Point point(currentX, currentY, !draw);
    visited.emplace_back(point);
    return point;
}

Point Player::generateNextLine() {
    if (turn == -1) {
        angle -= CIRCULAR_SPEED;
    }
    else if (turn == 1) {
        angle += CIRCULAR_SPEED;
    }
    currentX += LINEAR_SPEED * cos(angle);
    currentY += LINEAR_SPEED * sin(angle);
    Point point(currentX, currentY);
    if (!draw)
        point.setGap(true);
    visited.emplace_back(point);
    return point;
}

Point Player::getPoint(int index) {
    return visited[index];
}

int Player::getVisitedSize() {
    return visited.size();
}

void Player::markGap() {
    if (nowPlaying) {
        const int size = visited.size();
        Point last = visited[size-1];
        visited[size-1] = Point(last.getX(), last.getY(), true);
    }
}

void Player::setTurn(int turn) {
    this->turn = turn;
}

void Player::setDraw(bool draw) {
    this->draw = draw;
}

vector<Point> Player::getVisited() {
    return visited;
}

bool Player::isNowPlaying() {
    return nowPlaying;
}

void Player::setNowPlaying(bool nowPlaying) {
    this->nowPlaying = nowPlaying;
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

void Player::setSockAddr(sockaddr_in clientSockAddr) {
    this->clientSockAddr = clientSockAddr;
}

SafeQueue <Message> *Player::getTcpQueue() {
    return tcpQueue;
}

SafeQueue <string> *Player::getUdpQueue() {
    return udpQueue;
}

void Player::setUdpQueue(SafeQueue <string> *udpQueue) {
    this->udpQueue = udpQueue;
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

bool Player::isThreadAlive() {
    return threadAlive;
}

void Player::setThreadAlive(bool threadAlive) {
    this->threadAlive = threadAlive;
}