#pragma once

#include <map>
#include <String.h>
#include "LoggerFactory.hpp"
#include "HardwareManager.hpp"
#include "AbstractTcpCommandProcessor.hpp"
#include "CommandParser.hpp"
#include "ReplyBuilder.hpp"

class SmartDoorsTcpCommandProcessor final : public AbstractTcpCommandProcessor {
private:
    HardwareManager& _hardwareManager;
    LoggerFactory& _logger;
    
    String getCommandReply(bool status, const String& value, const std::map<String, String>& attributes = std::map<String, String>()) {
        ReplyBuilder* replyBuilder = new ReplyBuilder(F("cmdreply"), value);
        replyBuilder = replyBuilder
            ->addAttribute("panelid", _hardwareManager.getPanelId())
            ->addAttribute("status", status ? "ack" : "nack");

         for (const auto& attribute : attributes) {
            replyBuilder = replyBuilder->addAttribute(attribute.first, attribute.second);
        }

        return replyBuilder->build();
    }

     String getError(const String& commandName, const String& error) {
        std::map<String, String> attributes = {
            {"error", error}
        };
        return getCommandReply(false, commandName, attributes);
    }

    bool isCommandValid(const String& command) {
        std::map<String, bool> commands = {
            {"pingpanel", true},
            {"opendoor", true},
            {"lightdoor", true},
            {"displaydoor", true}
        };
        return commands.count(command) > 0;
    }

    u_int8_t getDoor(const String& door) {
        return door.toInt();
    }

    bool isDoorExisting(const u_int8_t door) {
        return _hardwareManager.isDoorExisting(door);
    }

    bool handleOpenDoorCommand(const u_int8_t door) {
        _hardwareManager.unlock(door);
        return true;
    }

    bool handleLightDoorCommand(const u_int8_t door) {
        _hardwareManager.light(door);
        return true;
    }

    bool handleDisplayDoorCommand(const u_int8_t door, const String& lineValue, const String& text) {
        u_int8_t line = 1;
        if (!lineValue.isEmpty()) {
           line = lineValue.toInt();
        }

        if (line <= 0 || line > 2) {
           line = 1;
        }

        _hardwareManager.print(door, text, line - 1);
        return true;
    }

public:
    SmartDoorsTcpCommandProcessor(HardwareManager& hardwareManager, LoggerFactory& logger)
        : _hardwareManager(hardwareManager), _logger(logger) {}

    String getReply(char* command) override {
        CommandParser parser(_logger);
        parser.parse(command);

        bool status = true;
        const String commandName = parser.getCommandName();
        if (!isCommandValid(commandName)) {
            return F("unknown command\r\n");
        }

        if (commandName == "pingpanel") {
            return getCommandReply(status, commandName);
        }

        const String doorValue = parser.getAttributeValue("door");
        if (doorValue.isEmpty()) {
            return getError(commandName, F("door attribute is required"));
        }

        const u_int8_t door = getDoor(doorValue);
        if (!isDoorExisting(door)) {
            return getError(commandName, F("door does not exist"));
        }

        if (commandName == "opendoor") {
            status = handleOpenDoorCommand(door);
        }
        
        if (commandName == "lightdoor") {
            status = handleLightDoorCommand(door);
        }
        
        if (commandName == "displaydoor") {
            String text = parser.getCommandValue();
            if (text.isEmpty()) {
                return getError(commandName, F("text is required"));
            }
            if (text.length() > 16) {
                return getError(commandName, F("max text length is 16"));
            }
            String line = parser.getAttributeValue("line");
            status = handleDisplayDoorCommand(door, line, text);
        }

        std::map<String, String> attributes;
        attributes["door"] = door;
        return getCommandReply(status, commandName, attributes);
    }
};