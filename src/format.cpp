#include <string>
#include <sstream>

#include "format.h"

using std::string;

// helper function that converts long to HH:MM:SS
// https://www.programmingnotes.org/2062/c-convert-time-from-seconds-into-hours-min-sec-format/
string Format::ElapsedTime(long time_in_seconds) { 
    long hours = time_in_seconds /3600;
    time_in_seconds = time_in_seconds % 3600;
    long minutes = time_in_seconds / 60;
    long seconds = time_in_seconds % 60;
    std::stringstream time_ss;
    time_ss << hours << ":" << minutes << ":" << seconds;
    return time_ss.str(); 
}