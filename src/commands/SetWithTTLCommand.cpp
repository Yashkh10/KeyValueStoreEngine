#include "SetWithTTLCommand.h"

SetWithTTLCommand::SetWithTTLCommand(const std::string& key,const std::string& value,int ttlSeconds): key(key),value(value),
ttlSeconds(ttlSeconds){}

std::string SetWithTTLCommand::execute(CommandContext& ctx){
    ctx.storage.setWithTTL(key,value,ttlSeconds);
    return "OK";
}