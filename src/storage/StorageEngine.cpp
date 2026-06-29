#include "StorageEngine.h"
#include <sstream>

StorageEngine::StorageEngine(): persistence("data.aof"){
    recover();
    cleanerThread = std::thread(&StorageEngine::cleanupLoop,this);
}

StorageEngine::~StorageEngine(){
    running = false;
    cv.notify_all();
    if(cleanerThread.joinable()){
        cleanerThread.join();
    }
}

void StorageEngine::set(const std::string& key,const std::string& value){
    uint64_t version;
    {
        std::lock_guard<std::mutex> lock(mtx);
        version = nextVersion++;
        store[key] ={value,{},false,version};
    }
    persistence.append("set " + key + " " + value);
}

void StorageEngine::setWithTTL(const std::string& key,const std::string& value,int ttlSeconds){
    long long expiryMillis;

    {
        std::lock_guard<std::mutex> lock(mtx);

        auto expiry = std::chrono::steady_clock::now() + std::chrono::seconds(ttlSeconds);
        uint64_t version = nextVersion++;

        store[key] = {value,expiry,true,version};
        minHeap.push({expiry,key,version});

        auto expirySystem =std::chrono::system_clock::now()+ std::chrono::seconds(ttlSeconds);
        expiryMillis = std::chrono::duration_cast<std::chrono::milliseconds>(
            expirySystem.time_since_epoch()
        ).count();
    }

    persistence.append("setex "+ key+ " "+ std::to_string(expiryMillis)+ " "+ value);
    cv.notify_one();
}

std::string StorageEngine::get(const std::string& key){
    std::lock_guard<std::mutex> lock(mtx);
    auto it = store.find(key);
    if(it==store.end()){
        return "Data not found";
    }

    if(it->second.hasExpiry &&std::chrono::steady_clock::now()>=it->second.expiryTime){
        store.erase(it);
        return "Data not found";
    }
    return it->second.value;
}

bool StorageEngine::del(const std::string& key){
    bool deleted = false;

    {
        std::lock_guard<std::mutex> lock(mtx);

        auto it = store.find(key);
        if(it != store.end()){
            store.erase(it);
            deleted = true;
        }
    }

    if(deleted){
        persistence.append("del " + key);
    }

    return deleted;
}

int StorageEngine::ttl(const std::string& key){
    std::lock_guard<std::mutex> lock(mtx);
    auto it = store.find(key);
    if(it==store.end()){
        return -2;
    }

    if(!it->second.hasExpiry){
        return -1;
    }

    auto now = std::chrono::steady_clock::now();
    if(now >= it->second.expiryTime){
        store.erase(it);
        return -2;
    }

    return static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(it->second.expiryTime - now).count());
}

void StorageEngine::cleanupExpired(){
    std::lock_guard<std::mutex> lock(mtx);
    auto now = std::chrono::steady_clock::now();
    while(!minHeap.empty()){
        const auto& node = minHeap.top();
        if(node.expiryTime > now){
            break;
        }

        auto it = store.find(node.key);
        if(it != store.end() &&it->second.hasExpiry &&it->second.version == node.version){
            store.erase(it);
        }
        minHeap.pop();
    }
}

void StorageEngine::cleanupLoop(){
    while(running){
        cleanupExpired();
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock,std::chrono::seconds(1),[this]{
            return !running.load();
        });
    }
}

void StorageEngine::recover(){
    auto commands = persistence.load();

    for(const auto& command : commands){
        std::stringstream ss(command);
        std::string operation;
        ss>>operation;

        if(operation=="set"){
            std::string key;
            std::string value;

            ss>>key;
            std::getline(ss,value);

            if(!value.empty() && value[0]==' '){
                value.erase(0,1);
            }

            uint64_t version = nextVersion++;

            store[key] = {value,{},false,version};
        }
        else if(operation=="setex"){
            std::string key;
            long long expiryMillis;

            ss>>key;
            ss>>expiryMillis;

            std::string value;
            std::getline(ss,value);
            if(!value.empty() && value[0]==' '){
                value.erase(0,1);
            }

            auto expirySystem =std::chrono::system_clock::time_point{} + std::chrono::milliseconds(expiryMillis);
            auto nowSystem = std::chrono::system_clock::now();

            if(expirySystem<=nowSystem){
                continue;
            }

            auto remaining = std::chrono::duration_cast<std::chrono::milliseconds>(expirySystem-nowSystem);
            auto expirySteady = std::chrono::steady_clock::now() + remaining;
            uint64_t version = nextVersion++;

            store[key] = {value,expirySteady,true,version};
            minHeap.push({expirySteady,key,version});
        }
        else if(operation=="del"){
            std::string key;
            ss>>key;
            store.erase(key);
        }
    }
}