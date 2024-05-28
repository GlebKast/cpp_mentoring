#include "StreamFileWriter.h"

namespace fileManager
{
    StreamFileWriter::StreamFileWriter(const char *fileName)
    {
        spdlog::trace("StreamFileWriter::StreamFileWriter()");

        m_file.open(fileName, std::ofstream::binary);
    }

    bool StreamFileWriter::write(const char *s, std::size_t actualSize)
    {
        spdlog::trace("StreamFileWriter::write()");

        if (!m_file)
        {
            return false;
        }

        spdlog::info("Writing {} bytes. Contents: {}", actualSize, s);
        m_file.write(s, actualSize);

        return true;
    }

}    // namespace fileManager