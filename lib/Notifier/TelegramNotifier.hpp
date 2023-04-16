#pragma once

#include <String.h>
#include <HTTPClient.h>

class TelegramNotifier {
private:
    const String& _botId;
    const String& _chatId;

public:
    TelegramNotifier(const String& botId, const String& chatId)
        : _botId(botId), _chatId(chatId) {}

    void notify(const String& message) {
        HTTPClient httpClient;

        String url("https://api.telegram.org/bot");
        url.reserve(_botId.length() + _chatId.length() + message.length() + 56);
        url += _botId;
        url += "/sendMessage?chat_id=";
        url += _chatId;
        url += "&text=";
        url += message;

        httpClient.begin(url.c_str());
        int httpResponseCode = httpClient.GET();
        httpClient.end();
    }
};