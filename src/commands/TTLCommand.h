#pragma once

#include "ICommand.h"
#include <string>

class TTLCommand : public ICommand {
private:
    std::string key;

public:
    explicit TTLCommand(const std::string& key);

    std::string execute(CommandContext& ctx) override;
};