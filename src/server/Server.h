#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include "../storage/StorageEngine.h"
#include "../parser/CommandParser.h"
#include "../commands/CommandFactory.h"

#pragma comment(lib, "Ws2_32.lib")

class Server {
private:
    int port;
    SOCKET serverSocket;
    StorageEngine storage;
    CommandParser parser;
    CommandFactory factory;

public:
    Server(int port);
    bool start();
    void run();
    void cleanup();
};