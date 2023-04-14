#pragma once

#include <String.h>

class AbstractDateTimeProvider {
protected:
    virtual ~AbstractDateTimeProvider() = default;

public:
   virtual String getTimestamp() = 0;
};