#include <string>

#ifndef SLITHER_IO_SERVERUSER_H
#define SLITHER_IO_SERVERUSER_H


class ServerUser {
public:

    ServerUser(int id, const std::string &name, const std::string &username, const std::string &password,
               int score, int sessionId, int queueId);

private:
    const int id;

    std::string name;

    const std::string username;
    const std::string password;

    int score;

    const int sessionId;
    int queueId;
};


#endif //SLITHER_IO_SERVERUSER_H
