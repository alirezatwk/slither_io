#include <vector>
#include "Cell.h"

#ifndef SLITHER_IO_MAP_H
#define SLITHER_IO_MAP_H


class Map {
public:
    Map(const int dimension, const int maxFood, const double prWall);

private:
    const int dimension;
    const int maxFood;
    const double prWall;

    int food;
    std::vector<std::vector<Cell &> > cells;
};


#endif //SLITHER_IO_MAP_H
