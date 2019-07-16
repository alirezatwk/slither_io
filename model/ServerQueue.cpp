#include "ServerQueue.h"

ServerQueue::ServerQueue(const int id, const int MAXSIZE) : id(id), MAXSIZE(MAXSIZE), size(0) {
    users.resize(0);
}

int ServerQueue::getSize() const {
    return size;
}

void ServerQueue::setSize(int size) {
    ServerQueue::size = size;
}

const int ServerQueue::getId() const {
    return id;
}

const int ServerQueue::getMAXSIZE() const {
    return MAXSIZE;
}

ServerUser &ServerQueue::getUser(int index) const {
    return users[index];
}

bool ServerQueue::isFull() const {
    return size == MAXSIZE;
}

bool ServerQueue::addUser(ServerUser &user) {
    if (isFull())
        return false;
    users.push_back(user);
    setSize(static_cast<int>(users.size()))
    return true;
}


