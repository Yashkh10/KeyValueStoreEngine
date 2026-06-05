#pragma once

#include <string>
#include <vector>
#include <fstream>

class Persistence{
private:
    std::string filename;
    std::ofstream outFile;

public:
    explicit Persistence(const std::string& filename);

    ~Persistence();

    void append(const std::string& command);

    std::vector<std::string> load();
};