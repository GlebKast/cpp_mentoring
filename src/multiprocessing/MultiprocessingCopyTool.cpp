#include "MultiprocessingCopyTool.h"

using namespace multiprocessing;

MultiprocessingCopyTool::~MultiprocessingCopyTool()
{
    boost::interprocess::message_queue::remove(m_mqChunkReadyName.c_str());
}

MultiprocessingCopyTool::MultiprocessingCopyTool() : m_chunk(CHUNK_SIZE) {}

ReturnCode MultiprocessingCopyTool::copy(int argc, char *const argv[])
{
    if(argc != 5)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <command> <source> <destination> <shared memory name>" << std::endl;
        return ReturnCode::InvalidArguments;
    }

    std::string command(argv[1]);
    std::string source(argv[2]);
    std::string destination(argv[3]);
    m_mqChunkReadyName = argv[4];

    if(source == destination)
    {
        std::cout << "Source and destination are the same. Exiting." << std::endl;
        return ReturnCode::InvalidArguments;
    }

    if(command == "read")
    {
        read(source);
    }
    else if(command == "write")
    {
        write(destination);
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
        return ReturnCode::InvalidArguments;
    }

    return ReturnCode::Success;
}

ReturnCode MultiprocessingCopyTool::read(const std::string &sourceName)
{
    std::ifstream file(sourceName, std::ios::binary);

    boost::posix_time::ptime timeout;

    auto mqChunkReady = boost::interprocess::message_queue(
      boost::interprocess::open_or_create, m_mqChunkReadyName.c_str(), 1, CHUNK_SIZE);

    while(file)
    {
        file.read(m_chunk.data(), CHUNK_SIZE);

        if(file.gcount() < CHUNK_SIZE)
        {
            m_chunk.resize(file.gcount());
        }

        timeout = boost::posix_time::second_clock::universal_time() + boost::posix_time::seconds(5);

        if(mqChunkReady.timed_send(m_chunk.data(), m_chunk.size(), 0, timeout) == false)
        {
            return ReturnCode::ReadError;
        }
    }

    char lastChunk[] = "";
    std::size_t lastChunkSize = 0;

    mqChunkReady.send(lastChunk, lastChunkSize, 0);

    while(mqChunkReady.get_num_msg() != 0 || timeout > boost::posix_time::second_clock::universal_time())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return ReturnCode::Success;
}

ReturnCode MultiprocessingCopyTool::write(const std::string &targetName)
{
    std::ofstream file(targetName, std::ios::binary);

    boost::posix_time::ptime timeout;

    std::size_t recv_size = 0;
    unsigned int priority = 0;

    auto mqChunkReady = boost::interprocess::message_queue(
      boost::interprocess::open_or_create, m_mqChunkReadyName.c_str(), 1, CHUNK_SIZE);

    while(true)
    {
        timeout =
          boost::posix_time::microsec_clock::universal_time() + boost::posix_time::seconds(5);

        if(
          mqChunkReady.timed_receive(m_chunk.data(), CHUNK_SIZE, recv_size, priority, timeout) ==
          false)
        {
            return ReturnCode::WriteError;
        }

        file.write(m_chunk.data(), recv_size);

        if(recv_size == 0)
        {
            return ReturnCode::Success;
        }
    }

    return ReturnCode::Success;
}