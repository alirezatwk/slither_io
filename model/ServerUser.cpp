#include <utility>

#include <utility>

#include <utility>

#include "ServerUser.h"


ServerUser::ServerUser(ServerUser *serverUser) : id(serverUser->getId()), name(serverUser->getName()),
                                                 username(serverUser->getUsername()),
                                                 password(serverUser->getPassword()),
                                                 sessionId(serverUser->getSessionId()),
                                                 queueId(serverUser->getQueueId()),
                                                 inGame(true),
                                                 login(true) {} // because we make new user with pointer
// only for clients.

ServerUser::ServerUser(int id, std::string name, std::string username, std::string password,
                       int score, int sessionId, int queueId, bool inGame) : id(id), name(std::move(name)), username(
        std::move(username)),
                                                                             password(std::move(password)),
                                                                             score(score),
                                                                             sessionId(sessionId), queueId(queueId),
                                                                             inGame(inGame), login(false) {}

void ServerUser::addScore(int score) {
    ServerUser::score += score;
}

void ServerUser::setQueueId(int queueId) {
    ServerUser::queueId = queueId;
}

const int ServerUser::getId() const {
    return id;
}

const std::string &ServerUser::getName() const {
    return name;
}

const std::string &ServerUser::getUsername() const {
    return username;
}

const std::string &ServerUser::getPassword() const {
    return password;
}

int ServerUser::getScore() const {
    return score;
}

int ServerUser::getSessionId() const {
    return sessionId;
}

int ServerUser::getQueueId() const {
    return queueId;
}

void ServerUser::setSessionId(int sessionId) {
    ServerUser::sessionId = sessionId;
}

bool ServerUser::isInGame() const {
    return inGame;
}

void ServerUser::setInGame(bool inGame) {
    ServerUser::inGame = inGame;
}

ServerUser::~ServerUser() {

}

bool ServerUser::isLogin() const {
    return login;
}

void ServerUser::setLogin(bool login) {
    ServerUser::login = login;
}
