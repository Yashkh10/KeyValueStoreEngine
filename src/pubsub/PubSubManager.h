#pragma once

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <winsock2.h>
#include <mutex>

class PubSubManager {
private:
    std::unordered_map<std::string, std::unordered_set<SOCKET>> channelSubscribers;
    std::unordered_map<SOCKET, std::unordered_set<std::string>> clientChannels;

    std::mutex mtx;

public:
    void subscribe(const std::string& channel, SOCKET client);
    void unsubscribe(const std::string& channel, SOCKET client);
    void unsubscribeAll(SOCKET client);

    void publish(const std::string& channel, const std::string& message);
};