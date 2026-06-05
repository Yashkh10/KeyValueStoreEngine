#include "SetCommand.h"

SetCommand::SetCommand(const std::string& key, const std::string& value)
    : key(key), value(value) {}

std::string SetCommand::execute(StorageEngine& storage) {
    storage.set(key, value);

    return "OK";
}