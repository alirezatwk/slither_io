#include <random>
#include "Map.h"


Map::Map(int dimension, int maxFood, int prWall) : dimension(dimension), maxFood(maxFood), prWall(prWall) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < dimension; i++) {
        std::vector<Cell &> row;
        for (int j = 0; j < dimension; j++) {
            int pr = rand();
            Cell *cell = new Cell(i, j, pr <= prWall, 0, 0, false, 0, 0);
            row.push_back(cell);
        }
        cells.push_back(row);
    }
}

Map::~Map() {
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            delete cells[i][j];
        }
    }
}

void Map::addClientToCell(Client *client, Cell *cell) {
    client->addCell(*cell);
    cell->setClientInGameId(client->getInGameId());
}
