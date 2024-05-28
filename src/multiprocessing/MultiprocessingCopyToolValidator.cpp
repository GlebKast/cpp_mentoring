#include "MultiprocessingCopyToolValidator.h"

namespace multiprocessing
{
bool MultiprocessingCopyToolValidator::validateUserInput(int argc, char *const argv[])
{
    spdlog::trace("MultiprocessingCopyToolValidator::validateUserInput(argc, argv)");

    if(argc != 5)
    {
        spdlog::error(
          "Wrong arguments, please follow this format: {} <read/write> <source file name> <target "
          "file name> <shared memory name>",
          argv[0]);
        return false;
    }

    if(strcmp(argv[1], "read") != 0 && strcmp(argv[1], "write") != 0)
    {
        spdlog::error("Unknown command: {}", argv[1]);
        return false;
    }

    if(! std::filesystem::exists(argv[2]))
    {
        spdlog::error("File to copy doesn't exist.");
        return false;
    }

    std::filesystem::path filePath(argv[3]);
    if(filePath.has_parent_path() && ! std::filesystem::exists(filePath.parent_path()))
    {
        spdlog::error(
          "Parent path for target file doesn't exist: {}", filePath.parent_path().string());
        return false;
    }

    if(argv[2] == argv[3])
    {
        spdlog::error("Source and target file names are the same.");
        return false;
    }

    return true;
}

}    // namespace multiprocessing