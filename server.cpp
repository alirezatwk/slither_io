#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <SFML/Network.hpp>
#include "proto/communication.pb.h"
#include "proto/database.pb.h"
#include "proto/request.pb.h"
#include "proto/response.pb.h"
#include "proto/types.pb.h"

#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include "messages.pb.h"

const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");

std::string name;
std::string msgSend;
bool new_message = false;

sf::TcpListener listener;
sf::TcpSocket socket;
sf::Mutex globalMutex;
bool quit = false;

char who;

void Sender(int x) {
    while (!quit) {
        std::cout << x << std::endl;
        sf::Packet packetSend;
        bool send = false;
        globalMutex.lock();
        if (new_message) {
            new_message = false;
            send = true;

            Message msg;
            msg.set_name(name);
            msg.set_chat_message(msgSend);

            std::stringstream stream;
            msg.SerializeToOstream(&stream);

            packetSend << stream.str();
        }
        globalMutex.unlock();

        if (send)
            socket.send(packetSend);
    }
}

void Receiver(void) {
    while (!quit) {
        std::string msgStr;
        sf::Packet packetReceive;

        socket.receive(packetReceive);
        if (packetReceive >> msgStr) {
            Message msg;
            msg.ParseFromString(msgStr);

            std::cout << msg.name() << ": " << msg.chat_message() << std::endl;
        }

    }
}

void Server(void) {
    listener.listen(PORT);
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
}

bool Client(void) {
    if (socket.connect(IPADDRESS, PORT) == sf::Socket::Done) {
        std::cout << "Connected\n";
        return true;
    }
    return false;
}

void GetInput(void) {
    while (!quit) {
        std::string s;
        getline(std::cin, s);
        if (s == "quit")
            quit = true;
        globalMutex.lock();
        msgSend = s;
        new_message = true;
        globalMutex.unlock();
    }
}


int main(int argc, char *argv[]) {
    sf::Thread *receiver_thread = 0, *sender_thread = 0, *getInput_thread = 0;

    std::cout << "Do you want to be a server (s) or a client (c) ? ";
    std::cin >> who;

    std::string temp;
    getline(std::cin, temp);

    socket.setBlocking(false);

    if (who == 's')
        Server();
    else
        Client();

    receiver_thread = new sf::Thread(&Receiver);
    sender_thread = new sf::Thread(&Sender);
    getInput_thread = new sf::Thread(&GetInput);
    receiver_thread->launch();
    sender_thread->launch();
    getInput_thread->launch();


    while (!quit) {
        if (who == 's') {
            listener.listen(PORT);
            listener.accept(socket);
            std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
        }
    }

    if (getInput_thread) {
        getInput_thread->wait();
        delete getInput_thread;
    }
    if (sender_thread) {
        sender_thread->wait();
        delete sender_thread;
    }
    if (receiver_thread) {
        receiver_thread->wait();
        delete receiver_thread;
    }
    return 0;
}