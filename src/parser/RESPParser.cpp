#include "RESPParser.h"

#include <sstream>

bool RESPParser::isRESP(const std::string& input){
    return !input.empty() && input[0] == '*';
}

std::vector<std::string> RESPParser::parse(const std::string& input){
    std::vector<std::string> parts;
    std::stringstream ss(input);
    std::string line;
    std::getline(ss,line);
    int count = std::stoi(line.substr(1));

    for(int i=0;i<count;i++){
        std::getline(ss,line);
        std::getline(ss,line);
        if(!line.empty() && line.back()=='\r'){
            line.pop_back();
        }

        parts.push_back(line);
    }

    return parts;
}

bool RESPParser::hasCompleteMessage(const std::string& buffer){
    if(buffer.empty() || buffer[0] != '*'){
        return false;
    }

    std::stringstream ss(buffer);
    std::string line;
    if(!std::getline(ss,line)){
        return false;
    }
    if(!line.empty() &&line.back()=='\r'){
        line.pop_back();
    }

    int count = std::stoi(line.substr(1));
    int linesNeeded = 1 + count*2;
    int linesFound = 0;
    std::stringstream counter(buffer);
    while(std::getline(counter,line)){
        linesFound++;
    }

    return linesFound>=linesNeeded;
}

std::string RESPParser::extractMessage(std::string& buffer){
    std::stringstream ss(buffer);
    std::string line;
    std::getline(ss,line);
    std::string firstLine = line;
    if(!firstLine.empty() &&
       firstLine.back()=='\r'){
        firstLine.pop_back();
    }

    int count = std::stoi(firstLine.substr(1));

    int linesNeeded = 1 + count * 2;

    std::string message;
    std::stringstream reader(buffer);
    for(int i=0;i<linesNeeded;i++){
        std::getline(reader,line);
        message+=line;
        if(i+1<linesNeeded){
            message += "\n";
        }
    }

    size_t consumed = 0;
    reader.clear();
    reader.seekg(0);
    for(int i=0;i<linesNeeded;i++){
        std::getline(reader,line);
        consumed+=line.size();
        consumed+=1;
    }
    buffer.erase(0,consumed);
    return message;
}