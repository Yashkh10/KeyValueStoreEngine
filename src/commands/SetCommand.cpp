#include "SetCommand.h"

SetCommand::SetCommand(const std::string& key, const std::string& value): key(key), value(value) {}

std::string SetCommand::execute(CommandContext& ctx){
    ctx.storage.set(key, value);

    return "OK";
}