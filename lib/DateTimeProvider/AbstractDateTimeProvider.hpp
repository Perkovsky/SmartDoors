#pragma once

#include <String.h>

class AbstractDateTimeProvider {
protected:
    virtual ~AbstractDateTimeProvider() = default;

public:
   virtual String getTimestamp() = 0;
   virtual String getDateTime() = 0;
   virtual String getDate() = 0;
   virtual String getTime() = 0;
};