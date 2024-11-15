#include <iostream>
#include <vector>
#include <thread>
#include <filesystem>
#include <map>
#include "counter_processor.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cout << "Invalid number of input files";
        return 1;
    }

    std::string directory_path = argv[1];

    std::vector<std::string> files;

    for (const auto& inputFile : std::filesystem::directory_iterator(directory_path)) {
        if (inputFile.is_regular_file()) {
            files.push_back(inputFile.path().string());
        }
    }

    if (files.size() != 8) {
        std::cout << "Invalid number of files.";
        return 1;
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

    for (const auto& [filename, count] : discontinuityCounts) {
        std::cout << "File: " << filename << ", Count of discontinuities: " << count << std::endl;
    }

    return 0;
}
