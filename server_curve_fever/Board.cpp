#include "Player.h"
#include "Board.h"
#include <math.h>
#include <iostream>
#include <chrono>
#include <random>
#include <bits/unique_ptr.h>
#include <unistd.h>
//#include <synchapi.h>

#define PI 3.14

Board::Board() {
    //colors[0] = "0x000000";
    //colors[1] = "0x00005F";
    //colors[2] = "0x00008F";
    //colors[3] = "0x0000FF";
}

void Board::start(int numberOfPlayers, TcpServer tcpServer, UdpServer udpServer) {
    this->numberOfPlayers = numberOfPlayers;
    this->tcpServer = tcpServer;
    this->udpServer = udpServer;
    srand(time(NULL));
    initPlayers(numberOfPlayers);
    //start thread
    std::cout << "Board:start() is running!" << std::endl;
    // init players
    // schedule startDrawing()
    // this.drawLines(player[i]) (client - ?)
    // add handle to timeline and start
}

void Board::initPlayers(const int maxNumberOfPlayers) {
    if (maxNumberOfPlayers == 1) {
        player.emplace_back(Player(WIDTH / 2.0, HEIGHT / 2.0, std::rand() * 2 * PI));//, colors[0]));
    } else if (maxNumberOfPlayers == 2) {
        player.emplace_back(Player(WIDTH / 3.0, HEIGHT / 2.0, std::rand() * 2 * PI));//, colors[0]));
        player.emplace_back(Player(2 * WIDTH / 3.0, HEIGHT / 2.0, std::rand() * 2 * PI));//, colors[1]));
    } else if (maxNumberOfPlayers == 3) {
        player.emplace_back(Player(WIDTH / 3.0, HEIGHT / 2.0, std::rand() * 2 * PI));//, colors[0]));
        player.emplace_back(Player(2 * WIDTH / 3.0, HEIGHT / 2.0, std::rand() * 2 * PI));//, colors[1]));
        player.emplace_back(Player(WIDTH / 2.0, 2 * HEIGHT / 3.0, std::rand() * 2 * PI));//, colors[2]));
    } else if (maxNumberOfPlayers == 4) {
        player.emplace_back(Player(WIDTH / 3.0, HEIGHT / 3.0, std::rand() * 2 * PI));//, colors[0]));
        player.emplace_back(Player(2 * WIDTH / 3.0, HEIGHT / 3.0, std::rand() * 2 * PI));//, colors[1]));
        player.emplace_back(Player(WIDTH / 3.0, 2 * HEIGHT / 3.0, std::rand() * 2 * PI));//, colors[2]));
        player.emplace_back(Player(2 * WIDTH / 3.0, 2 * HEIGHT / 3.0, std::rand() * 2 * PI));//, colors[3]));
    }
}

//Player Board::player[4];
//string Board::colors[4] = {"0x000000", "0x00005F", "0x00008F", "0x0000FF"};
//int tmp_num_players = Board::getNumberOfPlayers();
using namespace std::chrono;

/*Board::Board(int maxNumberOfPlayers) {
    initPlayers(Board::getNumberOfPlayers());
}*/

void Board::startDrawing() {
    for (Player p : player) {
        p.setDraw(true);
    }
    long double time = round(MIN_TIME_OF_DRAWING + rand() * (MAX_TIME_OF_DRAWING - MIN_TIME_OF_DRAWING));
    //timer.schedule ...
}

void Board::stopDrawing() {
    for (Player p : player) {
        p.markGap();
        p.setDraw(false);
    }
    long double time = round(MIN_TIME_OF_DELAY + rand() * (MAX_TIME_OF_DELAY - MIN_TIME_OF_DELAY));
    //timer.schedule ...
}

/*void Board::start() {
    while(checkStillPlaying()) {

        while(checkStillTiming()){
            for (int i = 0; i < tmp_num_players; i++) {
                player[i].generateNextLine();
            }
            checkCollision();
            if (numberOfPlayers <= 1)
                setStillPlaying(FALSE); // LAST PLAYER LEFT - GAME OVER

            if (!checkStillPlaying()) {
                cout << "Game over";  // JAK TO ZROBIC ZGODNIE Z SERVEREM ?
                break;
            }
            sleep(100); // CZAS KWANTOWANIA
            cout<<" Checking cycle \n";
        }
    }
    //return;
}*/

void Board::TimerTask(int interval, bool executor) {
    interval = 0;
    sleep(10);
    while (executor) {
        interval += 1;
        if (interval > 500) {
            executor = checkStillTiming();
            interval = 0;
        }
    }
}

bool Board::outOfBounds(Point p) {
    return (p.getX() > WIDTH || p.getX() < 0 || p.getY() > HEIGHT || p.getY() < 0);
}

bool Board::checkStillTiming() {
    return STILL_TIMING;
}

bool Board::checkStillPlaying() {
    return STILL_PLAYING;
}

void Board::setStillTiming(bool cond) {
    STILL_TIMING = cond;
}

void Board::setStillPlaying(bool cond) {
    STILL_PLAYING = cond;
}

int Board::getNumberOfPlayers() {
    return NUMBER_OF_PLAYERS;
}

bool Board::areIntersecting(Point p1, Point p2, Point q1, Point q2) {
    bool result = 0;
    // special case when player move out of map
    if (outOfBounds(q2)) {
        result = true;
    }
        // other cases when sections are intersecting
    else if (p1.getX() != p2.getX() && q1.getX() != q2.getX()) {
        // count a, b parameters for two lines
        double pa = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
        double pb = p1.getY() - pa * p1.getX();
        double qa = (q1.getY() - q2.getY()) / (q1.getX() - q2.getX());
        double qb = q1.getY() - qa * q1.getX();
        // point of intersection
        double x = (qb - pb) / (pa - qa);
        double y = pa * x + pb;
        // check if the point belongs to the sections
        bool pCondition = min(p1.getX(), p2.getX()) <= x && x <= max(p1.getX(), p2.getX())
                          && min(p1.getY(), p2.getY()) <= y && y <= max(p1.getY(), p2.getY());
        bool qCondition = min(q1.getX(), q2.getX()) <= x && x <= max(q1.getX(), q2.getX())
                          && min(q1.getY(), q2.getY()) <= y && y <= max(q1.getY(), q2.getY());
        result = pCondition && qCondition;
    } else if (p1.getX() == p2.getX() && q1.getX() == q2.getX()) {
        result = p1.getX() == q1.getX() && (min(p1.getY(), p2.getY()) <= max(q1.getY(), q2.getY())
                                            || min(q1.getY(), q2.getY()) <= max(p1.getY(), p2.getY()));
    } else if (p1.getX() == p2.getX()) {
        double qa = (q1.getY() - q2.getY()) / (q1.getX() - q2.getX());
        double qb = q1.getY() - qa * q1.getX();
        double x = p1.getX();
        double y = qa * x + qb;
        bool pCondition = min(p1.getY(), p2.getY()) <= x && x <= max(p1.getY(), p2.getY());
        bool qCondition = min(q1.getX(), q2.getX()) <= x && x <= max(q1.getX(), q2.getX())
                          && min(q1.getY(), q2.getY()) <= y && y <= max(q1.getY(), q2.getY());
        result = pCondition && qCondition;
    } else { // (q1.getX() == q2.getX())
        double pa = (p1.getY() - p2.getY()) / (p1.getX() - p2.getX());
        double pb = p1.getY() - pa * p1.getX();
        double x = q1.getX();
        double y = pa * q1.getX() + pb;
        bool pCondition = min(p1.getX(), p2.getX()) <= x && x <= max(p1.getX(), p2.getX())
                          && min(p1.getY(), p2.getY()) <= y && y <= max(p1.getY(), p2.getY());
        bool qCondition = min(q2.getX(), q2.getY()) <= x && x <= max(q2.getX(), q2.getY());
        result = pCondition && qCondition;
    }
    return result;
}

void Board::checkCollision() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!player[i].isNowPlaying()) continue;
        const vector<Point> iVisited = player[i].getVisited();
        if (iVisited.size() > 1) {
            Point last = iVisited.at(iVisited.size() - 1);
            Point nextToLast = iVisited.at(iVisited.size() - 2);
            for (int j = 0; j < NUMBER_OF_PLAYERS; j++) {
                const vector<Point> jVisited = player[j].getVisited();
                const int sizeToCheck = (i == j) ? jVisited.size() - 3 : jVisited.size() - 1;
                for (unsigned int k = 0; k < sizeToCheck; k++) {
                    Point p1 = jVisited.at(k);
                    Point p2 = jVisited.at(k + 1);
                    if (!p1.isGap() && areIntersecting(p1, p2, nextToLast, last)) {
                        player[i].setNowPlaying(false);
                        if (--numberOfPlayers <= 1) {
                            //this.showResults(); // dopiero bedzie zaimplementowana ! <na samym dole>
                            return;
                        }
                    }
                }
            }
        }
    }

    // to know number of pts to check
    int mySize = 0;
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        mySize += player[i].getVisited().size();
    }
    cout << mySize;
}


//ShowResults - przemyslec i gdzies dokomponowac
// ? w Serwerze //TODO LATER //

/*
void Board::showResults() {
    int winnerIndex = -1;
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (player[i].isNowPlaying()) {
            winnerIndex = i;
        }
    }
    Alert alert = new Alert(Alert.AlertType.INFORMATION); //alert TODO
    alert.setHeaderText(null);
    alert.setTitle(TITLE);
    alert.setContentText("Player " + (winnerIndex+1) + " won!");
    timeline.stop();
    alert.show();
}
*/