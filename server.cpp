#include <iostream>
#include <SFML/Network.hpp>
#include <string>

const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");

bool quit = false;

void Sender() {
    sf::Packet packetSend;

}

void Receiver() {
    while(!quit){

    }

}

int main(int argc, char *argv[]) {

    sf::Thread *receiver_thread = 0, *sender_thread = 0;
    receiver_thread = new sf::Thread(&Receiver);
    sender_thread = new sf::Thread(&Sender);
    receiver_thread->launch();
    sender_thread->launch();


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