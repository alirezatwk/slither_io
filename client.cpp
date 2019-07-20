#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "proto/communication.pb.h"
#include "proto/request.pb.h"
#include "proto/response.pb.h"

const int WindowSize = 300;


static const int fontSize = 15;
static const int borderSize = 4;

static const int PORT = 5000;
const std::string IPADDRESS("127.0.0.1");

sf::TcpSocket socket;

sf::Font font;


int sessionId;


void menuPage(sf::RenderWindow &window) {

}

/*

void Sender(void) {
    while (!quit) {
        bool send = false;
        globalMutex.lock();
        if (new_message) {
            new_message = false;
            send = true;

        sf::Packet packetSend;
            Message msg;
            msg.set_name(name);
            msg.set_chat_message(msgSend);

            std::stringstream stream;
            msg.SerializeToOstream(&stream);

            packetSend << stream.str();
            socket.send(packetSend);
        }
        globalMutex.unlock();

        if (send)
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
    sf::TcpListener listener;
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
    std::string s;
    getline(std::cin, s);

    globalMutex.lock();
    msgSend = s;
    new_message = true;
    globalMutex.unlock();
}

*/

void write(std::string &curr, sf::Keyboard::Key &key) {
    if (sf::Keyboard::A <= key && key <= sf::Keyboard::Key::Num9) {
        if (key < sf::Keyboard::Num0) {
            curr += static_cast<char>(static_cast<int>('A') + key);
        } else {
            curr += static_cast<char>(static_cast<int>('0') + (key - sf::Keyboard::Num0));
        }
    } else if (key == sf::Keyboard::BackSpace) {
        if (!curr.empty()) {
            curr.resize(curr.size() - 1);
        }
    }
}

std::string makeStar(const std::string &password) {
    std::string ans;
    for (int i = 0; i < password.size(); i++)
        ans += '*';
    return ans;
}

void gamePage(sf::RenderWindow &window) {


}

void registerPage(sf::RenderWindow &window) {
    int state = 0;
    sf::Event event{};

    std::string name;
    const std::string nameStr("Name:");
    std::string username;
    const std::string usernameStr("Username:");
    std::string password;
    const std::string passwordStr("Password:");
    std::string confirmPassword;
    const std::string confirmPasswordStr("Password:");
    const std::string registerStr("Register");

    sf::Text nameText("", font);
    sf::Text usernameText("", font);
    sf::Text passwordText("", font);
    sf::Text passwordConfirmText("", font);
    sf::Text registerText("", font);

    nameText.setCharacterSize(fontSize);
    usernameText.setCharacterSize(fontSize);
    passwordText.setCharacterSize(fontSize);
    passwordConfirmText.setCharacterSize(fontSize);
    registerText.setCharacterSize(fontSize);

    nameText.setFillColor(sf::Color::Black);
    usernameText.setFillColor(sf::Color::Black);
    passwordText.setFillColor(sf::Color::Black);
    passwordConfirmText.setFillColor(sf::Color::Black);
    registerText.setFillColor(sf::Color::Black);

    usernameText.setPosition(0, fontSize + borderSize);
    passwordText.setPosition(0, 2 * (fontSize + borderSize));
    passwordConfirmText.setPosition(0, 3 * (fontSize + borderSize));
    registerText.setPosition(0, 4 * (fontSize + borderSize));

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            window.clear(sf::Color::White);

            nameText.setString(nameStr + name);
            usernameText.setString(usernameStr + username);
            passwordText.setString(passwordStr + makeStar(password));
            passwordConfirmText.setString(confirmPasswordStr + makeStar(confirmPassword));
            registerText.setString(registerStr);

            switch (state) {
                case 0:
                    nameText.setString(nameStr + name + "|");
                    break;
                case 1:
                    usernameText.setString(usernameStr + username + "|");
                    break;
                case 2:
                    passwordText.setString(passwordStr + makeStar(password) + "|");
                    break;
                case 3:
                    passwordConfirmText.setString(confirmPasswordStr + makeStar(confirmPassword) + "|");
                    break;
                case 4:
                    registerText.setString(registerStr + "<-");
                    break;
                default:
                    break;
            }

            window.draw(nameText);
            window.draw(usernameText);
            window.draw(passwordText);
            window.draw(passwordConfirmText);
            window.draw(registerText);

            switch (event.type) {
                case sf::Event::Closed:
                    std::cout << "close that.";
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            state = std::max(0, state - 1);
                            break;
                        case sf::Keyboard::Down:
                            state = std::min(4, state + 1);
                            break;
                        case sf::Keyboard::Space:
                            if (state == 4) {
                                std::cout << name << ' ' << username << ' ' << password << ' ' << confirmPassword
                                          << '\n';
                                socket.connect(IPADDRESS, PORT);
                                sf::Packet packetSend;
                                Request req;
                                auto reg = req.mutable_register_();
                                reg->set_name(name);
                                reg->set_username(username);
                                reg->set_password(password);
                                reg->set_confirm_password(confirmPassword);

                                std::stringstream stream;
                                req.SerializeToOstream(&stream);

                                packetSend << stream.str();
                                socket.send(packetSend);

                                std::string msgStr;
                                sf::Packet packetReceive;

                                socket.receive(packetReceive);
                                if (packetReceive >> msgStr) {
                                    Response res;
                                    res.ParseFromString(msgStr);
                                    // TODO INJAHASH MOND
                                    sessionId = static_cast<int>(res.login().session_id());
                                    if (sessionId != 0) {
                                        menuPage(window);
                                        return;
                                    }
                                }
                                socket.disconnect();
                            }
                            // TODO
                            break;
                        default:
                            switch (state) {
                                case 0:
                                    write(name, event.key.code);
                                    break;
                                case 1:
                                    write(username, event.key.code);
                                    break;
                                case 2:
                                    write(password, event.key.code);
                                    break;
                                case 3:
                                    write(confirmPassword, event.key.code);
                                    break;
                                default:
                                    break;
                            }
                            break;
                    }

                    break;
                default:
                    break;
            }

            window.display();
        }
    }
}

void loginPage(sf::RenderWindow &window) {
    int state = 0;
    sf::Event event{};

    std::string username;
    const std::string usernameStr("Username:");
    std::string password;
    const std::string passwordStr("Password:");
    const std::string submitStr("Submit");
    const std::string registerStr("Register");

    sf::Text usernameText("", font);
    sf::Text passwordText("", font);
    sf::Text submitText("", font);
    sf::Text registerText("", font);

    usernameText.setCharacterSize(fontSize);
    passwordText.setCharacterSize(fontSize);
    submitText.setCharacterSize(fontSize);
    registerText.setCharacterSize(fontSize);

    usernameText.setFillColor(sf::Color::Black);
    passwordText.setFillColor(sf::Color::Black);
    submitText.setFillColor(sf::Color::Black);
    registerText.setFillColor(sf::Color::Black);

    passwordText.setPosition(0, fontSize + borderSize);
    submitText.setPosition(0, 2 * (fontSize + borderSize));
    registerText.setPosition(0, 3 * (fontSize + borderSize));

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            window.clear(sf::Color::White);

            usernameText.setString(usernameStr + username);
            passwordText.setString(passwordStr + makeStar(password));
            submitText.setString(submitStr);
            registerText.setString(registerStr);

            switch (state) {
                case 0:
                    usernameText.setString(usernameStr + username + "|");
                    break;
                case 1:
                    passwordText.setString(passwordStr + makeStar(password) + "|");
                    break;
                case 2:
                    submitText.setString(submitStr + "<-");
                    break;
                case 3:
                    registerText.setString(registerStr + "<-");
                    break;
                default:
                    break;
            }

            window.draw(usernameText);
            window.draw(passwordText);
            window.draw(submitText);
            window.draw(registerText);

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Up:
                            state = std::max(0, state - 1);
                            break;
                        case sf::Keyboard::Down:
                            state = std::min(3, state + 1);
                            break;
                        case sf::Keyboard::Space:
                            if (state == 2) {
                                std::cout << username << ' ' << password << '\n';
                                socket.connect(IPADDRESS, PORT);
                                sf::Packet packetSend;
                                Request req;
                                auto login = req.mutable_login();
                                login->set_username(username);
                                login->set_password(password);

                                std::stringstream stream;
                                req.SerializeToOstream(&stream);

                                packetSend << stream.str();
                                socket.send(packetSend);

                                std::string msgStr;
                                sf::Packet packetReceive;

                                socket.receive(packetReceive);
                                if (packetReceive >> msgStr) {
                                    Response res;
                                    res.ParseFromString(msgStr);
                                    sessionId = static_cast<int>(res.login().session_id());
                                    if (sessionId != 0) {
                                        menuPage(window);
                                        return;
                                    }
                                }
                                socket.disconnect();
                            } else if (state == 3) {
                                registerPage(window);
                            }
                        default:
                            switch (state) {
                                case 0:
                                    write(username, event.key.code);
                                    break;
                                case 1:
                                    write(password, event.key.code);
                                    break;
                                default:
                                    break;
                            }
                            break;
                    }

                    break;
                default:
                    break;
            }

            window.display();
        }

    }
}


int main(int argc, char *argv[]) {
    sf::RenderWindow window(sf::VideoMode(WindowSize, WindowSize), "Slither.io");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);
    font.loadFromFile("arial.ttf");

    loginPage(window);

    return 0;
}