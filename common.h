#pragma once

#include <iostream>
#include <string>
#include <mutex>
#include <future>
#include <vector>
#include <condition_variable>
#include <fstream>
#include <atomic>
#include <filesystem>

const std::size_t CHUNK_SIZE = 1024;

enum class ReturnCode
{
    Success = 0,
    ReadError,
    WriteError,
    CopyError
};
