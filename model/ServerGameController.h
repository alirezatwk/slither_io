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
    ServerGameController(int id);

    Client *addClient(ServerUser *serverUser);

    int getId() const;

    void setId(int id);

    void getGameState(response::GameState *gameState);

    void runCycle();

    bool actionSubmit(int inGameId, Direction direction);

    void getCycleNumber(response::PendingGameCycle *pendingGameCycle);

    void makePlayerFromClient(types::Player *player, Client *client);

    void makePlayerBlockFromCell(types::PlayerBlock *playerBlock, Cell *cell);

    std::vector<Client *> clientsGoesInCell(Cell *cell);

    void move(Client *client);

    bool isEmpty(Cell *cell);

private:
    int id;

    Map map;
    int cycleNumber;

    int clientsSaid;
    int numberOfDeads; // for scoring

    std::vector<Client *> clients;

    std::vector<Direction> directions;


};


#endif //SLITHER_IO_SERVERGAMECONTROLLER_H
