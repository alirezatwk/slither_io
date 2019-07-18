#include <string>
#include <deque>
#include "Cell.h"
#include "Direction.h"
#include "ServerUser.h"

#ifndef SLITHER_IO_CLIENT_H
#define SLITHER_IO_CLIENT_H


class Client : public ServerUser {
public:
    Client(ServerUser *serverUser, int gameId, int inGameId, std::deque<Cell *> cells);
    Client(int id, std::string &name, std::string &username, std::string &password, int score, int sessionId,
           int queueId, int inGameId, int gameId, std::deque<Cell *> cells);

    bool isAlive() const;

    void setAlive(bool alive);

    int getRemainBenefits() const;

    void setRemainBenefits(int remainBenefits);

    int getLength() const;

    Cell *getCell(int i) const;

    void addCell(Cell *cell);

    void removeLastCell();

    const int getGameId() const;

    int getInGameId() const;

    void setInGameId(int inGameId);

private:
    int gameId;

    int inGameId;

    bool alive;
    int remainBenefits;

    std::deque<Cell *> cells;
};


#endif //SLITHER_IO_CLIENT_H
