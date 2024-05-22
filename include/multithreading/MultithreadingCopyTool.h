/*
Your task is to implement simple 2 threaded "copy" tool.

Tool should be implemented as a console application.
Tool should accept 2 parameters source filename and target filename.
Copying logic should be organized with help of 2 threads.
First thread should read the data from source file.
Second thread should write the data to the target file.
*/

#pragma once
#include "../ICopyTool.h"

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>

namespace multithreading
{

class MultithreadingCopyTool : public ICopyTool
{
public:
    MultithreadingCopyTool();

    ReturnCode copy(int argc, char *const argv[]) override;

    ReturnCode read(const std::string &sourceName) override;

    ReturnCode write(const std::string &targetName) override;

private:
    std::vector<char> m_chunk;
    std::atomic<bool> m_completed;
    std::atomic<bool> m_chunkReady;
    std::mutex m_chunkMutex;
    std::condition_variable m_chunkCV;
};
}    // namespace multithreading