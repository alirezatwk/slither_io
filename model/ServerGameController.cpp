#include "ServerGameController.h"


ServerGameController::ServerGameController(int id, std::vector<Client *> &clientsArg) : id(id),
                                                                                        cycleNumber(1),
                                                                                        map(15, 8, 1000),
                                                                                        clientsSaid(0),
                                                                                        numberOfDeads(0) {
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

    std::vector<Client *> deads;

    for (int i = 0; i < map.getDimension(); i++) {
        for (int j = 0; j < map.getDimension(); j++) {
            auto cell = map.getCell(i, j);
            std::vector<Client *> goIn = clientsGoesInCell(cell);

            // Shakh be SHakha! :))
            if (isEmpty(cell)) {
                if (goIn.size() == 1) {
                    move(goIn[0]);
                } else if (goIn.size() == 2) {
                    Direction dir0 = directions[goIn[0]->getInGameId()];
                    Direction dir1 = directions[goIn[1]->getInGameId()];

                    // if, else if are estesna :))), else is halate koli
                    if (dir0 == Direction::UP && dir1 == Direction::LEFT) {
                        deads.push_back(goIn[1]);
                    } else if (dir0 == Direction::LEFT && dir1 == Direction::UP) {
                        deads.push_back(goIn[0]);
                    } else {
                        for (int k = 0; k < 4; k++) {
                            if (dir0 == k) {
                                deads.push_back(goIn[0]);
                                break;
                            } else if (dir1 == k) {
                                deads.push_back(goIn[1]);
                                break;
                            }
                        }
                    }
                } else if (goIn.size() == 3) {
                    for (int k = 0; k < 4; k++) {
                        bool has = false;
                        for (auto client : goIn) {
                            if (directions[client->getInGameId()] == k) {
                                has = true;
                                break;
                            }
                        }
                        if (!has) {
                            for (auto client : goIn) {
                                Direction clientDir = directions[client->getInGameId()];
                                if (clientDir != (k + 2) % 4) {
                                    deads.push_back(client);
                                }
                            }
                            break;
                        }
                    }
                } else if (goIn.size() == 4) {
                    for (auto client : clients) {
                        if (directions[client->getInGameId()] != Direction::DOWN) {
                            deads.push_back(client);
                        }
                    }
                }
            } else { // Cell Por bod
                for (auto client : goIn) {
                    deads.push_back(client);
                }
            }
        }
    }

    // beran biron az safe
    for (auto client : clients) {
        auto targetCell = map.getDirectedCell(client->getCell(0), directions[client->getInGameId()]);
        if (targetCell == nullptr) {
            deads.push_back(client);
        }
    }


    for (auto dead : deads) {
        map.hearse(dead);
        dead->setAlive(false);
        dead->addScore(numberOfDeads);
    }

    numberOfDeads += static_cast<int>(deads.size());
    for (auto client : clients) {
        if (client->isAlive()) {
            move(client);
        }
    }

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
        makePlayerBlockFromCell(head, client->getCell(0));
        for (int i = 1; i < client->getLength(); i++) {
            auto playerBlock = player->add_blocks();
            makePlayerBlockFromCell(playerBlock, client->getCell(i));
        }
    }
}

void ServerGameController::makePlayerBlockFromCell(types::PlayerBlock *playerBlock, Cell *cell) {
    auto block = playerBlock->mutable_position();
    block->set_x(static_cast<google::protobuf::uint32>(cell->getX()));
    block->set_y(static_cast<google::protobuf::uint32>(cell->getY()));
}

std::vector<Client *> ServerGameController::clientsGoesInCell(Cell *cell) {
    std::vector<Client *> ans;
    for (auto client : clients) {
        if (client->isAlive() && map.getDirectedCell(client->getCell(0), directions[client->getInGameId()]) == cell) {
            ans.push_back(client);
        }
    }
    return ans;
}

void ServerGameController::move(Client *client) {
    Cell *currentCell = client->getCell(0);
    int id = client->getInGameId();
    map.addClientToCell(client, map.getDirectedCell(currentCell, directions[id]));

    int remain = client->getRemainBenefits();
    if(1 <= remain){
        client->setRemainBenefits(remain - 1);
    } else {
        map.removeClientLastCell(client);
    }
}

bool ServerGameController::isEmpty(Cell *cell) {
    if (cell->isWall()) {
        return false;
    }
    if (cell->getClientInGameId() == -1){
        return true;
    }
    int id = cell->getClientInGameId();
    int length = clients[id]->getLength();
    Cell *lastCellClient = clients[id]->getCell(length - 1);
    if(lastCellClient == cell && clients[id]->getRemainBenefits() == 0){
        return true;
    }
    return false;
}

