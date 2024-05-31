#pragma once

#include "Common.h"
#include "IChunkController.h"

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>

namespace multithreading
{

class MultithreadingChunkController : public IChunkController
{
public:
    MultithreadingChunkController();
    virtual ~MultithreadingChunkController() override;

    bool get(char *chunk, std::size_t &actualSize) override;
    bool put(const char *chunk, std::size_t actualSize) override;

    bool notifyCompleted() override;
    bool isCompleted() const override;

private:
    std::atomic<bool> m_completed;
    std::atomic<bool> m_chunkReady;
    std::vector<char> m_chunk;
    std::mutex m_chunkMutex;
    std::condition_variable m_chunkCV;
};

}    // namespace multithreading