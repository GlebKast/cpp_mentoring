#pragma once

#include "Common.h"

class ICopyTool
{
public:
    virtual ~ICopyTool() = default;
    ICopyTool() = default;

    ICopyTool(const ICopyTool &) = delete;
    ICopyTool &operator=(const ICopyTool &) = delete;

    ICopyTool(ICopyTool &&) = delete;
    ICopyTool &operator=(ICopyTool &&) = delete;

    virtual ReturnCode copy(int argc, char *const argv[]) = 0;
    virtual ReturnCode read(const std::string &source) = 0;
    virtual ReturnCode write(const std::string &target) = 0;
};