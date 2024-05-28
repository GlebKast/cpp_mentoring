#pragma once

#include "Common.h"
#include "IChunkController.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

namespace multiprocessing
{

class MultiprocessingChunkController : public IChunkController
{
public:
    MultiprocessingChunkController(const std::string &sharedMemoryName);

    virtual ~MultiprocessingChunkController() override;

    virtual bool get(char *chunk, std::size_t &actualSize) override;
    virtual bool put(const char *chunk, std::size_t actualSize) override;

    virtual bool notifyCompleted() override;
    virtual bool isCompleted() const override;

private:
    boost::interprocess::message_queue m_mqChunkReady;
    std::vector<char> m_chunk;
    std::string m_sharedMemoryName;
    bool m_isChunkReady;
    bool m_isCompleted;
};

}    // namespace multiprocessing