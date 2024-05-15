#pragma once

#include <atomic>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

constexpr std::size_t CHUNK_SIZE = 1024;
constexpr std::chrono::seconds TIMEOUT(5);

enum class ReturnCode
{
    Success = 0,
    ReadError,
    WriteError,
    CopyError
};
