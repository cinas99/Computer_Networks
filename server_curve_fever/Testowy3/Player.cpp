
#include "Player.h"

/*

1 potwierdzenie hs + liczba graczy + nicki graczy + odbior hs
2 potwierdzenie hs + sprawdzenie nicku + potwierdzenie przyjecia nicku
3 potwierdzenie hs (gotowosc k do rozpoczecia rozgrywki)
4 start - oczekiwanie na hs all klientow

 */


Player::Player(){

}

string Player::getNick() {
    return nick;
}
