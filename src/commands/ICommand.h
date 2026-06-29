#pragma once

#include <string>
#include "../server/CommandContext.h"

class ICommand {
public:
    virtual std::string execute(CommandContext& ctx) = 0;
    virtual ~ICommand() = default;
};