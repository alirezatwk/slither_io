#include <random>
#include "Map.h"


Map::Map(int dimension, int maxFood, int prWall) : dimension(dimension), maxFood(maxFood), prWall(prWall), food(0) {
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < dimension; i++) {
        std::vector<Cell *> row;
        for (int j = 0; j < dimension; j++) {
            int pr = rand();
            Cell *cell;
            if (pr <= prWall) {
                cell = new Cell(i, j, true, 0, -1, false, 0, 0);
            } else {
                cell = new Cell(i, j, false, 0, -1, false, 0, 0);
            }
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
    client->addCell(cell);
    cell->setClientInGameId(client->getInGameId());
}


std::deque<Cell *> Map::placeOfNewClient(int inGameId) {
    int x = rand() % (dimension - 1);
    int y = rand() % dimension;
    while (!goodPositionForNew(x, y) || !goodPositionForNew(x + 1, y)) {
        x = rand() % (dimension - 1);
        y = rand() % dimension;
    }
    std::deque<Cell *> ans;
    ans.push_front(getCell(x + 1, y));
    getCell(x + 1, y)->setClientInGameId(inGameId);
    ans.push_front(getCell(x, y));
    getCell(x, y)->setClientInGameId(inGameId);
    return ans;
}

Cell *Map::getCell(int x, int y) {
    return cells[x][y];
}

bool Map::goodPositionForNew(int x, int y) {
    Cell *cell = getCell(x, y);
    if (cell->isWall())
        return false;
    if (cell->isPortal())
        return false;
    if (cell->getFoodBenefit() != 0)
        return false;
    if (cell->getClientInGameId() != -1)
        return false;
    return true;
}

const int Map::getDimension() const {
    return dimension;
}

Cell *Map::getDirectedCell(Cell *currentCell, Direction direction) {
    int x = currentCell->getX();
    int y = currentCell->getY();
    int n = getDimension();

    if (direction == Direction::NONE) {
        return currentCell;
    } else if (direction == Direction::UP) {
        if (x == 0)
            return nullptr;
        return getCell(x - 1, y);
    } else if (direction == Direction::DOWN) {
        if (x == n - 1)
            return nullptr;
        return getCell(x + 1, y);
    } else if (direction == Direction::RIGHT) {
        if (y == n - 1)
            return nullptr;
        return getCell(x, y + 1);
    } else if (direction == Direction::LEFT) {
        if (y == 0)
            return nullptr;
        return getCell(x, y - 1);
    }
}

void Map::hearse(Client *client) {
    for (int i = 0; i < client->getLength(); i++) {
        auto cell = client->getCell(i);
        cell->setClientInGameId(-1);
    }
}

void Map::removeClientLastCell(Client *client) {
    auto cell = client->getCell(client->getLength() - 1);
    cell->setClientInGameId(-1);
}
