#include "UnsubscribeCommand.h"

UnsubscribeCommand::UnsubscribeCommand(const std::string& channel)
    : channel(channel) {}

std::string UnsubscribeCommand::execute(CommandContext& ctx) {
    ctx.pubsub.unsubscribe(channel, ctx.clientSocket);
    return "OK";
}