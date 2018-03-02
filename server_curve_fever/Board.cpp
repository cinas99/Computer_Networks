#include "Player.h"
#include "Board.h"
#include <math.h>
#include <iostream>
#include <chrono>
#include <random>
#include <bits/unique_ptr.h>
#include <unistd.h>
#include <thread>
//#include <synchapi.h>

#define PI 3.14

Board::Board() {
    this->currentNumberOfPlayers = 0;
    //colors[0] = "0x000000";
    //colors[1] = "0x00005F";
    //colors[2] = "0x00008F";
    //colors[3] = "0x0000FF";
}

void Board::threadDrawing() {
    std::this_thread::sleep_for(std::chrono::milliseconds(START_DRAWING_DELAY));
    while (run) {
        startDrawing();
        const int stopTime = round(MIN_TIME_OF_DRAWING + random() * (MAX_TIME_OF_DRAWING - MIN_TIME_OF_DRAWING));
        std::this_thread::sleep_for(std::chrono::milliseconds(stopTime));
        if (run) {
            stopDrawing();
            const int startTime = round(MIN_TIME_OF_DELAY + random() * (MAX_TIME_OF_DELAY - MIN_TIME_OF_DELAY));
            std::this_thread::sleep_for(std::chrono::milliseconds(startTime));
        }
        else
            break;
    }
    std::cout << "Board: drawing is finished!" << std::endl;
}

void Board::threadGenerateLines() {
    while (run) {
        for (int i=0; i<currentNumberOfPlayers; i++) {
            player[i]->generateNextLine();
        }
        checkCollision();
        std::this_thread::sleep_for(std::chrono::milliseconds(KEYFRAME_DURATION_TIME));
    }
    std::cout << "Board: generating lines is finished!" << std::endl;
}

void Board::start(std::vector <Player*> player) {
    this->currentNumberOfPlayers++;
    int size = player.size();
    if (this->currentNumberOfPlayers == size) {
        srand(time(NULL));
        this->run = true;
        this->player = player;
        this->numberOfPlayers = player.size();

        initPlayers(size);
        thread gapGenerator(&Board::threadDrawing, this);
        gapGenerator.detach();
        thread lineGenerator(&Board::threadGenerateLines, this);
        lineGenerator.detach();
        std::cout << "Board:start() is running!" << std::endl << std::endl;
    }
}

void Board::initPlayers(const int maxNumberOfPlayers) {
    if (maxNumberOfPlayers == 1) {
        player[0]->init(WIDTH / 2.0, HEIGHT / 2.0, random() * 2 * PI);
    } else if (maxNumberOfPlayers == 2) {
        player[0]->init(WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        player[1]->init(2 * WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
    } else if (maxNumberOfPlayers == 3) {
        player[0]->init(WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        player[1]->init(2 * WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        player[2]->init(WIDTH / 2.0, 2 * HEIGHT / 3.0, random() * 2 * PI);
    } else if (maxNumberOfPlayers == 4) {
        player[0]->init(WIDTH / 3.0, HEIGHT / 3.0, random() * 2 * PI);
        player[1]->init(2 * WIDTH / 3.0, HEIGHT / 3.0, random() * 2 * PI);
        player[2]->init(WIDTH / 3.0, 2 * HEIGHT / 3.0, random() * 2 * PI);
        player[3]->init(2 * WIDTH / 3.0, 2 * HEIGHT / 3.0, random() * 2 * PI);
    }
}

double Board::random() {
    return static_cast <double> (std::rand()) / static_cast <double> (RAND_MAX);
}

void Board::startDrawing() {
    for (Player *p : player) {
        p->setDraw(true);
    }
}

void Board::stopDrawing() {
    for (Player *p : player) {
        p->markGap();
        p->setDraw(false);
    }
}

/*void Board::start() {
    while(checkStillPlaying()) {

        while(checkStillTiming()){
            for (int i = 0; i < tmp_num_players; i++) {
                player[i].generateNextLine();
            }
            checkCollision();
            if (currentNumberOfPlayers <= 1)
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

/*void Board::TimerTask(int interval, bool executor) {
    interval = 0;
    sleep(10);
    while (executor) {
        interval += 1;
        if (interval > 500) {
            executor = checkStillTiming();
            interval = 0;
        }
    }
}*/

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

/*int Board::getNumberOfPlayers() {
    return numberOfPlayers;
}*/

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
    //const int NUMBER_OF_PLAYERS = player.size();
    for (int i = 0; i < numberOfPlayers; i++) {
        if (!player[i]->isNowPlaying()) continue;
        const vector<Point> iVisited = player[i]->getVisited();
        if (iVisited.size() > 1) {
            Point last = iVisited.at(iVisited.size() - 1);
            Point nextToLast = iVisited.at(iVisited.size() - 2);
            for (int j = 0; j < numberOfPlayers; j++) {
                const vector<Point> jVisited = player[j]->getVisited();
                const int sizeToCheck = (i == j) ? jVisited.size() - 3 : jVisited.size() - 1;
                for (int k = 0; k < sizeToCheck; k++) {
                    Point p1 = jVisited.at(k);
                    Point p2 = jVisited.at(k + 1);
                    if (!p1.isGap() && areIntersecting(p1, p2, nextToLast, last)) {
                        player[i]->setNowPlaying(false);
                        if (--currentNumberOfPlayers <= 1) {
                            showResults();
                            return;
                        }
                    }
                }
            }
        }
    }

    // to know number of pts to check
    /*int mySize = 0;
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        mySize += player[i]->getVisited().size();
    }
    cout << mySize;*/
}


//ShowResults - przemyslec i gdzies dokomponowac
// ? w Serwerze //TODO LATER //

void Board::showResults() {
    int winnerIndex;
    if (numberOfPlayers == 1) {
        winnerIndex = 0;
    }
    else {
        //winnerIndex = -1;
        for (int i=0; i<numberOfPlayers; i++) {
            if (player[i]->isNowPlaying())
                winnerIndex = i;
        }
    }
    this->run = false;
    std::cout << "Winner is player " << winnerIndex << std::endl;
    // stop timeline
    // send info about winner
}

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