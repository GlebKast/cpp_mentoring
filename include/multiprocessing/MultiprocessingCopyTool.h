/*
Your task is to rework the copy tool in the next manner:
1. Your tool should now accept 3 parameters <source> <destination> <name of shared memory object>
2. When run your tool by script twice.
2.1. First exemplar of executed tools should act as a reader of source file
2.2. Second exemplar of executed tools should act as a writer of destination file
3. Use 'placement' new for organize reading to shared memory. As well as writing.
4. Try to use constructor/destructor of a class - to notify about capture/release memory after read/write operation completion
Discover how to organize interprocess synchronization (windows, Linux, MacOS)
*/

#pragma once
#include "../ICopyTool.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>

namespace multiprocessing
{
class MultiprocessingCopyTool : public ICopyTool
{
public:
    MultiprocessingCopyTool();

    ~MultiprocessingCopyTool();

    ReturnCode copy(int argc, char *const argv[]) override;

    ReturnCode read(const std::string &sourceName) override;

    ReturnCode write(const std::string &targetName) override;

private:
    std::vector<char> m_chunk;
    std::string m_mqChunkReadyName;
};
}    // namespace multiprocessing

// add class for controling
// add mutex on shared memory
// boost options reading
