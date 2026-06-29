#pragma once

#include "ICommand.h"

class GetCommand : public ICommand {
private:
    std::string key;

public:
    GetCommand(const std::string& key);
    std::string execute(CommandContext& ctx) override;
};