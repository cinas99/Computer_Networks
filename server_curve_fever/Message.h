#ifndef SERVER_CURVE_FEVER_MESSAGE_H
#define SERVER_CURVE_FEVER_MESSAGE_H

#define QUEUE_SIZE 100
#define BUF_SIZE 1024
#define TCP_PORT 9022
#define UDP_PORT 9023

enum Message{
    UNJOIN,
    JOIN,
    TURN_ON,
    LEAVE,
    UNREADY,
    READY,
    START,
    ROOM_EVENT,
    UDP_CONNECT,
    TRUE,
    FALSE
};

#endif //SERVER_CURVE_FEVER_MESSAGE_H