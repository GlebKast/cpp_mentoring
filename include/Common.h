#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

constexpr std::size_t CHUNK_SIZE = 1024;
constexpr std::chrono::seconds TIMEOUT(5);

enum class ReturnCode
{
    Success = 0,
    ReadError,
    WriteError,
    CopyError,
    InvalidArguments
};
