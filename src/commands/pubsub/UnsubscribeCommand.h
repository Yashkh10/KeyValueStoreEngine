#pragma once

#include "../ICommand.h"
#include <string>

class UnsubscribeCommand : public ICommand {
private:
    std::string channel;

public:
    explicit UnsubscribeCommand(const std::string& channel);
    std::string execute(CommandContext& ctx) override;
};