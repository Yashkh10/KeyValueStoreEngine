#include "TTLCommand.h"

TTLCommand::TTLCommand(const std::string& key): key(key){}

std::string TTLCommand::execute(StorageEngine& storage){
    return std::to_string(
        storage.ttl(key)
    );
}