#include <iostream>
#include <sstream>
#include <map>
#include <random>

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

std::vector<ServerQueue *> queues;
std::vector<ServerUser *> users;

std::map<int, sf::Thread *> threadId;
std::map<int, sf::TcpSocket *> socketId;

bool quit = false;

sf::Mutex globalMutex;

std::string hashFunction(std::string password) {
    // TODO
    return password;
}

bool checkUser(std::string username, std::string hashPassword) {

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

    if (req.has_login()) {
        const request::Login &reqLogin = req.login();
        // Ba farz inke seda kardan e tabe dige to ye threadi gand nemizane.

        if (checkUser(reqLogin.username(), hashFunction(reqLogin.password()))) {
            int sessionId = generateSessionId();

            response::Login resLogin;
        }

    }
    if (req.has_register_()) {
        const request::Register &reqRegister = req.register_();

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