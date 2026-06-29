#include "PublishCommand.h"

PublishCommand::PublishCommand(const std::string& channel,const std::string& message): channel(channel), message(message) {}

std::string PublishCommand::execute(CommandContext& ctx) {
    ctx.pubsub.publish(channel, message);
    return "OK";
}