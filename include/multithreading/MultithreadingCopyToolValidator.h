#pragma once

#include "ICopyToolValidator.h"

namespace multithreading
{

class MultithreadingCopyToolValidator : public ICopyToolValidator
{
public:
    bool validateUserInput(int argc, char *const argv[]) override;
};

}    // namespace multithreading