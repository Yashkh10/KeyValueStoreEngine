#include "TTLCommand.h"

TTLCommand::TTLCommand(const std::string& key): key(key){}

std::string TTLCommand::execute(CommandContext& ctx){
    return std::to_string(ctx.storage.ttl(key));
}