#include "Board.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

static const long MIN_TIME_OF_DRAWING = 4000;
void stopDrawing(const boost::system::error_code&);
using namespace std;

void generate(){
    cout<<"generateNextLine\n"; // printuje - chce miec info o wywolaniu
}

void checkCollision() {
    cout<<"CheckCollisions\n"; // printuje - chce miec info o wywolaniu
}

void startDrawing(const boost::system::error_code& /*e*/) {
    for  (int i = 0; i<5; i++) {
        cout<<"setTrue"<<i<<endl;
    }
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(4));
    t.async_wait(&stopDrawing);
    io.run();
}

void stopDrawing(const boost::system::error_code&) {

    for  (int i = 0; i<5; i++) {
        cout<<"setFalse"<<i<<endl;
    }
    boost::asio::io_service io;
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(4));
    t.async_wait(&stopDrawing);
    io.run();

}

void core(const boost::system::error_code& /*e*/)
{
    for (int i = 0; i < 4; i++) {
        generate();
    }
    checkCollision();
}

void startCycling(){
    int iteration = 1;
    while(1) {
            cout << "Iteracja nr:" << iteration << endl;

            boost::asio::io_service io;
            boost::asio::deadline_timer t(io, boost::posix_time::seconds(2));
            t.async_wait(&core);
            io.run();
            iteration++;
        }
}

void runDrawing(){
    boost::asio::io_service iox;
    boost::asio::deadline_timer t(iox, boost::posix_time::seconds(1));
    t.async_wait(&startDrawing);   // pierwsze wywolanie startDrawingg
    iox.run();
}

int main() {

    runDrawing();
    startCycling();

    /*Board board;
    board.runDrawing();
    board.startCycling();
    */
     }