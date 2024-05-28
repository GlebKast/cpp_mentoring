#pragma once

#include <spdlog/spdlog.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include <vector>
#include <algorithm>

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