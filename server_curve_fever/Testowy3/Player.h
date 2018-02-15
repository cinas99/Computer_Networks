//
// Created by marcin on 13.02.18.
//

#ifndef TESTOWY3_PREPARESERVICE_H
#define TESTOWY3_PREPARESERVICE_H

// serviceCategory = <1..4>

#include <netinet/in.h>
#include <string.h>
#include <array>

using namespace std;

class Player {
private:
    string nick;
    bool ready;
    bool started;
    sockaddr_in address;
public:
    Player();
    string getNick();
};

#endif //TESTOWY3_PREPARESERVICE_H
