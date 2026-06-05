#pragma once

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <cstdint>

#include "CacheEntry.h"
#include "Persistence.h"

class StorageEngine{
private:
    struct HeapNode{
        std::chrono::steady_clock::time_point expiryTime;
        std::string key;
        uint64_t version;

        bool operator>(const HeapNode& other) const{
            return expiryTime > other.expiryTime;
        }
    };

    std::unordered_map<std::string,CacheEntry> store;

    std::priority_queue<HeapNode,std::vector<HeapNode>,std::greater<>>minHeap;
    std::mutex mtx;
    std::thread cleanerThread;
    std::condition_variable cv;
    std::atomic<bool> running{true};
    uint64_t nextVersion = 1;
    Persistence persistence;

    void cleanupLoop();

    void cleanupExpired();

public:

    StorageEngine();

    ~StorageEngine();

    void set(const std::string& key,const std::string& value);

    void setWithTTL(const std::string& key,const std::string& value,int ttlSeconds);

    std::string get(const std::string& key);

    bool del(const std::string& key);

    int ttl(const std::string& key);

    void recover();
};