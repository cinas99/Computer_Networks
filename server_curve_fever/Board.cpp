#include "Player.h"
#include "Board.h"
#include <math.h>
#include <random>
#include <thread>

#define PI 3.1415926536

Board::Board() {
    this->currentNumberOfPlayers = 0;
    srand(time(NULL));
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
        std::vector <PointWrapper> newPoints;
        for (int i=0; i<currentNumberOfPlayers; i++) {
            if (player[i]->isNowPlaying()) {
                Point point = player[i]->generateNextLine();
                newPoints.emplace_back(PointWrapper(i, player[i]->getVisitedSize() - 1, point));
            }
        }
        sendPoints(newPoints);
        checkCollision();
        std::this_thread::sleep_for(std::chrono::milliseconds(KEYFRAME_DURATION_TIME));
    }
    std::cout << "Board: generating lines is finished!" << std::endl;
}

void Board::sendPoints(std::vector <PointWrapper> newPoints) {
    for (std::vector<Player *>::iterator it = player.begin(); it != player.end(); ++it) {
        for (std::vector<PointWrapper>::iterator point = newPoints.begin(); point != newPoints.end(); ++point) {
            string msg = prepareMessage(point->getPlayerNumber(), point->getPointNumber(), point->getPoint());
            (*it)->getUdpQueue()->push(msg);
        }
    }
}

std::string Board::prepareMessage(int playerNumber, int pointNumber, Point point) {
    std::string strPlayerNumber = std::to_string(playerNumber);
    std::string strPointNumber = std::to_string(pointNumber);
    std::string strX = std::to_string(point.getX());
    std::string strY = std::to_string(point.getY());
    std::string strIsGap = point.isGap() ? "1" : "0";
    return strPlayerNumber + "," + strPointNumber + "," + strX + "," + strY + "," + strIsGap + ",";
}

void Board::start(std::vector <Player*> player) {
    this->currentNumberOfPlayers++;
    int size = player.size();
    if (this->currentNumberOfPlayers == size) {
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
    std::vector <PointWrapper> newPoints;
    if (maxNumberOfPlayers == 1) {
        Point point = player[0]->init(WIDTH / 2.0, HEIGHT / 2.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(0, player[0]->getVisitedSize() - 1, point));
    }
    else if (maxNumberOfPlayers == 2) {
        Point point0 = player[0]->init(WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(0, player[0]->getVisitedSize() - 1, point0));

        Point point1 = player[1]->init(2 * WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(1, player[1]->getVisitedSize() - 1, point1));
    }
    else if (maxNumberOfPlayers == 3) {
        Point point0 = player[0]->init(WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(0, player[0]->getVisitedSize() - 1, point0));

        Point point1 = player[1]->init(2 * WIDTH / 3.0, HEIGHT / 2.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(1, player[1]->getVisitedSize() - 1, point1));

        Point point2 = player[2]->init(WIDTH / 2.0, 2 * HEIGHT / 3.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(2, player[2]->getVisitedSize() - 1, point2));
    }
    else if (maxNumberOfPlayers == 4) {
        Point point0 = player[0]->init(WIDTH / 3.0, HEIGHT / 3.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(0, player[0]->getVisitedSize() - 1, point0));

        Point point1 = player[1]->init(2 * WIDTH / 3.0, HEIGHT / 3.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(1, player[1]->getVisitedSize() - 1, point1));

        Point point2 = player[2]->init(WIDTH / 3.0, 2 * HEIGHT / 3.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(2, player[2]->getVisitedSize() - 1, point2));

        Point point3 = player[3]->init(2 * WIDTH / 3.0, 2 * HEIGHT / 3.0, random() * 2 * PI);
        newPoints.emplace_back(PointWrapper(3, player[3]->getVisitedSize() - 1, point3));
    }
    sendPoints(newPoints);
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

bool Board::outOfBounds(Point p) {
    return (p.getX() > WIDTH || p.getX() < 0 || p.getY() > HEIGHT || p.getY() < 0);
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