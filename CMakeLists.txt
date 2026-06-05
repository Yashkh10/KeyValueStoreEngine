cmake_minimum_required(VERSION 3.10)

project(KeyValueStoreEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(KeyValueStoreEngine
    src/main.cpp

    # Server
    src/server/Server.cpp

    # Parser
    src/parser/CommandParser.cpp
    src/parser/RESPParser.cpp

    # Storage
    src/storage/StorageEngine.cpp
    src/storage/Persistence.cpp

    # Protocol
    src/protocol/RESPSerializer.cpp

    # Commands
    src/commands/SetCommand.cpp
    src/commands/SetWithTTLCommand.cpp
    src/commands/GetCommand.cpp
    src/commands/DelCommand.cpp
    src/commands/TTLCommand.cpp
    src/commands/QuitCommand.cpp
    src/commands/InvalidCommand.cpp
    src/commands/CommandFactory.cpp
)

# Windows socket library
if (WIN32)
    target_link_libraries(KeyValueStoreEngine PRIVATE ws2_32)
endif()