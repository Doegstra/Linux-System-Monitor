
#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"
using std::string;
using std::vector;

int Processor::previdle{0};
int Processor::previowait{0};
int Processor::prevuser{0};
int Processor::prevnice{0};
int Processor::prevsystem{0};
int Processor::previrq{0};
int Processor::prevsoftirq{0};
int Processor::prevsteal{0};


// Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> string_utils = LinuxParser::CpuUtilization();
    vector<int> int_utils;
    for (auto& s_util: string_utils) {
        int_utils.emplace_back(stoi(s_util));
    }
    // guide how to use the numbers in int_utils
    // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

    int user{int_utils[0]};    // 43240
    int nice{int_utils[1]};    // 8595
    int system{int_utils[2]};  // 16028
    int idle{int_utils[3]};    // 164053
    int iowait{int_utils[4]};  // 2184
    int irq{int_utils[5]};     // 0
    int softirq{int_utils[6]}; // 385
    int steal{int_utils[7]};   // 0

    int PrevIdle = previdle + previowait;  // 0 + 0 = 0
    int Idle = idle + iowait;  // 164053 + 2184 = 166237

    int PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal; // 0 + 0 + 0 + 0 + 0 + 0 = 0
    int NonIdle = user + nice + system + irq + softirq + steal;  // 43240 + 8595 + 16028 + 0 + 385 = 68248

    int PrevTotal = PrevIdle + PrevNonIdle;  // 0 + 0 = 0
    int Total = Idle + NonIdle;  // 166237 + 68248 = 234485

    // differentiate: actual value minus the previous one
    int totald = Total - PrevTotal;  // 234485 + 0 = 234485
    int idled = Idle - PrevIdle;  // 166237 + 0 = 166237

    float CPU_Percentage = (float)(totald - idled)/(float)totald;  // (234485 - 166237)/234485 = 0.291

    previdle = idle;
    previowait = iowait;
    prevuser = user;
    prevnice = nice;
    prevsystem = system;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;

    return CPU_Percentage; 
    
}