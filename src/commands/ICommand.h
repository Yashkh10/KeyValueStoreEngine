#pragma once

#include <string>
#include "../storage/StorageEngine.h"

class ICommand {
public:
    virtual std::string execute(StorageEngine& storage) = 0;
    virtual ~ICommand() = default;
};