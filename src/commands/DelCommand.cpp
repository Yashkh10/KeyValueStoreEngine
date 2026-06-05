#include "DelCommand.h"

DelCommand::DelCommand(const std::string& key)
    : key(key) {}

std::string DelCommand::execute(StorageEngine& storage) {
    bool deleted = storage.del(key);
    if(deleted){
        return "Deleted";
    }
    return "Data not found";
}