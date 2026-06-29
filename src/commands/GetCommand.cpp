#include "GetCommand.h"

GetCommand::GetCommand(const std::string& key): key(key) {}

std::string GetCommand::execute(CommandContext& ctx) {
    return ctx.storage.get(key);
}