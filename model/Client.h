#include <deque>
#include "Cell.h"
#include "Direction.h"

#ifndef SLITHER_IO_CLIENT_H
#define SLITHER_IO_CLIENT_H

// TODO inherit User

class Client {
public:
    Client(const int inGameId, const int gameId, Direction lastDirection, const std::deque<Cell &> &cells);


private:
    const int inGameId;
    const int gameId;

    bool alive;
    int remainBenefits;

    Direction lastDirection;

    std::deque<Cell &> cells;
};


#endif //SLITHER_IO_CLIENT_H
