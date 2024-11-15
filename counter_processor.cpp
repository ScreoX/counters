#include "counter_processor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>

struct CounterInfo {
    bool isFirstValue;
    uint32_t data;
};

std::mutex cout_mutex;

void process_file(const std::string& filepath, std::map<std::string, int>& discontinuityCounts) {
    const int countOfChannels = 32;

    std::ifstream file(filepath, std::ios::binary);

    if (!file) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Error with file opening " << filepath << '\n';
        return;
    }

    std::vector<CounterInfo> counters(countOfChannels);

    for (auto& counter : counters) {
        counter.isFirstValue = true;
    }

    int discontinuity_count = 0;

    uint32_t counter;
    int index = 0;

    while (file.read(reinterpret_cast<char*>(&counter), sizeof(counter))) {
        const int channel = index % countOfChannels;

        if (counters[channel].isFirstValue) {
            counters[channel].data = counter;
            counters[channel].isFirstValue = false;
        } else {
            uint32_t expected = counters[channel].data + 1;

            if (counter != expected) {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "File: " << filepath << ", Channel: " << channel
                          << ", Expected: " << expected
                          << ", Real value: " << counter << '\n';
                discontinuity_count++;
            }
            counters[channel].data = counter;
        }
        index++;
    }

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        discontinuityCounts[filepath] = discontinuity_count;
    }
}

