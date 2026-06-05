#include "Persistence.h"

#include <fstream>

Persistence::Persistence(const std::string& filename): filename(filename){
    outFile.open(filename,std::ios::app);
}

Persistence::~Persistence(){
    if(outFile.is_open()){
        outFile.close();
    }
}

void Persistence::append(const std::string& command){
    if(outFile.is_open()){
        outFile << command << '\n';
        outFile.flush();
    }
}

std::vector<std::string> Persistence::load(){
    std::vector<std::string> commands;

    std::ifstream inFile(filename);

    std::string line;

    while(std::getline(inFile,line)){
        if(!line.empty()){
            commands.push_back(line);
        }
    }

    return commands;
}