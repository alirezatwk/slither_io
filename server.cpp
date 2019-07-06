#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include <SFML/Network.hpp>
#include "messages.pb.h"

const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");

std::string name;
std::string msgSend;
bool new_message = false;

sf::TcpSocket socket;

std::list<sf::TcpSocket *> clients;
sf::SocketSelector selector;

sf::TcpListener listener;
sf::Mutex globalMutex;
bool quit = false;

void Sender(void) {
    while (!quit) {
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

void Request() {
    while (!quit) {
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                auto *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    clients.push_back(client);
                    selector.add(*client);
                } else {
                    delete client;
                }
            } else {
                std::vector<std::list<sf::TcpSocket *>::iterator> rm;
                std::size_t dummy;

                for (auto it = clients.begin(); it != clients.end(); ++it) {
                    sf::TcpSocket &client = **it;
                    if (selector.isReady(client)) {
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done) {
                            // TODO
                        }
                    }
                    // deleting disconnected clients
                    if (client.receive(&dummy, 0, dummy) == sf::Socket::Disconnected)
                        rm.push_back(it);
                }
                for (auto it : rm)
                    clients.erase(it);
            }
        }
    }
}


void GetInput(void) {
    std::string s;
    getline(std::cin, s);

    globalMutex.lock();
    msgSend = s;
    new_message = true;
    globalMutex.unlock();
}


int main(int argc, char *argv[]) {

    listener.listen(PORT);
    selector.add(listener);

    sf::Thread *requestThread = new sf::Thread(&Request);
    sf::Thread *responseThread = new sf::Thread(&Response);
    requestThread->launch();


    while (!quit) {
        GetInput();
    }

    if (requestThread){

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