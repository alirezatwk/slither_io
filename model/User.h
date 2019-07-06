#include <string>
#ifndef SLITHER_IO_USER_H
#define SLITHER_IO_USER_H


class User {
public:


private:
    unsigned int id;
    std::string username;
    std::string password;

    std::string name;
    unsigned int score;

    unsigned int sessionId;

    unsigned int queueId;
};


#endif //SLITHER_IO_USER_H
