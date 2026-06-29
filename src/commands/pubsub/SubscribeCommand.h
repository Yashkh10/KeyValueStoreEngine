#pragma once

#include "../ICommand.h"
#include <string>

class SubscribeCommand : public ICommand {
private:
    std::string channel;

public:
    explicit SubscribeCommand(const std::string& channel);
    std::string execute(CommandContext& ctx) override;
};