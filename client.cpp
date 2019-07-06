#include <iostream>
#include <string>
#include <SFML/Network.hpp>

const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");

sf::TcpSocket socket;


int main(int argc, char *argv[]){

    if(socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
        std::cout << "Connected\n";
    else
        std::cout << "NotConnected\n";

    char mes[100] = "saalm\0";

    while(true){
        socket.send(mes, 100);
    }

    return 0;
}