#pragma once

#include "Common.h"

class ICopyTool
{
public:
    virtual ~ICopyTool() = default;

    virtual ReturnCode copy(int argc, char *const argv[]) = 0;
    virtual ReturnCode read() = 0;
    virtual ReturnCode write() = 0;
};