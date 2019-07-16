#include <vector>
#include "ServerUser.h"

#ifndef SLITHER_IO_SERVERQUEUE_H
#define SLITHER_IO_SERVERQUEUE_H


class ServerQueue {
public:
    ServerQueue(int id, int MAXSIZE);

    int getSize() const;

    void setSize(int size);

    const int getId() const;

    const int getMAXSIZE() const;

    ServerUser &getUser(int index) const;

    bool addUser(ServerUser &user);

    bool isFull() const;

private:
    const int id;
    const int MAXSIZE;

    int size;
    std::vector<ServerUser &> users;
};


#endif //SLITHER_IO_SERVERQUEUE_H
