#include "SubscribeCommand.h"

SubscribeCommand::SubscribeCommand(const std::string& channel)
    : channel(channel) {}

std::string SubscribeCommand::execute(CommandContext& ctx) {
    ctx.pubsub.subscribe(channel, ctx.clientSocket);
    return "OK";
}