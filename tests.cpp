#include <gtest/gtest.h>
#include <vector>
#include <thread>
#include <filesystem>
#include <map>
#include "counter_processor.h"

void runTest(const std::string& directoryPath, std::vector<int>& expectedDiscontinuities) {

    std::vector<std::string> files;

    for (const auto& inputFile : std::filesystem::directory_iterator(directoryPath)) {
        if (inputFile.is_regular_file()) {
            files.push_back(inputFile.path().string());
        }
    }

    std::map<std::string, int> discontinuityCounts;

    std::vector<std::thread> threads;
    for (const std::string& filepath : files) {
        threads.emplace_back(process_file, filepath, std::ref(discontinuityCounts));
    }

    for (std::thread& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    std::vector<int> result;

    for (auto i : discontinuityCounts) {
        result.emplace_back(i.second);
    }

    ASSERT_EQ(result, expectedDiscontinuities);

}

TEST(CounterProcessorTests, TestScenario1) {
    std::string directoryPath = "D:/InternShip/Tasks/counter/cmake-build-debug/data_ok";
    std::vector<int> expectedDiscontinuityCounts(8, 0);
    runTest(directoryPath, expectedDiscontinuityCounts);
}

TEST(CounterProcessorTests, TestScenario2) {
    std::string testFilePath = "D:/InternShip/Tasks/counter/cmake-build-debug/data_fail";
    std::vector<int> expectedDiscontinuityCounts(8, 128);
    runTest(testFilePath, expectedDiscontinuityCounts);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

