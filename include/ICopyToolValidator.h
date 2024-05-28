#pragma once

#include "Common.h"

class ICopyToolValidator
{
public:
    virtual ~ICopyToolValidator() = default;

    virtual bool validateUserInput(int argc, char *const argv[]) = 0;
};