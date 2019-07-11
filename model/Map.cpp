#include <random>
#include "Map.h"


Map::Map(const int dimension, const int maxFood, const double prWall) : dimension(dimension), maxFood(maxFood),
                                                                        food(0), prWall(prWall) {
    srand((unsigned int)time(0));
    std::vector<Cell &> row;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            double pr = (double) random() / RAND_MAX;
            if (pr <= prWall)

        }
    }
}
