#include <vector>
#include <deque>

#include "Cell.h"
#include "Client.h"
#include "../proto/types.pb.h"

#ifndef SLITHER_IO_MAP_H
#define SLITHER_IO_MAP_H


class Map {
public:
    Map(int dimension, int maxFood, int prWall);

    virtual ~Map();

    void addClientToCell(Client *client, Cell *cell);

    void removeClientLastCell(Client *client);

    std::deque<Cell *> placeOfNewClient(int inGameId);

    bool goodPositionForNew(int x, int y);

    Cell *getCell(int x, int y);

    Cell *getDirectedCell(Cell *currentCell, Direction direction);

    const int getDimension() const;

    void hearse(Client *client);

private:
    const int dimension;
    const int maxFood;
    const int prWall;


    int food;
    std::vector<std::vector<Cell *> > cells;
};


#endif //SLITHER_IO_MAP_H
