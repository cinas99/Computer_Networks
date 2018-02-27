#include "Board.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

static const long MIN_TIME_OF_DRAWING = 4000;
void stopDrawingg(const boost::system::error_code&);
using namespace std;

void Board::generate(){
    cout<<"generateNextLine\n"; // printuje - chce miec info o wywolaniu
}

void Board::checkCollision() {
    cout<<"CheckCollisions\n"; // printuje - chce miec info o wywolaniu
}

void Board::startDrawing(const boost::system::error_code&) {

    for  (int i = 0; i<5; i++) {
        cout<<"setFalse"<<i<<endl;
    }
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(4));
    t.async_wait(&stopDrawingg);
    io.run();

}

void Board::stopDrawing(const boost::system::error_code& /*e*/) {
    for  (int i = 0; i<5; i++) {
        cout<<"setTrue"<<i<<endl;
    }
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(4));
    t.async_wait(&startDrawing);
    io.run();
}

void Board::coreGame(const boost::system::error_code& /*e*/)
{
    for (int i = 0; i < 4; i++) {
        Board::generate();
    }
    Board::checkCollision();
}

void Board::startCycling(){
    int iteration = 1;
    while(1) {
        cout << "Iteracja nr:" << iteration << endl;

        boost::asio::io_service io;
        boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
        t.async_wait(&coreGame);
        io.run();
        iteration++;
    }
}

void Board::runDrawing(){
    boost::asio::io_service iox;
    boost::asio::deadline_timer t(iox, boost::posix_time::seconds(1));
    t.async_wait(&startDrawing);   // pierwsze wywolanie startDrawingg
    iox.run();
}
