#include "RESPSerializer.h"

std::string RESPSerializer::simpleString(const std::string& value){
    return "+" + value + "\r\n";
}

std::string RESPSerializer::bulkString(const std::string& value){
    return "$" +std::to_string(value.size()) +"\r\n" +value +"\r\n";
}

std::string RESPSerializer::error(const std::string& value){
    return "-" + value + "\r\n";
}