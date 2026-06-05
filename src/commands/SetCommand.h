#pragma once

#include "ICommand.h"

class SetCommand : public ICommand {
private:
    std::string key;
    std::string value;

public:
    SetCommand(const std::string& key, const std::string& value);
    std::string execute(StorageEngine& storage) override;
};