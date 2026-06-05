#include "CommandFactory.h"

#include "SetCommand.h"
#include "SetWithTTLCommand.h"
#include "GetCommand.h"
#include "DelCommand.h"
#include "TTLCommand.h"
#include "QuitCommand.h"
#include "InvalidCommand.h"

#include <memory>

std::unique_ptr<ICommand> CommandFactory::createCommand(
    const CommandData& data){
    if(data.operation=="set"){
        return std::make_unique<SetCommand>(
            data.args[0],
            data.args[1]
        );
    }

    if(data.operation=="setex"){
        return std::make_unique<SetWithTTLCommand>(data.args[0],data.args[1],std::stoi(data.args[2]));
    }

    if(data.operation=="get"){
        return std::make_unique<GetCommand>(data.args[0]);
    }

    if(data.operation=="del"){
        return std::make_unique<DelCommand>(data.args[0]);
    }

    if(data.operation=="ttl"){
        return std::make_unique<TTLCommand>(data.args[0]);
    }

    if(data.operation=="quit"){
        return std::make_unique<QuitCommand>();
    }
    return std::make_unique<InvalidCommand>();
}