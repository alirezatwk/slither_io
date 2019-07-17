#include "ServerGameController.h"


ServerGameController::ServerGameController(int id, std::vector<Client *> &clientsArg) : id(id),
                                                                                        cycleNumber(1),
                                                                                        map(15, 8, 1000),
                                                                                        clientsSaid(0),
                                                                                        dead(0) {
    for (auto client : clientsArg) {
        clients.push_back(client);
        directions.push_back(Direction::NONE);
    }
}

int ServerGameController::getId() const {
    return id;
}

void ServerGameController::setId(int id) {
    ServerGameController::id = id;
}

void ServerGameController::getCycleNumber(response::PendingGameCycle *pendingGameCycle) {
    pendingGameCycle->set_cycle_number(static_cast<google::protobuf::uint32>(cycleNumber));

    if (clientsSaid == static_cast<int>(clients.size())) {
        // TODO Maybe need a little delay.
        runCycle();
    }
}

void ServerGameController::getGameState(response::GameState *gameState) {
    // TODO Portal
    for (int i = 0; i < map.getDimension(); i++) {
        for (int j = 0; j < map.getDimension(); j++) {
            auto cell = map.getCell(i, j);
            types::Block *block = nullptr;
            if (cell->isWall()) {
                auto wall = gameState->add_walls();
                block = wall->mutable_position();

            } else if (cell->getFoodBenefit() != 0) {
                auto food = gameState->add_foods();
                block = food->mutable_position();
                food->set_benefit(static_cast<google::protobuf::uint32>(cell->getFoodBenefit()));
            }
            if (block != nullptr) {
                block->set_x(static_cast<google::protobuf::uint32>(i));
                block->set_y(static_cast<google::protobuf::uint32>(j));
            }
        }
    }
    for (auto client : clients) {
        auto player = gameState->add_players();
        makePlayerFromClient(player, client);
    }
}

void ServerGameController::runCycle() {


}

bool ServerGameController::actionSubmit(int inGameId, Direction direction) {
    if (directions[inGameId] == direction) {
        clientsSaid++;
        directions[inGameId] = direction;
        return true;
    }
    return false;
}

void ServerGameController::makePlayerFromClient(types::Player *player, Client *client) {
    player->set_name(client->getName());

    bool alive = client->isAlive();
    player->set_is_alive(alive);
    if (alive) {
        auto head = player->mutable_head();
        makePlayerBlockFromCell(head, client->getcell(0));
        for (int i = 1; i < client->getLenght(); i++) {
            auto playerBlock = player->add_blocks();
            makePlayerBlockFromCell(playerBlock, client->getcell(i));
        }
    }
}

void ServerGameController::makePlayerBlockFromCell(types::PlayerBlock *playerBlock, Cell *cell) {
    auto block = playerBlock->mutable_position();
    block->set_x(static_cast<google::protobuf::uint32>(cell->getX()));
    block->set_y(static_cast<google::protobuf::uint32>(cell->getY()));
}
