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

#include "common.h"

namespace multiprocessing
{
class SharedMemoryBuffer
{
public:
    SharedMemoryBuffer(boost::interprocess::mapped_region &region) : mRegion(region)
    {
        std::cout << "Shared memory is captured." << std::endl;
    }

    ~SharedMemoryBuffer() { std::cout << "Shared memory is released." << std::endl; }

    char *data() { return static_cast<char *>(mRegion.get_address()); }

private:
    boost::interprocess::mapped_region &mRegion;
};

void reader(const std::string &fileName, const std::string &sharedMemoryName)
{
    std::ifstream file(fileName, std::ios::binary);

    boost::interprocess::shared_memory_object shm(
      boost::interprocess::open_or_create, sharedMemoryName.c_str(),
      boost::interprocess::read_write);
    shm.truncate(CHUNK_SIZE);
    boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
    SharedMemoryBuffer buffer(region);

    std::string mqChunkReadyName = "mqChunkReady" + sharedMemoryName;
    std::string mqChunkSizeName = "mqChunkSize" + sharedMemoryName;

    boost::interprocess::message_queue::remove(mqChunkReadyName.c_str());
    boost::interprocess::message_queue mqChunkReady(
      boost::interprocess::create_only, mqChunkReadyName.c_str(), 1, sizeof(int));

    boost::interprocess::message_queue::remove(mqChunkSizeName.c_str());
    boost::interprocess::message_queue mqChunkSize(
      boost::interprocess::create_only, mqChunkSizeName.c_str(), 1, sizeof(int));

    int payload = 0;
    size_t recv_size = 0;
    unsigned int priority = 0;
    boost::posix_time::ptime timeout;

    while(file)
    {
        file.read(buffer.data(), CHUNK_SIZE);

        payload = file.gcount();
        mqChunkSize.send(&payload, sizeof(payload), 0);

        timeout =
          boost::posix_time::second_clock::universal_time() + boost::posix_time::seconds(5);

        if(! mqChunkReady.timed_receive(&payload, sizeof(payload), recv_size, priority, timeout))
        {
            return;
        }
    }
    payload = -1;
    mqChunkSize.send(&payload, sizeof(payload), 0);
}

void writer(const std::string &fileName, const std::string &sharedMemoryName)
{
    std::ofstream file(fileName, std::ios::binary);

    boost::interprocess::shared_memory_object shm(
      boost::interprocess::open_or_create, sharedMemoryName.c_str(),
      boost::interprocess::read_write);
    boost::interprocess::mapped_region region(shm, boost::interprocess::read_write);
    SharedMemoryBuffer buffer(region);

    std::string mqChunkReadyName = "mqChunkReady" + sharedMemoryName;
    std::string mqChunkSizeName = "mqChunkSize" + sharedMemoryName;

    boost::interprocess::message_queue mqChunkReady(boost::interprocess::open_only, mqChunkReadyName.c_str());

    boost::interprocess::message_queue mqChunkSize(boost::interprocess::open_only, mqChunkSizeName.c_str());

    auto start = std::chrono::system_clock::now();

    int payload = 0;
    size_t recv_size = 0;
    unsigned int priority = 0;
    boost::posix_time::ptime timeout;

    while(true)
    {
        timeout =
          boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(5);

        if(! mqChunkSize.timed_receive(&payload, sizeof(payload), recv_size, priority, timeout))
        {
            return;
        }

        if(payload == -1)
        {
            return;
        }

        file.write(buffer.data(), payload);

        mqChunkReady.send(&payload, sizeof(payload), 0);
    }
}
}    // namespace multiprocessing

void runMutiprocessingCopyTool(int argc, char *const argv[])
{
    if(argc != 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <command> <source> <destination> <shared memory name>" << std::endl;
        return;
    }

    std::string command(argv[1]);
    std::string source(argv[2]);
    std::string destination(argv[3]);
    std::string sharedMemoryName(argv[4]);

    if(source == destination)
    {
        std::cout << "Source and destination are the same. Exiting." << std::endl;
        return;
    }

    if(command == "read")
    {
        multiprocessing::reader(source, sharedMemoryName);
    }
    else if(command == "write")
    {
        multiprocessing::writer(destination, sharedMemoryName);
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
        return;
    }
}