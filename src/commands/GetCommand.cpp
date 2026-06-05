#include "GetCommand.h"

GetCommand::GetCommand(const std::string& key)
    : key(key) {}

std::string GetCommand::execute(StorageEngine& storage) {
    return storage.get(key);
}