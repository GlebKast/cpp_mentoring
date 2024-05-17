#include "multiprocessing.h"

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>

namespace
{
using namespace multiprocessing;

class MultiprocessingCopyToolTest : public ::testing::Test
{
private:
    std::string mqChunkReadyName = "mqChunkReadyshared_memory";
    std::string mqChunkSizeName = "mqChunkSizeshared_memory";

protected:
    void SetUp() override
    {
        std::ofstream sourceFile("source.txt");
        sourceFile << "This is a test file.";
        sourceFile.close();

        std::ofstream targetFile("target.txt");
        targetFile.close();
    }

    void TearDown() override
    {
        std::filesystem::remove("source.txt");
        std::filesystem::remove("target.txt");

        boost::interprocess::message_queue::remove(mqChunkSizeName.c_str());
        boost::interprocess::message_queue::remove(mqChunkReadyName.c_str());

        boost::interprocess::shared_memory_object::remove("shared_memory");
    }

    bool areFilesIdentical(const std::string &file1, const std::string &file2)
    {
        auto fileSize1 = std::filesystem::file_size(file1);
        auto fileSize2 = std::filesystem::file_size(file2);

        const auto MAX_SIZE = 1000000;
        if(fileSize1 > MAX_SIZE || fileSize2 > MAX_SIZE)
        {
            return false;
        }

        std::ifstream f1(file1, std::ios::binary);
        std::ifstream f2(file2, std::ios::binary);

        if(! f1.is_open() || ! f2.is_open())
        {
            return false;
        }

        std::vector<char> v1(std::istreambuf_iterator<char>(f1), {});
        std::vector<char> v2(std::istreambuf_iterator<char>(f2), {});

        return v1 == v2;
    }
};

TEST_F(MultiprocessingCopyToolTest, CopySuccessTest)
{
    system("./mentoringProj read source.txt target.txt shared_memory &");
    system("sleep 2 &");
    system("./mentoringProj write source.txt target.txt shared_memory");
    EXPECT_TRUE(areFilesIdentical("source.txt", "target.txt"));
}

TEST_F(MultiprocessingCopyToolTest, ReadTimeoutTest)
{
    auto start = std::chrono::high_resolution_clock::now();

    system("./mentoringProj read source.txt target.txt shared_memory");

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    EXPECT_LE(elapsed, TIMEOUT + std::chrono::seconds(1));

    EXPECT_FALSE(areFilesIdentical("source.txt", "target.txt"));
}

TEST_F(MultiprocessingCopyToolTest, WriteTimeoutTest)
{
    auto start = std::chrono::high_resolution_clock::now();

    system("./mentoringProj write source.txt target.txt shared_memory");

    auto end = std::chrono::high_resolution_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    EXPECT_LE(elapsed, TIMEOUT + std::chrono::seconds(1));

    EXPECT_FALSE(areFilesIdentical("source.txt", "target.txt"));
}

}    // namespace