#pragma once

#include <map>
#include <String.h>
#include <tinyxml2.h>
#include "LoggerFactory.hpp"

class CommandParser {
private:
    LoggerFactory& _logger;
    String _commandName;
    String _commandValue;
    std::map<String, String> _attributes;

public:
    CommandParser(LoggerFactory& logger) : _logger(logger) {}

    String getCommandName() {
        return _commandName;
    }

    String getCommandValue() {
        return _commandValue;
    }

    std::map<String, String>& getAttributes() {
       return _attributes;
    }

     String getAttributeValue(const String& key) {
        return _attributes[key];
    }

    void parse(char* xml) {
        tinyxml2::XMLDocument doc;
        doc.Parse(xml);

        if (doc.Error()) {
            _logger.logWarning("Error parsing XML");
            return;
        }

        tinyxml2::XMLElement* root = doc.RootElement();
        if (!root) {
            _logger.logWarning("Error parsing XML: root is empty");
            return;
        }

        _commandName = root->Name();
        _commandValue = root->GetText();

        const tinyxml2::XMLAttribute* attribute = root->FirstAttribute();
        while (attribute) {
            _attributes[attribute->Name()] = attribute->Value();
            attribute = attribute->Next();
        }
    }
};