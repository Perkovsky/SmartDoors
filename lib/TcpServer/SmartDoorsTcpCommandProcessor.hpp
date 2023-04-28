#pragma once

#include <map>
#include <String.h>
#include "LoggerFactory.hpp"
#include "SmartDoorsPanelSetup.hpp"
#include "AbstractTcpCommandProcessor.hpp"
#include "CommandParser.hpp"

class SmartDoorsTcpCommandProcessor final : public AbstractTcpCommandProcessor {
private:
    const SmartDoorsPanelSetup& _panel;
    LoggerFactory& _logger;
    
    void addAttribute(String& reply, const String& attribute, const String& value) {
        reply += " ";
        reply += attribute;
        reply += "=\"";
        reply += value;
        reply += "\"";
    }

    String getCommandReply(bool status, const String& value, const std::map<String, String>& attributes = std::map<String, String>()) {
        String reply("<cmdreply");

        //TODO: calculate reserve

        addAttribute(reply, "panelid", _panel.id);
        addAttribute(reply, "status", status ? "ack" : "nack");

        for (const auto& attribute : attributes) {
            addAttribute(reply, attribute.first, attribute.second);
        }

        reply += ">";
        reply += value;
        reply += "</cmdreply>\r\n";

        return reply;
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

    bool isDoorExisting(const String& doorValue) {
        int8_t door = doorValue.toInt();
        return _panel.doors.find(door) != _panel.doors.end();
    }

    bool handleOpenDoorCommand(const String& door) {
        // DO HARDWARE LOGIC
        return true;
    }

    bool handleLightDoorCommand(const String& door) {
        // DO HARDWARE LOGIC
        return true;
    }

    bool handleDisplayDoorCommand(const String& door, const String& lineValue, const String& text) {
        int8_t line = 1;
        if (!lineValue.isEmpty()) {
           line = lineValue.toInt();
        }

        if (line <= 0 || line > 2) {
           line = 1;
        }

        // DO HARDWARE LOGIC
        return true;
    }

public:
    SmartDoorsTcpCommandProcessor(const SmartDoorsPanelSetup& panel, LoggerFactory& logger)
        : _panel(panel), _logger(logger) {}

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

        const String door = parser.getAttributeValue("door");
        if (door.isEmpty()) {
            return getError(commandName, F("door attribute is required"));
        }

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