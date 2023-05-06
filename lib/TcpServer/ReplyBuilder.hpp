#pragma once

#include <String.h>

class ReplyBuilder {
private:
    const String& _tag;
    const String& _value;
    String _result = "";

public:
    ReplyBuilder(const String& tag, const String& value): _tag(tag), _value(value) {
        _result += "<";
        _result += tag;
    }

    ReplyBuilder* addAttribute(const String& attribute, const String& value) {
        _result += " ";
        _result += attribute;
        _result += "=\"";
        _result += value;
        _result += "\"";
        return this;
    }

    String build() {
        _result += ">";
        _result += _value;
        _result += "</";
        _result += _tag;
        _result += ">\r\n";
        return _result;
    }
};