#include "CommandParser.h"

#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<std::string> CommandParser::splitCommand(const std::string& input){
    std::stringstream ss(input);
    std::string word;
    std::vector<std::string> words;

    while(ss >> word){
        words.push_back(word);
    }

    return words;
}

std::string CommandParser::toLower(std::string str){
    std::transform(
        str.begin(),
        str.end(),
        str.begin(),
        [](unsigned char c){
            return std::tolower(c);
        }
    );

    return str;
}

CommandData CommandParser::parse(const std::string& input){
    std::vector<std::string> parts = splitCommand(input);

    if(parts.empty()){
        return {"invalid", {}};
    }

    std::string operation = toLower(parts[0]);

    if(operation == "get"){
        if(parts.size() != 2){
            return {"invalid", {}};
        }
        return {"get", {parts[1]}};
    }

    if(operation == "del"){
        if(parts.size() != 2){
            return {"invalid", {}};
        }
        return {"del", {parts[1]}};
    }

    if(operation == "ttl"){
        if(parts.size() != 2){
            return {"invalid", {}};
        }
        return {"ttl", {parts[1]}};
    }

    // =========================
    // QUIT
    // =========================
    if(operation == "quit"){
        return {"quit", {}};
    }

    // =========================
    // SET
    // =========================
    if(operation == "set"){
        if(parts.size() < 3){
            return {"invalid", {}};
        }

        std::string key = parts[1];

        std::ostringstream valueBuilder;

        for(size_t i = 2; i < parts.size(); i++){
            valueBuilder << parts[i];
            if(i + 1 < parts.size()){
                valueBuilder << " ";
            }
        }

        return {"set", {key, valueBuilder.str()}};
    }

    if(operation == "subscribe"){
        if(parts.size() != 2){
            return {"invalid", {}};
        }

        return {"subscribe", {parts[1]}};
    }
    if(operation == "unsubscribe"){
        if(parts.size() != 2){
            return {"invalid", {}};
        }

        return {"unsubscribe", {parts[1]}};
    }
    
    if(operation == "publish"){
        if(parts.size() < 3){
            return {"invalid", {}};
        }

        std::string channel = parts[1];

        std::ostringstream msg;
        for(size_t i = 2; i < parts.size(); i++){
            msg << parts[i];
            if(i + 1 < parts.size()){
                msg << " ";
            }
        }

        return {"publish", {channel, msg.str()}};
    }

    return {"invalid", {}};
}