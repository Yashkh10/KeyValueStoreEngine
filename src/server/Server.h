#pragma once

#include <winsock2.h>
#include <vector>
#include <string>

#include "../storage/StorageEngine.h"
#include "../pubsub/PubSubManager.h"
#include "../commands/CommandFactory.h"
#include "../parser/CommandParser.h"

class Server {
private:
    int port;
    SOCKET serverSocket;

    StorageEngine storage;
    PubSubManager pubsub;
    CommandFactory factory;
    CommandParser parser;

    std::vector<SOCKET> clients;
    std::vector<std::string> buffers;

private:
    void handleNewClient();
    void handleClientData(size_t index);
    void disconnectClient(size_t index);

    void sendResponse(SOCKET client, const std::string& raw);

public:
    Server(int port);
    bool start();
    void run();
    void cleanup();
};