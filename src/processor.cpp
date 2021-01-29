
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

    int user{int_utils[0]};
    int nice{int_utils[1]};
    int system{int_utils[2]};
    int idle{int_utils[3]};
    int iowait{int_utils[4]};
    int irq{int_utils[5]};
    int softirq{int_utils[6]};
    int steal{int_utils[7]};

    int PrevIdle = previdle + previowait;
    int Idle = idle + iowait;

    int PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    int NonIdle = user + nice + system + irq + softirq + steal;

    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    // differentiate: actual value minus the previous one
    int totald = Total - PrevTotal;
    int idled = Idle - PrevIdle;

    float CPU_Percentage = (totald - idled)/totald;

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