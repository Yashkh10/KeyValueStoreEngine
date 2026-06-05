#pragma once

#include "ICommand.h"

class InvalidCommand : public ICommand {
public:
    std::string execute(StorageEngine& storage) override;
};