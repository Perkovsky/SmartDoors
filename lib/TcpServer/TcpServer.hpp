#pragma once

#include <String.h>
#include <AsyncTCP.h>
#include "LoggerFactory.hpp"
#include "AbstractTcpCommandProcessor.hpp"

class TcpServer {
private:
    AbstractTcpCommandProcessor& _tcpCommandProcessor;
    LoggerFactory& _logger;
    AsyncServer* _server = NULL;
    AsyncClient* _client = NULL;

    void onClientConnect(AsyncClient* newClient) {
        if (_client) {
            newClient->write("Only one connection is allowed at a time\r\n");
            newClient->close();
            return;
        }

        _client = newClient;
        _logger.logInfo(F("TCP Client has been connected"));

        newClient->onDisconnect([this](void* data, AsyncClient* c) {
            _logger.logInfo(F("TCP Client has been disconnected"));
            _client = NULL;
        });

        newClient->onData([this](void*, AsyncClient* c, void* data, size_t len) {
            char* command = (char*)data;
            command[len-1] = '\0'; // trim \n
            command[len-2] = '\0'; // trim \r
            String reply = _tcpCommandProcessor.getReply(command);
            c->write(reply.c_str());
        }, NULL);
    }

public:
    TcpServer(AbstractTcpCommandProcessor& tcpCommandProcessor, LoggerFactory& logger)
        : _tcpCommandProcessor(tcpCommandProcessor), _logger(logger) {}

    void bebin(uint16_t port) {
        _server = new AsyncServer(port);
        
        _server->onClient([](void* arg, AsyncClient* newClient) {
            TcpServer* server = reinterpret_cast<TcpServer*>(arg);
            server->onClientConnect(newClient);
        }, this);
        
        _server->begin();
        _logger.logInfo(F("TCP Server has been started"));
    }

    bool sendMessageToClient(const String& message) {
        if (!_client)
            return false;

        _client->write(message.c_str());
        return true;
    }

    void end() {
        _server->end();
        _logger.logInfo(F("TCP Server has been ended"));
    }

    void stopClient() {
        if (!_client)
            return;
        
        _client->stop();
        _client = NULL;
        _logger.logInfo(F("TCP Client has been stopped"));
    }
};