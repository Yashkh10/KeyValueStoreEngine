#include "DelCommand.h"

DelCommand::DelCommand(const std::string& key): key(key) {}

std::string DelCommand::execute(CommandContext& ctx) {
    bool deleted = ctx.storage.del(key);
    if(deleted){
        return "Deleted";
    }
    return "Data not found";
}