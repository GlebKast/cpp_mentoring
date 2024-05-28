#pragma once

#include "ICopyToolValidator.h"

namespace multiprocessing
{
    
class MultiprocessingCopyToolValidator : public ICopyToolValidator
{
public:
    bool validateUserInput(int argc, char *const argv[]) override;
};

}    // namespace multiprocessing