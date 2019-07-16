#include <string>
#include <queue>
#include "Direction.h"
#include "Cell.h"

#ifndef SLITHER_IO_SERVERUSER_H
#define SLITHER_IO_SERVERUSER_H


class ServerUser {
public:

    explicit ServerUser(ServerUser *serverUser);

    ServerUser(int id, const std::string &name, const std::string &username, const std::string &password,
               int score, int sessionId, int queueId, bool inGame);

    virtual ~ServerUser();

    void setScore(int score);

    void setQueueId(int queueId);

    void setSessionId(int sessionId);

    const int getId() const;

    const std::string &getName() const;

    const std::string &getUsername() const;

    const std::string &getPassword() const;

    int getScore() const;

    int getSessionId() const;

    int getQueueId() const;

    bool isInGame() const;

    void setInGame(bool inGame);

private:
    const int id;

    std::string name;

    const std::string username;
    const std::string password;

    int score;

    int sessionId;
    int queueId;

    bool inGame;

};


#endif //SLITHER_IO_SERVERUSER_H
