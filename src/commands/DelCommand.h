#pragma once

#include "ICommand.h"

class DelCommand : public ICommand {
private:
    std::string key;

public:
    DelCommand(const std::string& key);
    std::string execute(CommandContext& ctx) override;
};