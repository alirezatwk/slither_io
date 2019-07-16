#include <vector>

#include <SFML/Network.hpp>
#include "Client.h"
#include "Map.h"
#include "Direction.h"
#include "ServerQueue.h"
#include "../proto/response.pb.h"

#ifndef SLITHER_IO_SERVERGAMECONTROLLER_H
#define SLITHER_IO_SERVERGAMECONTROLLER_H


class ServerGameController {

public:
    ServerGameController(int id, std::vector<Client *> &clients);

    int getId() const;

    void setId(int id);

    response::GameState getGameState();

    void runCycle();

    bool actionSubmit(int inGameId, Direction direction);

    response::PendingGameCycle getCycleNumber();


private:
    int id;

    Map map;
    int cycleNumber;

    int clientsSaid;
    int dead; // for scoring

    std::vector<Client *> clients;

    std::vector<Direction> directions;


};


#endif //SLITHER_IO_SERVERGAMECONTROLLER_H
