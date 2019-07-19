#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

const int WindowSize = 300;

static const int loginFontSize = 20;
static const int borderSize = 4;
static const int startXForInputLogin = 105;
sf::Font font;

/*const unsigned short PORT = 5000;
const std::string IPADDRESS("127.0.0.1");

std::string name;

std::string msgSend;
bool new_message = false;

sf::TcpSocket socket;
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

void registerPage(sf::RenderWindow &window) {
    sf::Event event{};
    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            window.clear(sf::Color::Blue);

            window.display();
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }
}

void loginPage(sf::RenderWindow &window) {
    int state = 0;
    sf::Event event{};

    std::string username;
    std::string password;
    const std::string submitStr("Submit");
    const std::string registerStr("Register");

    sf::Text usernameText("Username :", font);
    sf::Text typedUsername("", font);
    sf::Text passwordText("Password :", font);
    sf::Text typedPassword("", font);
    sf::Text submitText(submitStr, font);
    sf::Text registerText(registerStr, font);

    usernameText.setCharacterSize(loginFontSize);
    typedUsername.setCharacterSize(loginFontSize);
    passwordText.setCharacterSize(loginFontSize);
    typedPassword.setCharacterSize(loginFontSize);
    submitText.setCharacterSize(loginFontSize);
    registerText.setCharacterSize(loginFontSize);

    usernameText.setFillColor(sf::Color::Black);
    typedUsername.setFillColor(sf::Color::Black);
    passwordText.setFillColor(sf::Color::Black);
    typedPassword.setFillColor(sf::Color::Black);
    submitText.setFillColor(sf::Color::Black);
    registerText.setFillColor(sf::Color::Black);

    typedUsername.setPosition(startXForInputLogin, 0);
    passwordText.setPosition(0, loginFontSize + borderSize);
    typedPassword.setPosition(startXForInputLogin, loginFontSize + borderSize);
    submitText.setPosition(0, 2 * (loginFontSize + borderSize));
    registerText.setPosition(0, 3 * (loginFontSize + borderSize));

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            window.clear(sf::Color::White);
            std::cout << username << std::endl;

            typedUsername.setString(username);
            submitText.setString(submitStr);
            registerText.setString(registerStr);

            // for making *****
            std::string starPassword;
            for (int i = 0; i < password.size(); i++)
                starPassword += '*';
            typedPassword.setString(starPassword);

            switch (state) {
                case 0:
                    typedUsername.setString(username + "|");
                    break;
                case 1:
                    typedPassword.setString(starPassword + "|");
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
            window.draw(typedUsername);
            window.draw(passwordText);
            window.draw(typedPassword);
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
                            switch (state) {
                                case 2:
                                    std::cout << username << ' ' << password << '\n';
                                    // TODO SEND USERNAME AND PASSWORD TO SERVER
                                    break;
                                case 3:
                                    registerPage(window);
                                    break;
                                default:
                                    break;
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
            if (event.type == sf::Event::Closed)
                window.close();
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