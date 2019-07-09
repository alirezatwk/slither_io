//
// Created by tavak on 7/9/19.
//

#include "Client.h"


Client::Client(const int inGameId, const int gameId, Direction lastDirection, const std::deque<Cell &> &cells)
        : inGameId(inGameId), gameId(gameId), lastDirection(lastDirection),
          cells(cells), alive(true), remainBenefits(0) {}
