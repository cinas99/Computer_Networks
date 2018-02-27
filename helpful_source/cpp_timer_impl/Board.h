#ifndef BOOST_TEST_BOARD_H
#define BOOST_TEST_BOARD_H

#include <iostream>
#include <vector>
#include <string.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class Board {
public:
    Board() {}
    Board(int maxNumberOfPlayers);
    static Board& getInstance() {
        static Board instance;
        return instance;
    }
    //void setServers(TcpServer tcpServer, UdpServer udpServer);
    //static bool outOfBounds(Point p);
    //static bool areIntersecting(Point p1, Point p2, Point q1, Point q2);
    void initPlayers(int maxNumberOfPlayers);
    static void coreGame(const boost::system::error_code&);
    static void startCycling();
    static void runDrawing();
    static void generate();
    static void checkCollision();
    static void startDrawing(const boost::system::error_code&);
    static void stopDrawing(const boost::system::error_code&);



};

#endif //BOOST_TEST_BOARD_H