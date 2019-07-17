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
    ServerGameController(int id, std::vector<Client *> &clientsArg);

    int getId() const;

    void setId(int id);

    void getGameState(response::GameState *gameState);

    void runCycle();

    bool actionSubmit(int inGameId, Direction direction);

    void getCycleNumber(response::PendingGameCycle *pendingGameCycle);

    void makePlayerFromClient(types::Player *player, Client *client);

    void makePlayerBlockFromCell(types::PlayerBlock *playerBlock, Cell *cell);

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
