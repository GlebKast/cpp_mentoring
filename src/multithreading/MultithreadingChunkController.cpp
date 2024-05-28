#include "MultithreadingChunkController.h"

namespace multithreading
{

MultithreadingChunkController::MultithreadingChunkController() :
    m_completed(false), m_chunkReady(false)
{
    spdlog::trace("MultithreadingChunkController::MultithreadingChunkController()");
}

MultithreadingChunkController::~MultithreadingChunkController()
{
    spdlog::trace("MultithreadingChunkController::~MultithreadingChunkController()");
}

bool MultithreadingChunkController::get(char *chunk, std::size_t &actualSize)
{
    spdlog::trace("MultithreadingChunkController::get()");

    std::unique_lock lk(m_chunkMutex);

    m_chunkCV.wait(lk, [this] { return m_chunkReady == true || m_completed == true; });

    m_chunkReady = false;

    lk.unlock();

    m_chunkCV.notify_one();

    return true;
}

bool MultithreadingChunkController::put(const char *chunk, std::size_t actualSize)
{
    spdlog::trace("MultithreadingChunkController::put()");

    std::unique_lock lk(m_chunkMutex);

    m_chunkReady = true;

    lk.unlock();

    m_chunkCV.notify_one();

    lk.lock();

    m_chunkCV.wait(lk, [this] { return m_chunkReady == false; });

    return true;
}

bool MultithreadingChunkController::notifyCompleted()
{
    spdlog::trace("MultithreadingChunkController::notifyCompleted()");

    m_completed = true;
    m_chunkCV.notify_one();

    return true;
}

bool MultithreadingChunkController::isCompleted() const
{
    spdlog::trace("MultithreadingChunkController::isCompleted()");
    
    return m_completed;
}

}    // namespace multithreading