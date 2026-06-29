#pragma once

#include "../storage/StorageEngine.h"
#include "../pubsub/PubSubManager.h"
#include <winsock2.h>

struct CommandContext {
    StorageEngine& storage;
    PubSubManager& pubsub;
    SOCKET clientSocket;
};