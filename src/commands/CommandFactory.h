#pragma once

#include <memory>
#include "ICommand.h"
#include "../types/CommandData.h"

class CommandFactory {
public:
    static std::unique_ptr<ICommand> createCommand(const CommandData& data);
};