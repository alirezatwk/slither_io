#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <random>
#include <fstream>

#include <SFML/Network.hpp>
#include "proto/communication.pb.h"
#include "proto/types.pb.h"
#include "proto/database.pb.h"
#include "proto/response.pb.h"
#include "proto/request.pb.h"

#include "model/Client.h"
#include "model/ServerQueue.h"
#include "model/ServerUser.h"
#include "model/ServerGameController.h"

const unsigned short PORT = 5000;
sf::TcpListener listener;
const std::string DATABASE("database.txt");
const int NORMALQUEUESIZE = 6;

std::map<int, int> sessionIdToId;

std::map<int, ServerUser *> users;
std::map<int, ServerQueue *> queues;
std::map<int, ServerGameController *> games;

std::map<int, sf::Thread *> threadId;
std::map<int, sf::TcpSocket *> socketId;

int idForQueuesAndGames = 1;
int idForUsers = 1;

bool quit = false;

sf::Mutex globalMutex;

void importDatabase() {
    std::ifstream fIn;
    fIn.open(DATABASE);

    database::User user;


    std::string userStr;
    while (fIn) {
        fIn >> userStr;
        user.ParseFromString(userStr);
        auto newUser = new ServerUser(idForUsers, user.name(), user.username(), user.password(),
                                      static_cast<int>(user.score()), static_cast<int>(user.session_id()),
                                      static_cast<int>(user.queue_id()), false);
        users[idForUsers] = newUser;
        idForUsers++;
    }
    fIn.close();
}

void exportDatabase() {
    std::ofstream fOut;
    fOut.open(DATABASE);

    for (auto it : users) {
        ServerUser &serverUser = *it.second;
        database::User user;
        user.set_id(static_cast<google::protobuf::uint64>(serverUser.getId()));
        user.set_username(serverUser.getUsername());
        user.set_password(serverUser.getPassword());
        user.set_name(serverUser.getName());
        user.set_session_id(0);
        user.set_queue_id(0);
        user.set_game_id(0);
        std::string userStr;
        user.SerializeToString(&userStr);
        fOut << userStr << '\n';
    }
    for (auto it : users) {
        delete it.second;
    }

    fOut.close();
}

int generateSessionId() {
    int ans = 0;
    while (ans == 0 || sessionIdToId.find(ans) != sessionIdToId.end())
        ans = rand();
    return ans;
} // False sessionId is 0.


const int C = 313;
const int MOD = 1LL * 1000 * 1000 * 1000 + 7;

std::string hashFunction(std::string password) {
    long long ans = 0, t = 1;
    for (char c : password) {
        ans += (t * c) % MOD;
        ans %= MOD;
        t *= C;
        t %= MOD;
    }
    std::string hashPassword;
    while (ans) {
        hashPassword += static_cast<char>(static_cast<int>('0') + (ans % 10));
        ans /= 10;
    }
    return hashPassword;
}

bool checkUsernamePassword(const std::string &username, const std::string &password) {
    const std::string &hashPassword = hashFunction(password);
    for (auto it : users) {
        const ServerUser &user = (*it.second);
        if (username == user.getUsername() && hashPassword == user.getPassword() && !user.isLogin()) {
            return true;
        }
    }
    return false;
}

int makeAndSetSessionId(const std::string &username) {
    for (auto it : users) {
        auto &user = (*it.second);
        if (user.getUsername() == username) {
            user.setSessionId(generateSessionId());
            user.setLogin(true);
            sessionIdToId[user.getSessionId()] = user.getId();
            return user.getSessionId();
        }
    }
    return 0;
} // Can't find username -> 0

bool hadUsername(const std::string &username) {
    for (auto it : users) {
        const ServerUser &user = *it.second;
        if (username == user.getUsername()) {
            return true;
        }
    }
    return false;
}

bool makeUserInUsers(const std::string &name, const std::string &username, const std::string &password,
                     const std::string &confirmPassword) {
    if (password != confirmPassword || hadUsername(username))
        return false;
    auto newUser = new ServerUser(idForUsers, name, username, hashFunction(password), 0, 0, 0, false);
    users[idForUsers++] = newUser;
    return true;
}

int createQueue(int size) {
    auto serverQueue = new ServerQueue(idForQueuesAndGames, size);
    queues[idForQueuesAndGames] = serverQueue;
    idForQueuesAndGames++;
    return idForQueuesAndGames - 1;
}


void GetInput(int id) {

    sf::TcpSocket *socket = socketId[id];


    sf::Packet packetReceive;
    socket->receive(packetReceive);

    std::string reqStr;
    packetReceive >> reqStr;

    Request req;
    req.ParseFromString(reqStr);


    globalMutex.lock();

    Response res;
    // Login gets(username, password) give(sessionId)
    if (req.has_login()) {
        const request::Login &reqLogin = req.login();
        auto resLogin = res.mutable_login();
        if (checkUsernamePassword(reqLogin.username(), reqLogin.password())) {
            int sessionId = makeAndSetSessionId(reqLogin.username());
            resLogin->set_session_id((uint) sessionId);
        } else {
            resLogin->set_session_id(0);
        }
    }

    // Register gets(name, username, password, confirmPassword) give(success) DONE
    if (req.has_register_()) {
        const request::Register &reqRegister = req.register_();
        auto resRegister = res.mutable_register_();
        resRegister->set_success(
                makeUserInUsers(reqRegister.name(), reqRegister.username(), reqRegister.password(),
                                reqRegister.confirm_password()));
    }

    // UserInfo gets(sessionId) give(name, score) DONE
    if (req.has_user_info()) {
        const request::UserInfo &reqUserInfo = req.user_info();
        auto resUserInfo = res.mutable_user_info();
        int userId = sessionIdToId[reqUserInfo.session_id()];
        resUserInfo->set_score(static_cast<google::protobuf::uint64>(users[userId]->getScore()));
        resUserInfo->set_name(users[userId]->getName());
    }

    // Scoreboard gets(sessionId) give(score, rank, UserScore top10)
    if (req.has_scoreboard()) {
        const request::Scoreboard &reqScoreboard = req.scoreboard();
        auto resScoreboard = res.mutable_scoreboard();
        int userId = sessionIdToId[reqScoreboard.session_id()];

        int score = users[userId]->getScore();
        resScoreboard->set_user_score(static_cast<google::protobuf::uint64>(score));

        int rank = 1;
        for (auto it : users) {
            ServerUser &user = *it.second;
            if (user.getScore() > score)
                rank++;
        }
        resScoreboard->set_user_rate(static_cast<google::protobuf::uint64>(rank));

        std::set<std::pair<int, int> > tops;
        for (auto it : users) {
            ServerUser &user = *it.second;
            tops.insert({-user.getScore(), user.getId()});
        }

        int t = 0;
        for (auto it : tops) {
            if (t == 10)
                break;
            auto add = resScoreboard->add_top_10();
            add->set_name(users[it.second]->getName());
            add->set_score(static_cast<google::protobuf::uint64>(-it.first));
            t++;
        }
    }

    // QueueCreate gets(sessionId, queueSize) give(idForQueuesAndGames)
    if (req.has_queue_create()) {
        const request::QueueCreate &reqQCreate = req.queue_create();
        auto resQCreate = res.mutable_queue_create();
        int queueId = createQueue(reqQCreate.queue_size());
        int userId = sessionIdToId[reqQCreate.session_id()];
        queues[queueId]->addUser(users[userId]); // Is always true
        resQCreate->set_queue_id(static_cast<google::protobuf::uint64>(queueId));
    }

    // QueueJoin gets(sessionId, idForQueuesAndGames) give(success)
    if (req.has_queue_join()) {
        const request::QueueJoin &reqQJoin = req.queue_join();
        auto resQJoin = res.mutable_queue_join();

        int userId = sessionIdToId[reqQJoin.session_id()];
        int queueId = reqQJoin.queue_id();
        auto queue = queues[queueId];

        bool added = queue->addUser(users[userId]);

        resQJoin->set_success(added);
        if (added) {
            users[userId]->setQueueId(queueId);
            if (queue->isFull()) {
                auto game = new ServerGameController(queueId);
                games[queueId] = game;
                for (int i = 0; i < queue->getMAXSIZE(); i++) {
                    auto user = queue->getUser(i);
                    auto newClient = game->addClient(user);
                    delete user;
                    users[newClient->getId()] = newClient;
                }
                queues.erase(queueId);
                delete queue;
                if (queues.empty()) {
                    createQueue(NORMALQUEUESIZE);
                }
            }
        }
    }

    // QueueList gets(sessionId) give(repeated QueueItem) -> QueueItem = id, occupied, size
    if (req.has_queue_list()) {
        const request::QueueList &reqQList = req.queue_list();
        auto resQList = res.mutable_queue_list();
        for (auto it : queues) {
            auto add = resQList->add_queue_items();
            add->set_id(static_cast<google::protobuf::uint64>(it.second->getId()));
            add->set_occupied(static_cast<google::protobuf::uint32>(it.second->getSize()));
            add->set_size(static_cast<google::protobuf::uint32>(it.second->getMAXSIZE()));
        }
    }

    // QueueStatus gets(sessionId) [if in a queue] give(repeated playerNames[string], size, initialized)
    if (req.has_queue_status()) {
        const request::QueueStatus &reqQStatus = req.queue_status();
        auto resQStatus = res.mutable_queue_status();
        int userId = sessionIdToId[reqQStatus.session_id()];
        int queueId = users[userId]->getQueueId();

        if (queueId != 0) {
            for (int i = 0; i < queues[queueId]->getSize(); i++) {
                auto add = resQStatus->add_player_names();
                add->assign(queues[queueId]->getUser(i)->getName());
            }
            resQStatus->set_size(static_cast<google::protobuf::uint32>(queues[queueId]->getMAXSIZE()));
            resQStatus->set_initialized(queues[queueId]->isFull());
        } else {
            resQStatus->set_size(0);
            resQStatus->set_initialized(false);
        }
    }

    // PendingGameCycle gets(sessionId) give(cycleNumber)
    if (req.has_pending_game_cycle()) {
        int userId = sessionIdToId[req.pending_game_cycle().session_id()];
        auto resPending = res.mutable_pending_game_cycle();
        ServerUser &user = *users[userId];
        if (user.isInGame()) {
            auto client = dynamic_cast<Client *>(&user);
            int gameId = client->getGameId();
            games[gameId]->getCycleNumber(resPending);
        } else {
            resPending->set_cycle_number(0);
        }
    }

    // GameState gets(sessionId)
    if (req.has_game_state()) {
        const request::GameState &reqGameState = req.game_state();
        auto resGameState = res.mutable_game_state();
        int userId = sessionIdToId[reqGameState.session_id()];
        auto client = dynamic_cast<Client *>(users[userId]);
        if (client != nullptr) {
            games[client->getGameId()]->getGameState(resGameState);
        }
    }

    // ActionSubmit gets(sessionId, Action)
    if (req.has_action_submit()) {
        const request::ActionSubmit &reqActionSubmit = req.action_submit();
        auto resActionSubmit = res.mutable_action_submit();
        int userId = sessionIdToId[reqActionSubmit.session_id()];
        auto client = dynamic_cast<Client *>(users[userId]);
        if (client != nullptr) {
            games[client->getGameId()]->actionSubmit(client->getInGameId(),
                                                     static_cast<Direction>(reqActionSubmit.action()));
            resActionSubmit->set_success(true);
        } else {
            resActionSubmit->set_success(false);
        }
    }

    sf::Packet packetSend;

    std::stringstream stream;
    res.SerializeToOstream(&stream);
    packetSend << stream.str();
    socket->send(packetSend);

    globalMutex.unlock();

    // Ba farze inke beshe hamaro delete kard.
    delete socketId[id];
    socketId.erase(id);
    sf::Thread *tmp = threadId[id];
    threadId.erase(id);
    delete tmp;
}

void ClientHandler() {
    int id = 0;
    while (!quit) {
        auto *socket = new sf::TcpSocket;
        listener.accept(*socket);
        auto *thread = new sf::Thread(&GetInput, id);
        socketId[id] = socket;
        threadId[id] = thread;
        thread->launch();
    }
}

int main(int argc, char *argv[]) {

    // TODO OK KON RANDOM HA RO.
    srand(static_cast<unsigned int>(time(0)));

    // TODO BEBIN KODOMA TO PROTOBUF SIZE HASHON CHIAN, TAGHIR BEDESHON.
    // TODO END OF GAME.

    importDatabase();
    createQueue(NORMALQUEUESIZE);

    listener.listen(PORT);
    auto thread = new sf::Thread(&ClientHandler);
    thread->launch();

    std::string temp;
    std::cin >> temp;
    delete thread;

    exportDatabase();

    for(auto it : queues){
        delete it.second;
    }
    for(auto it : games){
        delete it.second;
    }
    return 0;
}