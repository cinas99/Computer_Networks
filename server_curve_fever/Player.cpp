#include "Player.h"
#include "Point.h"
#include "Message.h"

#include <math.h>
#include <vector>
#include <string>
#include <iostream>

Player::Player(int tcpSocket, sockaddr_in clientSockAddr, SafeQueue <Message> *tcpQueue) {
    this->tcpSocket = tcpSocket;

    mClientSockAddr.lock();
    this->clientSockAddr = clientSockAddr;
    mClientSockAddr.unlock();

    mInRoom.lock();
    this->inRoom = false;
    mInRoom.unlock();

    mReady.lock();
    this->ready = false;
    mReady.unlock();

    this->tcpQueue = tcpQueue;

    mThreadAlive.lock();
    this->threadAlive = true;
    mThreadAlive.unlock();

    mUdpSet.lock();
    this->udpSet = false;
    mUdpSet.unlock();
}

Point Player::init(double startX, double startY, double angle) {
    this->currentX = startX;
    this->currentY = startY;
    this->angle = angle;

    mDraw.lock();
    this->draw = false;
    mDraw.unlock();

    mNowPlaying.lock();
    this->nowPlaying = true;
    mNowPlaying.unlock();

    Point point(currentX, currentY, !draw);
    visited.emplace_back(point);
    return point;
}

Point Player::generateNextLine() {
    mTurn.lock();
    int nowTurn = turn;
    mTurn.unlock();

    if (nowTurn == -1) {
        angle -= CIRCULAR_SPEED;
    }
    else if (nowTurn == 1) {
        angle += CIRCULAR_SPEED;
    }
    currentX += LINEAR_SPEED * cos(angle);
    currentY += LINEAR_SPEED * sin(angle);
    Point point(currentX, currentY);

    mDraw.lock();
    bool nowDraw = !draw;
    mDraw.unlock();

    if (nowDraw)
        point.setGap(true);
    visited.emplace_back(point);
    return point;
}

int Player::getVisitedSize() {
    return visited.size();
}

void Player::markGap() {
    mNowPlaying.lock();
    bool isNowPlaying = nowPlaying;
    mNowPlaying.unlock();

    if (isNowPlaying) {
        const int size = visited.size();
        Point last = visited[size-1];
        visited[size-1] = Point(last.getX(), last.getY(), true);
    }
}

void Player::setTurn(int turn) {
    mTurn.lock();
    this->turn = turn;
    mTurn.unlock();
}

void Player::setDraw(bool draw) {
    this->draw = draw;
}

bool Player::isUdpSet() {
    return udpSet;
}

void Player::setUdpSet(bool set) {
    mUdpSet.lock();
    this->udpSet = set;
    mUdpSet.unlock();
}

std::vector<Point> Player::getVisited() {
    return visited;
}

bool Player::isNowPlaying() {
    return nowPlaying;
}

void Player::setNowPlaying(bool nowPlaying) {
    mNowPlaying.lock();
    this->nowPlaying = nowPlaying;
    mNowPlaying.unlock();
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
    mClientSockAddr.lock();
    this->clientSockAddr = clientSockAddr;
    mClientSockAddr.unlock();
}

SafeQueue <Message> *Player::getTcpQueue() {
    return tcpQueue;
}

SafeQueue <std::string> *Player::getUdpSendQueue() {
    return udpSendQueue;
}

void Player::setUdpSendQueue(SafeQueue<std::string> *udpQueue) {
    this->udpSendQueue = udpQueue;
}

SafeQueue <std::string> *Player::getUdpReceiveQueue() {
    return udpReceiveQueue;
}

void Player::setUdpReceiveQueue(SafeQueue<std::string> *udpQueue) {
    this->udpReceiveQueue = udpQueue;
}

void Player::setInRoom(bool inRoom) {
    mInRoom.lock();
    this->inRoom = inRoom;
    mInRoom.unlock();
}

bool Player::isInRoom() {
    return inRoom;
}

void Player::setReady(bool ready) {
    mReady.lock();
    this->ready = ready;
    mReady.unlock();
}

bool Player::isReady() {
    return ready;
}

bool Player::isThreadAlive() {
    return threadAlive;
}

void Player::setThreadAlive(bool threadAlive) {
    mThreadAlive.lock();
    this->threadAlive = threadAlive;
    mThreadAlive.unlock();
}