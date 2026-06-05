#pragma once

#include "ICommand.h"

class QuitCommand : public ICommand {
public:
    std::string execute(StorageEngine& storage) override;
};