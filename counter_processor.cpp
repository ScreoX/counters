#include "counter_processor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>

std::mutex cout_mutex;

void process_file(const std::string& filepath, std::map<std::string, int>& discontinuityCounts) {
    const int countOfChannels = 32;

    std::ifstream file(filepath, std::ios::binary);

    if (!file) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Error with file opening " << filepath << '\n';
        return;
    }

    std::vector<uint32_t> counters(countOfChannels);
    std::vector<bool> checkFirstValue(countOfChannels, true);
    int discontinuity_count = 0;

    uint32_t counter;
    int index = 0;

    while (file.read(reinterpret_cast<char*>(&counter), sizeof(counter))) {
        int channel = index % countOfChannels;

        if (checkFirstValue[channel]) {
            counters[channel] = counter;
            checkFirstValue[channel] = false;
        } else {
            uint32_t expected = counters[channel] + 1;

            if (counters[channel] == 0xFFFFFFFF) {
                expected = 0;
            }

            if (counter != expected) {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "File: " << filepath << ", Channel: " << channel
                          << ", Expected: " << expected
                          << ", Real value: " << counter << '\n';
                discontinuity_count++;
            }
            counters[channel] = counter;
        }
        index++;
    }

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        discontinuityCounts[filepath] = discontinuity_count;
    }
}

