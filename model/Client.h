#include <queue>
#include "Cell.h"
#include "Direction.h"

#ifndef SLITHER_IO_CLIENT_H
#define SLITHER_IO_CLIENT_H

// TODO inherit User

class Client {
public:
    Client(const int inGameId, const int gameId, Direction lastDirection, const std::queue<Cell &> &cells);

    bool isAlive() const;

    void setAlive(bool alive);

    int getRemainBenefits() const;

    void setRemainBenefits(int remainBenefits);

    int getLenght() const;

    Cell &getcell(int i) const;

    void addCell(const Cell &cell);

    void removeLastCell();

private:
    const int inGameId;
    const int gameId;

    bool alive;
    int remainBenefits;

    Direction lastDirection;

    std::queue<Cell &> cells;
};


#endif //SLITHER_IO_CLIENT_H
