//
// Created by tavak on 7/10/19.
//

#include "ServerUser.h"


ServerUser::ServerUser(int id, const std::string &name, const std::string &username, const std::string &password,
                       int score, int sessionId, int queueId) : id(id), name(name), username(username),
                                                                      password(password), score(score),
                                                                      sessionId(sessionId), queueId(queueId) {}
