#include "PrepareGame.h"
//#include "main.cpp"

PrepareGame::PrepareGame() {}

void PrepareGame::addPlayer(){
    number_of_players += 1;
}

int PrepareGame::checkPlayersNumber(){
    return number_of_players;
}