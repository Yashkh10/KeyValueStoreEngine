#pragma once

#include <string>

class RESPSerializer{
public:
    static std::string simpleString(const std::string& value);
    static std::string bulkString(const std::string& value);
    static std::string error(const std::string& value);
};