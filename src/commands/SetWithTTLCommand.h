#pragma once

#include "ICommand.h"
#include <string>

class SetWithTTLCommand : public ICommand {
private:
    std::string key;
    std::string value;
    int ttlSeconds;

public:
    SetWithTTLCommand(const std::string& key,const std::string& value,int ttlSeconds);
    std::string execute(StorageEngine& storage) override;
};