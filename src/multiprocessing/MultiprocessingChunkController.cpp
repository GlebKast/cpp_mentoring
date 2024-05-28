#include "MultiprocessingChunkController.h"

namespace multiprocessing
{
    
MultiprocessingChunkController::MultiprocessingChunkController(
  const std::string &sharedMemoryName) :
    m_mqChunkReady(boost::interprocess::open_or_create, sharedMemoryName.c_str(), 1, CHUNK_SIZE),
    m_chunk(CHUNK_SIZE),
    m_sharedMemoryName(sharedMemoryName),
    m_isChunkReady(false),
    m_isCompleted(false)
{
    spdlog::trace("MultiprocessingChunkController::MultiprocessingChunkController()");
}

MultiprocessingChunkController::~MultiprocessingChunkController()
{
    spdlog::trace("MultiprocessingChunkController::~MultiprocessingChunkController()");

    boost::interprocess::message_queue::remove(m_sharedMemoryName.c_str());
}

bool MultiprocessingChunkController::get(char *chunk, std::size_t &actualSize)
{
    spdlog::trace("MultiprocessingChunkController::get()");

    boost::posix_time::ptime timeout =
      boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(5);

    unsigned int priority = 0;

    if(m_mqChunkReady.timed_receive(chunk, CHUNK_SIZE, actualSize, priority, timeout) == false)
    {
        return false;
    }

    if(actualSize == 0)
    {
        m_isCompleted = true;
    }

    return true;
}

bool MultiprocessingChunkController::put(const char *chunk, std::size_t actualSize)
{
    spdlog::trace("MultiprocessingChunkController::put()");

    boost::posix_time::ptime timeout =
      boost::posix_time::second_clock::universal_time() + boost::posix_time::seconds(5);

    if(m_mqChunkReady.timed_send(chunk, actualSize, 0, timeout) == false)
    {
        return false;
    }

    return true;
}

bool MultiprocessingChunkController::notifyCompleted()
{
    spdlog::trace("MultiprocessingChunkController::notifyCompleted()");

    m_chunk.clear();

    return put(m_chunk.data(), m_chunk.size());
}

bool MultiprocessingChunkController::isCompleted() const
{
    spdlog::trace("MultiprocessingChunkController::isCompleted()");

    return m_isCompleted;
}

}    // namespace multiprocessing