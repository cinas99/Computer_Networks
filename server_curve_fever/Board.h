#ifndef SERVER_CURVE_FEVER_BOARD_H
#define SERVER_CURVE_FEVER_BOARD_H

class Board {
private:
    Board() {}
    Board(Board const&);
    void operator=(Board const&);
public:
    static Board& getInstance() {
        static Board instance;
        return instance;
    }
};

#endif //SERVER_CURVE_FEVER_BOARD_H
