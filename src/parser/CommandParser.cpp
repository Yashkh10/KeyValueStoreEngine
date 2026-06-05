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
    std::transform(str.begin(),str.end(),str.begin(),[](unsigned char c){
        return std::tolower(c);
    });

    return str;
}

CommandData CommandParser::parse(const std::string& input){
    std::vector<std::string> parts =splitCommand(input);
    if(parts.empty()){
        return {"invalid",{}};
    }

    std::string operation =toLower(parts[0]);

    if(operation=="get"){
        if(parts.size()!=2){
            return {"invalid",{}};
        }
        return {"get",{parts[1]}};
    }

    if(operation=="del"){
        if(parts.size()!=2){
            return {"invalid",{}};
        }
        return {"del",{parts[1]}};
    }

    if(operation=="ttl"){
        if(parts.size()!=2){
            return {"invalid",{}};
        }
        return {"ttl",{parts[1]}};
    }

  
    if(operation=="quit"){
        return {"quit",{}};
    }

    
    if(operation=="set"){
        if(parts.size() < 3){
            return {"invalid",{}};
        }

        std::string key = parts[1];
        bool hasTTL = false;
        int exIndex = -1;
        if(parts.size() >= 5){
            if(toLower(parts[parts.size() - 2])=="ex"){
                hasTTL = true;
                exIndex = static_cast<int>(parts.size()) - 2;
            }
        }

        std::ostringstream valueBuilder;
        size_t valueStart = 2;
        size_t valueEnd = hasTTL ? exIndex : parts.size();

        for(size_t i = valueStart;i < valueEnd;++i){
            valueBuilder << parts[i];
            if(i + 1 < valueEnd){
                valueBuilder << " ";
            }
        }

        std::string value = valueBuilder.str();
        if(hasTTL){
            std::string ttl = parts.back();
            return {"setex",{key,value,ttl}
            };
        }
        return {"set",{key,value}};
    }

    return {"invalid",{}};
}