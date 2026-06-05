#include "Server.h"
#include "../parser/RESPParser.h"
#include "../protocol/RESPSerializer.h"

#include <iostream>
#include <vector>
#include <algorithm>

Server::Server(int port): port(port),serverSocket(INVALID_SOCKET){}

bool Server::start(){
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0){
        std::cout<<"WSAStartup failed\n";
        return false;
    }

    serverSocket =socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(serverSocket==INVALID_SOCKET){
        std::cout<<"Socket creation failed\n";
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket,(sockaddr*)&serverAddr,sizeof(serverAddr))==SOCKET_ERROR){
        std::cout<<"Bind failed\n";
        cleanup();
        return false;
    }

    if(listen(serverSocket,SOMAXCONN)==SOCKET_ERROR){
        std::cout<<"Listen failed\n";
        cleanup();
        return false;
    }

    std::cout<< "Server listening on port "<< port<< "\n";
    return true;
}

void Server::run(){
    std::vector<SOCKET> clients;
    std::vector<std::string> buffers;

    while(true){
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(serverSocket,&readfds);

        for(SOCKET client : clients){
            FD_SET(client,&readfds);
        }

        int activity = select(0,&readfds,nullptr,nullptr,nullptr);
        if(activity==SOCKET_ERROR){
            std::cout<<"select() failed\n";
            continue;
        }

        if(FD_ISSET(serverSocket,&readfds)){
            SOCKET clientSocket = accept(serverSocket,nullptr,nullptr);
            if(clientSocket != INVALID_SOCKET){
                clients.push_back(clientSocket);
                buffers.push_back("");
                std::cout<<"Client connected\n";
            }
        }

        for(size_t i = 0;i<clients.size();){
            SOCKET clientSocket = clients[i];

            if(!FD_ISSET(clientSocket,&readfds)){
                i++;
                continue;
            }

            char buffer[4096];
            memset(buffer,0,sizeof(buffer));

            int bytesReceived = recv(clientSocket,buffer,sizeof(buffer)- 1,0);
            if(bytesReceived <= 0){
                std::cout<<"Client disconnected\n";
                closesocket(clientSocket);
                clients.erase(clients.begin()+ i);
                buffers.erase(buffers.begin()+ i);
                continue;
            }

            buffer[bytesReceived] = '\0';
            buffers[i] += std::string(buffer);

            while(true){
                CommandData data;
                bool hasCommand = false;

                if(!buffers[i].empty() && buffers[i][0] == '*'){

                    if(!RESPParser::hasCompleteMessage(buffers[i])){
                        break;
                    }

                    std::vector<std::string> tokens = RESPParser::parse(buffers[i]);
                    buffers[i].clear();

                    if(tokens.empty()){
                        break;
                    }

                    data.operation = tokens[0];
                    data.args.assign(tokens.begin() + 1, tokens.end());

                    hasCommand = true;
                }
                else {

                    size_t pos = buffers[i].find('\n');
                    if(pos == std::string::npos){
                        break;
                    }

                    std::string input = buffers[i].substr(0,pos);
                    buffers[i].erase(0,pos+1);

                    input.erase(std::remove(input.begin(),input.end(),'\r'),input.end());

                    if(input.empty()){
                        continue;
                    }

                    data = parser.parse(input);
                    hasCommand = true;
                }

                if(!hasCommand){
                    break;
                }

                auto command = factory.createCommand(data);
                std::string raw = command->execute(storage);
                std::string response;

                if(raw == "OK"){
                    response = RESPSerializer::simpleString(raw);
                }
                else if(raw == "Data not found"){
                    response = "$-1\r\n";
                }
                else if(raw == "Bye"){
                    response = RESPSerializer::simpleString(raw);
                }
                else{
                    response = RESPSerializer::bulkString(raw);
                }

                send(clientSocket,response.c_str(),static_cast<int>(response.size()),0);

                if(data.operation == "quit"){
                    closesocket(clientSocket);
                    clients.erase(clients.begin()+ i);
                    buffers.erase(buffers.begin()+ i);
                    std::cout<<"Client quit\n";
                    goto next_client;
                }
            }

            i++;

            next_client:;
        }
    }
}

void Server::cleanup(){
    if(serverSocket!=INVALID_SOCKET){
        closesocket(serverSocket);
    }
    WSACleanup();
}