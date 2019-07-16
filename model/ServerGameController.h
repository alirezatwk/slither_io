#include <vector>

#include "Client.h"
#include "Map.h"
#include "Direction.h"
#include "ServerQueue.h"


#ifndef SLITHER_IO_SERVERGAMECONTROLLER_H
#define SLITHER_IO_SERVERGAMECONTROLLER_H


class ServerGameController {

public:
    explicit ServerGameController(int id);

    void addClient(Client *client);

    void launch();

    void getClientDir(int clientInGameId, Direction direction);

private:
    int id;

    Map map;
    int cycleNumber;

    std::vector<Client *> clients;

    std::vector<Direction> directions;
};


#endif //SLITHER_IO_SERVERGAMECONTROLLER_H
