#include <string>

#ifndef SLITHER_IO_SERVERUSER_H
#define SLITHER_IO_SERVERUSER_H


class ServerUser {
public:



private:
    const int id;

    std::string name;

    std::string username;
    std::string password;

    int score;

    const int sessionId;
    int queueId;
};


#endif //SLITHER_IO_SERVERUSER_H
