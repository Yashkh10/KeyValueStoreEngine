#pragma once

#include "../ICommand.h"
#include <string>

class PublishCommand : public ICommand {
private:
    std::string channel;
    std::string message;

public:
    PublishCommand(const std::string& channel,const std::string& message);
    std::string execute(CommandContext& ctx) override;
};