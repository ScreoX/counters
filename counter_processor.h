#ifndef COUNTERS_COUNTER_PROCESSOR_H
#define COUNTERS_COUNTER_PROCESSOR_H
#include <string>
#include <map>

void process_file(const std::string& filepath, std::map<std::string, int>& discontinuityCounts);

#endif
