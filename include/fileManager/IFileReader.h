#pragma once

#include "Common.h"

namespace fileManager
{

class IFileReader
{
public:
    virtual ~IFileReader() = default;

    virtual bool read(char *s, std::size_t bufferSize, std::size_t &actualSize) = 0;
    virtual bool isEndOfFile() const = 0;
};

}    // namespace fileManager
