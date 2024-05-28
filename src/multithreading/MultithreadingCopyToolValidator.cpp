#include "MultithreadingCopyToolValidator.h"

namespace multithreading
{
bool MultithreadingCopyToolValidator::validateUserInput(int argc, char *const argv[])
{
    spdlog::trace("MultithreadingCopyToolValidator::validateUserInput(argc, argv)");
    
    if(argc != 3)
    {
        spdlog::error(
          "Wrong arguments, please follow this format: {} <source file name> <target file name>",
          argv[0]);
        return false;
    }

    if(! std::filesystem::exists(argv[1]))
    {
        spdlog::error("File to copy doesn't exist.");
        return false;
    }

    std::filesystem::path filePath(argv[2]);
    if(filePath.has_parent_path() && ! std::filesystem::exists(filePath.parent_path()))
    {
        spdlog::error(
          "Parent path for target file doesn't exist: {}", filePath.parent_path().string());
        return false;
    }

    if(argv[1] == argv[2])
    {
        spdlog::error("Source and target file names are the same.");
        return false;
    }

    return true;
}

}    // namespace multithreading