#include <string>

#include "format.h"

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    std::string hours = std::to_string(seconds / 3600);
    std::string minutes = std::to_string((seconds % 3600) / 60);
    std::string seconds_ = std::to_string(seconds % 60);
    return hours + ":" + minutes + ":" + seconds_;
}