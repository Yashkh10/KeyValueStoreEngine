#pragma once

#include <string>
#include <vector>
#include "../types/CommandData.h"

class CommandParser {
public:
    static CommandData parse(const std::string& input);

private:
    static std::vector<std::string> splitCommand(const std::string& input);
    static std::string toLower(std::string str);
};