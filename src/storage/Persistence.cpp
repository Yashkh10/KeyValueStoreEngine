#include "Persistence.h"

#include <fstream>
#include <chrono>

Persistence::Persistence(const std::string& filename): filename(filename){
    outFile.open(filename, std::ios::app);

    writerThread = std::thread(
        &Persistence::writerLoop,
        this
    );
}

Persistence::~Persistence(){
    running = false;
    cv.notify_all();

    if(writerThread.joinable())
        writerThread.join();

    if(outFile.is_open())
        outFile.close();
}

void Persistence::append(const std::string& command){
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        writeQueue.push(command);
    }

    cv.notify_one();
}

void Persistence::writerLoop(){
    std::vector<std::string> batch;

    while(running){
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            cv.wait_for(
                lock,
                std::chrono::milliseconds(100),
                [this]
                {
                    return !writeQueue.empty()
                           || !running;
                }
            );

            while(!writeQueue.empty()){
                batch.push_back(
                    std::move(writeQueue.front())
                );
                writeQueue.pop();
            }
        }

        if(batch.empty())
            continue;

        for(const auto& cmd : batch)
        {
            outFile << cmd << '\n';
        }

        outFile.flush();

        batch.clear();
    }

    while(true){
        std::string cmd;

        {
            std::lock_guard<std::mutex> lock(queueMutex);

            if(writeQueue.empty())
                break;

            cmd = std::move(writeQueue.front());
            writeQueue.pop();
        }

        outFile << cmd << '\n';
    }

    outFile.flush();
}

std::vector<std::string> Persistence::load(){
    std::vector<std::string> commands;
    std::ifstream inFile(filename);
    std::string line;

    while(std::getline(inFile,line)){
        if(!line.empty())
            commands.push_back(line);
    }

    return commands;
}