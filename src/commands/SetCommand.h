#pragma once

#include "ICommand.h"
#include <string>

class SetCommand : public ICommand {
private:
    std::string key;
    std::string value;

public:
    SetCommand(const std::string& key, const std::string& value);
    std::string execute(CommandContext& ctx) override;
};