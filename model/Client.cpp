//
// Created by tavak on 7/9/19.
//

#include "Client.h"


Client::Client(ServerUser *serverUser, int gameId, int inGameId,
               std::deque<Cell *> *cells) : ServerUser(serverUser),
                                            inGameId(inGameId),
                                            gameId(gameId),
                                            cells(cells) {}

Client::Client(int id, std::string &name, std::string &username, std::string &password, int score, int sessionId,
               int queueId, const int inGameId, const int gameId,
               std::deque<Cell *> *cells) : inGameId(inGameId), gameId(gameId),
                                                  cells(cells), alive(true), remainBenefits(0),
                                                  ServerUser(id, name, username, password, score, sessionId,
                                                             queueId, true) {}

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
    return static_cast<int>(cells->size());
}

Cell *Client::getcell(int i) const {
    return (*cells)[i];
}

void Client::addCell(Cell *cell) {
    cells->push_front(cell);
}

void Client::removeLastCell() {
    cells->pop_back();
}

const int Client::getGameId() const {
    return gameId;
}

int Client::getInGameId() const {
    return inGameId;
}

void Client::setInGameId(int inGameId) {
    Client::inGameId = inGameId;
}

