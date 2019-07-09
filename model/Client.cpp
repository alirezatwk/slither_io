//
// Created by tavak on 7/9/19.
//

#include "Client.h"


Client::Client(const int inGameId, const int gameId, Direction lastDirection, const std::queue<Cell &> &cells)
        : inGameId(inGameId), gameId(gameId), lastDirection(lastDirection),
          cells(cells), alive(true), remainBenefits(0) {}

bool Client::isAlive() const {
    return alive;
}

void Client::setAlive(bool alive) {
    Client::alive = alive;
}

int Client::getRemainBenefits() const {
    return remainBenefits;
}

void Client::setRemainBenefits(int remainBenefits) {
    Client::remainBenefits = remainBenefits;
}

int Client::getLenght() const {
    return static_cast<int>(cells.size());
}

Cell &Client::getcell(int i) const {
    return cells[getLenght() - i - 1];
}

void Client::addCell(const Cell &cell) {
    cells.push(cell);
}

void Client::removeLastCell() {
    cells.pop();
}