#include "PubSubManager.h"
#include <winsock2.h>

void PubSubManager::subscribe(const std::string& channel,SOCKET client){
    std::lock_guard<std::mutex> lock(mtx);

    channelSubscribers[channel].insert(client);
    clientChannels[client].insert(channel);
}

void PubSubManager::unsubscribe(const std::string& channel,SOCKET client){
    std::lock_guard<std::mutex> lock(mtx);

    auto it = channelSubscribers.find(channel);
    if(it!=channelSubscribers.end()){
        it->second.erase(client);

        if(it->second.empty()){
            channelSubscribers.erase(it);
        }
    }

    auto cit = clientChannels.find(client);
    if(cit!=clientChannels.end()){
        cit->second.erase(channel);

        if(cit->second.empty()){
            clientChannels.erase(cit);
        }
    }
}

void PubSubManager::unsubscribeAll(SOCKET client){
    std::lock_guard<std::mutex> lock(mtx);

    auto it = clientChannels.find(client);
    if(it==clientChannels.end()) return;

    for(const std::string& channel : it->second){
        auto sit = channelSubscribers.find(channel);
        if(sit!=channelSubscribers.end()){
            sit->second.erase(client);

            if(sit->second.empty()){
                channelSubscribers.erase(sit);
            }
        }
    }

    clientChannels.erase(it);
}

void PubSubManager::publish(const std::string& channel,const std::string& message){
    std::vector<SOCKET> targets;

    {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = channelSubscribers.find(channel);
        if(it==channelSubscribers.end()) return;

        targets.assign(it->second.begin(),it->second.end());
    }

    for(SOCKET client : targets){
        std::string msg = message + "\r\n";
        send(client,msg.c_str(),(int)msg.size(),0);
    }
}