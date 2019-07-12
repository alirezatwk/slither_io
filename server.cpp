#include <iostream>
#include <sstream>
#include <map>
#include <random>
#include <fstream>

#include <SFML/Network.hpp>
#include "proto/communication.pb.h"
#include "proto/database.pb.h"
#include "proto/response.pb.h"
#include "proto/types.pb.h"
#include "proto/request.pb.h"

#include "model/ServerQueue.h"
#include "model/ServerUser.h"

const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");
const std::string DATABASE("database.txt");

std::vector<ServerQueue *> queues;
std::vector<ServerUser *> users;

std::map<int, sf::Thread *> threadId;
std::map<int, sf::TcpSocket *> socketId;

bool quit = false;

sf::Mutex globalMutex;

std::string hashFunction(std::string password) {
    return password;
} // TODO

bool checkUser(const std::string &username, const std::string &password) {
    const std::string &hashPassword = hashFunction(password);

    std::ifstream fIn;
    fIn.open(DATABASE);

    database::User user;

    std::string userStr;
    while (fIn) {
        fIn >> userStr;
        user.ParseFromString(userStr);
        if (username == user.username() && hashPassword == user.password()) {
            fIn.close();
            return true;
        }
    }
    fIn.close();
    return false;
}

bool hadUsername(const std::string &username) {
    std::ifstream fIn;
    fIn.open(DATABASE);

    database::User user;

    std::string userStr;
    while (fIn) {
        fIn >> userStr;
        user.ParseFromString(userStr);
        if (username == user.username()) {
            fIn.close();
            return true;
        }
    }
    fIn.close();
    return false;
}

void addUserToUsers(const std::string &username, const int sessionId) {
    std::ifstream fIn;
    fIn.open(DATABASE);

    database::User user;

    std::string userStr;
    while (fIn) {
        fIn >> userStr;
        user.ParseFromString(userStr);
        if (username == user.username()) {
            auto newUser = new ServerUser(static_cast<int>(user.id()), user.name(), user.username(),
                                          user.password(), static_cast<int>(user.score()), sessionId, -1);
            users.push_back(newUser);
            fIn.close();
            return;
        }
    }
}

bool makeUser(const std::string &name, const std::string &username, const std::string &password) {
    if (hadUsername(username))
        return false;
    database::User newUser;
    newUser.set_id(1);
    newUser.set_session_id(0);
    newUser.set_name(name);
    newUser.set_username(username);
    newUser.set_password(hashFunction(password));
    newUser.set_score(0);
    newUser.set_game_id(0);
    newUser.set_queue_id(0);

    std::string str;
    newUser.SerializeToString(&str);

    std::ofstream fOut;
    fOut.open(DATABASE, std::ios_base::app);
    fOut << str;
    fOut.close();
    return true;
}

int generateSessionId() {
    return rand();
}


void Response(int id) {
    sf::TcpSocket *socket = socketId[id];


    sf::Packet packetReceive;
    socket->receive(packetReceive);

//    globalMutex.lock();
    std::string reqStr;
    packetReceive >> reqStr;

    Request req;
    req.ParseFromString(reqStr);

    Response
    res;

    if (req.has_login()) {
        const request::Login &reqLogin = req.login();

        // Ba farz inke seda kardan e tabe dige to ye threadi gand nemizane.
        if (checkUser(reqLogin.username(), reqLogin.password())) {
            int sessionId = generateSessionId();

            addUserToUsers(reqLogin.username(), sessionId);

            auto resLogin = new response::Login;
            resLogin->set_session_id((uint) sessionId);
            res.set_allocated_login(resLogin); // TODO Nemidonam
        }
        // TODO else
    }
    if (req.has_register_()) {
        const request::Register &reqRegister = req.register_();
        auto resRegister = new response::Register;
        resRegister->set_success(makeUser(reqRegister.name(), reqRegister.username(), reqRegister.password()));
    }
    if (req.has_user_info()) {
        const request::UserInfo &reqUserInfo = req.user_info();

    }

    sf::Packet packetSend;


    std::stringstream stream;
    msg.SerializeToOstream(&stream);

    packetSend << stream.str();


    socket->send(packetSend);

    // Ba farze inke beshe hamaro delete kard.
    delete socketId[id];
    socketId.erase(id);
    sf::Thread *tmp = threadId[id];
    threadId.erase(id);
    delete tmp;
}

int main(int argc, char *argv[]) {

    srand(time(0));

    sf::TcpListener listener;
    listener.listen(PORT);

    int id = 0;
    while (!quit) {
        auto *socket = new sf::TcpSocket;
        listener.accept(*socket);
        auto *thread = new sf::Thread(&Response, id);
        socketId[id] = socket;
        threadId[id] = thread;
        thread->launch();
    }

    return 0;
}