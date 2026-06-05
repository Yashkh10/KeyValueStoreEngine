#pragma once

#include <string>
#include <vector>

#include "../types/CommandData.h"

class CommandParser {
public:
    CommandData parse(const std::string& input);

private:
    std::vector<std::string> splitCommand(const std::string& input);

    std::string toLower(std::string str);
};