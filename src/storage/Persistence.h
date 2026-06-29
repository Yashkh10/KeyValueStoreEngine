#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>

class Persistence {
public:
    explicit Persistence(const std::string& filename);
    ~Persistence();

    void append(const std::string& command);
    std::vector<std::string> load();

private:
    void writerLoop();

    std::string filename;
    std::ofstream outFile;

    std::queue<std::string> writeQueue;

    std::mutex queueMutex;
    std::condition_variable cv;

    std::thread writerThread;
    std::atomic<bool> running{true};
};