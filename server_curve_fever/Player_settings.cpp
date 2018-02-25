//
// Created by marcin on 13.02.18.
//

#include "Player.h"
#include "Point.h"

#include <math.h>
#include <vector>
#include <string>

using namespace std;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "NotAssignable"

Player::Player(){};

Player::Player(double startX, double startY, double angle) {
    this->currentX = startX;
    this->currentY = startY;
    this->angle = angle;
    this->draw = false;
    //this->color = color;
    this->nowPlaying = true;
    this->cur_point = setCurPoint(startX,startY);

    if (draw) {
        visited.emplace_back(cur_point);
    }
}

Player::Player(double startX, double startY, double angle, string color):color(std::move(color)) {
    this->currentX = startX;
    this->currentY = startY;
    this->angle = angle;
    this->draw = false;
    this->color = color;
    this->nowPlaying = true;
    this->cur_point = setCurPoint(startX,startY);

    if (draw) {
        visited.emplace_back(cur_point);
    }
}

 void Player::generateNextLine() {
        if (turn == -1) {
            angle -= CIRCULAR_SPEED;
        }
        else if (turn == 1) {
            angle += CIRCULAR_SPEED;
        }
        currentX += LINEAR_SPEED * cos(angle);
        currentY += LINEAR_SPEED * sin(angle);
        if (draw && nowPlaying) {
            visited.emplace_back(Point(currentX, currentY));
        }
    }

 void Player::markGap() {
        if (nowPlaying) {
            const int size = visited.size();
            Point last = visited[size-1];
            visited[size-1] = Point(last.getX(), last.getY(), true);
        }
    }

 double Player::getCurrentX() {
        return currentX;
    }

 double Player::getCurrentY() {
        return currentY;
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

 vector<double> Player::getColor() {
        //return color;
    }

 bool Player::isNowPlaying() {
        return nowPlaying;
    }

Point Player::setCurPoint(double x, double y){
    cur_point = Point(x,y);
}

void Player::setNowPlaying(bool nowPlaying) {
        this->nowPlaying = nowPlaying;
    }


#pragma clang diagnostic pop