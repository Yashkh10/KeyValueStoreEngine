#include "Server.h"
#include "../parser/RESPParser.h"
#include "../protocol/RESPSerializer.h"

#include <iostream>
#include <algorithm>
#include <cstring>

Server::Server(int port): port(port),serverSocket(INVALID_SOCKET){}

bool Server::start(){
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
        std::cout << "WSAStartup failed\n";
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(serverSocket==INVALID_SOCKET){
        std::cout << "Socket creation failed\n";
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))==SOCKET_ERROR){
        std::cout << "Bind failed\n";
        cleanup();
        return false;
    }

    if(listen(serverSocket, SOMAXCONN)==SOCKET_ERROR){
        std::cout << "Listen failed\n";
        cleanup();
        return false;
    }

    std::cout << "Server listening on port " << port << "\n";
    return true;
}

void Server::run(){
    while(true){
        fd_set readfds;
        FD_ZERO(&readfds);

        FD_SET(serverSocket, &readfds);

        SOCKET maxSock = serverSocket;

        for(SOCKET c : clients){
            FD_SET(c, &readfds);
            if(c > maxSock) maxSock = c;
        }

        int activity = select(0, &readfds, nullptr, nullptr, nullptr);

        if(activity==SOCKET_ERROR){
            std::cout << "select() failed\n";
            continue;
        }

        if(FD_ISSET(serverSocket, &readfds)){
            handleNewClient();
        }

        for(size_t i = 0; i < clients.size();){
            SOCKET current = clients[i];
            if(FD_ISSET(current, &readfds)){
                handleClientData(i);
                if(i<clients.size() && clients[i]==current){
                    i++;
                }
            }
            else{
                i++;
            }
        }
    }
}

void Server::handleClientData(size_t i){
    SOCKET clientSocket = clients[i];

    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

    if(bytes <= 0){
        disconnectClient(i);
        return;
    }

    buffers[i].append(buffer, bytes);

    while(true){
        CommandData data;
        bool hasCommand = false;

        if(!buffers[i].empty() && buffers[i][0]=='*'){

            if(!RESPParser::hasCompleteMessage(buffers[i]))
                break;

            std::vector<std::string> tokens =RESPParser::parse(buffers[i]);

            if(tokens.empty())
                break;

            buffers[i].clear();

            data.operation = tokens[0];
            std::transform(data.operation.begin(),data.operation.end(),data.operation.begin(),[](unsigned char c){
                return std::tolower(c);
            }
);
            data.args.assign(tokens.begin() + 1, tokens.end());
            hasCommand = true;
        }

        else{
            size_t pos = buffers[i].find('\n');
            if(pos==std::string::npos)
                break;

            std::string input = buffers[i].substr(0, pos);
            buffers[i].erase(0, pos + 1);

            input.erase(std::remove(input.begin(), input.end(), '\r'),input.end());

            if(input.empty())
                continue;

            data = parser.parse(input);
            hasCommand = true;
        }

        if(!hasCommand)
            break;

        std::cout << "OP: [" << data.operation << "]\n";
        auto command = factory.createCommand(data);

        if(!command){
            sendResponse(clientSocket, "ERR unknown command");
            continue;
        }

        CommandContext ctx{
            storage,
            pubsub,
            clientSocket
        };

        std::string raw = command->execute(ctx);

        sendResponse(clientSocket, raw);

        if(data.operation=="quit"){
            disconnectClient(i);
            return;
        }
    }
}

void Server::disconnectClient(size_t i){
    SOCKET clientSocket = clients[i];

    pubsub.unsubscribeAll(clientSocket);

    closesocket(clientSocket);

    clients.erase(clients.begin() + i);
    buffers.erase(buffers.begin() + i);

    std::cout << "Client disconnected\n";
}

void Server::sendResponse(SOCKET client, const std::string& raw){
    std::string response;

    if(raw=="OK"){
        response = RESPSerializer::simpleString(raw);
    }
    else if(raw=="Data not found"){
        response = "$-1\r\n";
    }
    else if(raw=="Bye"){
        response = RESPSerializer::simpleString(raw);
    }
    else{
        response = RESPSerializer::bulkString(raw);
    }

    send(client, response.c_str(), (int)response.size(), 0);
}

void Server::cleanup(){
    for(SOCKET c : clients){
        closesocket(c);
    }

    if(serverSocket != INVALID_SOCKET){
        closesocket(serverSocket);
    }

    WSACleanup();
}

void Server::handleNewClient(){
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

    if(clientSocket==INVALID_SOCKET){
        std::cout << "Accept failed\n";
        return;
    }

    clients.push_back(clientSocket);
    buffers.push_back("");

    std::cout << "Client connected\n";
}