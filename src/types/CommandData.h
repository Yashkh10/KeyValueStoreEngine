#pragma once

#include <string>
#include <vector>

struct CommandData {
    std::string operation;
    std::vector<std::string> args;
};