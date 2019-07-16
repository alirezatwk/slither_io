//
// Created by tavak on 7/14/19.
//

#include "ServerGameController.h"


ServerGameController::ServerGameController(int id, std::vector<Client *> &clients) : id(id),
                                                                                     cycleNumber(1),
                                                                                     map(15, 8, 1000),
                                                                                     clients(clients),
                                                                                     clientsSaid(0),
                                                                                     dead(0) {
    for (int i = 0; i < clients.size(); i++) {
        directions.push_back(Direction::NONE);
    }
}

int ServerGameController::getId() const {
    return id;
}

void ServerGameController::setId(int id) {
    ServerGameController::id = id;
}

response::PendingGameCycle ServerGameController::getCycleNumber() {
    response::PendingGameCycle ans;
    ans.set_cycle_number(static_cast<google::protobuf::uint32>(cycleNumber));

    if (clientsSaid == static_cast<int>(clients.size())) {
        // TODO Maybe need a little delay.
        runCycle();
    }

    return ans;
}

response::GameState ServerGameController::getGameState() {
    response::GameState ans;

    // TODO ans.add_portal();
    for (int i = 0; i < map.getDimension(); i++) {
        for (int j = 0; j < map.getDimension(); j++) {
            Cell *cell = map.getCell(i, j);
            auto block = new types::Block;
            block->set_x(static_cast<google::protobuf::uint32>(i));
            block->set_y(static_cast<google::protobuf::uint32>(j));

            if (cell->isWall()) {
                auto wall = ans.add_walls();
                wall->set_allocated_position(block);
            } else if (cell->getFoodBenefit() != 0) {
                auto food = ans.add_foods();
                food->set_allocated_position(block);
                food->set_benefit(static_cast<google::protobuf::uint32>(cell->getFoodBenefit()));
            }
        }
    }

    for (int i = 0; i < clients.size(); i++) {

    }

    // TODO
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

