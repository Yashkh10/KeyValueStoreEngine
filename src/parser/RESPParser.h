#pragma once

#include <string>
#include <vector>

class RESPParser {
public:
    static bool isRESP(const std::string& input);

    static bool hasCompleteMessage(const std::string& buffer);

    static std::string extractMessage(std::string& buffer);

    static std::vector<std::string> parse(const std::string& input);
};