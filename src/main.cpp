#include <iostream>
#include "server/Server.h"

int main() {
    Server redisServer(6379);
    if(!redisServer.start()){
        std::cout<<"Failed to start server\n";
        return 1;
    }

    redisServer.run();
    redisServer.cleanup();
    return 0;
}