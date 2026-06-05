#include "SetWithTTLCommand.h"

SetWithTTLCommand::SetWithTTLCommand(const std::string& key,const std::string& value,int ttlSeconds): key(key),value(value),
ttlSeconds(ttlSeconds){}

std::string SetWithTTLCommand::execute(StorageEngine& storage){
    storage.setWithTTL(key,value,ttlSeconds);
    return "OK";
}