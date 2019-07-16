//
// Created by tavak on 7/14/19.
//

#include "ServerGameController.h"


ServerGameController::ServerGameController(int id) : id(id), cycleNumber(1), map(15, 8, 1000) {


    launch();
}

void ServerGameController::addClient(Client *client) {
    clients.push_back(client);
}

void getClientDirection(int clientInGameId, Direction direction){

}

void launch() {

    while()
}