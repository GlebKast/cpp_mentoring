#pragma once

#include "Common.h"

namespace fileManager
{

class IFileWriter
{
public:
    virtual ~IFileWriter() = default;
    virtual bool write(const char *s, std::size_t actualSize) = 0;
};

}    // namespace fileManager