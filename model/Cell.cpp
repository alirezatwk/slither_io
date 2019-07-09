#include "Cell.h"

Cell::Cell(const int x, const int y, const bool wall, int foodBenefit, int clientInGameId,
           const bool portal, const int portalX = 0, const int portalY = 0) : x(x),
                                                                              y(y),
                                                                              wall(wall),
                                                                              portal(portal),
                                                                              portalX(portalX),
                                                                              portalY(portalY),
                                                                              foodBenefit(
                                                                                      foodBenefit),
                                                                              clientInGameId(
                                                                                      clientInGameId) {}

Cell::~Cell() {}

int Cell::getFoodBenefit() const {
    return foodBenefit;
}

void Cell::setFoodBenefit(int foodBenefit) {
    Cell::foodBenefit = foodBenefit;
}

int Cell::getClientInGameId() const {
    return clientInGameId;
}

void Cell::setClientInGameId(int clientInGameId) {
    Cell::clientInGameId = clientInGameId;
}

const int Cell::getX() const {
    return x;
}

const int Cell::getY() const {
    return y;
}

const bool Cell::isWall() const {
    return wall;
}

const bool Cell::isPortal() const {
    return portal;
}

const int Cell::getPortalX() const {
    return portalX;
}

const int Cell::getPortalY() const {
    return portalY;
}
