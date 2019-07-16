#include <vector>
#include "Cell.h"
#include "Client.h"

#ifndef SLITHER_IO_MAP_H
#define SLITHER_IO_MAP_H


class Map {
public:
    Map(int dimension, int maxFood, int prWall);

    virtual ~Map();

    void addClientToCell(Client *client, Cell *cell);

    std::vector<Cell *> *placeOfNewClient();

    bool goodPositionForNew(int x, int y);

    Cell *getCell(int x, int y);

    const int getDimension() const;



private:
    const int dimension;
    const int maxFood;
    const int prWall;


    int food;
    std::vector<std::vector<Cell *> > cells;
};


#endif //SLITHER_IO_MAP_H
