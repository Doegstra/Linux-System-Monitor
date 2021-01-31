#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// constructor
Process::Process(int pid) : pid_(pid) {};

// Return this process's ID
int Process::Pid() const { 
    return pid_;
}

// Return this process's CPU utilization
// https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() { 

    // uptime of the system in seconds
    long int uptime = UpTime();

    // values from /proc/[PID]/stat
    vector<string> string_utils = LinuxParser::CpuUtilization(Pid());
    int utime = stoi(string_utils[13]); // CPU time spent in user code, measrued in clock ticks
    int stime = stoi(string_utils[14]); // CPU time sepnt in kernel code, measured in clock tickts
    int cutime = stoi(string_utils[15]); // Waited for children's CPU time spent in user code (in clock tickes)
    int cstime = stoi(string_utils[16]); // Waited for children's CPU time spent in kernel code (in clock tickes)
    int starttime = stoi(string_utils[21]); // time when the process started, measured in clock ticks

    // calculation
    int total_time = utime + stime; // total time spent in process
    total_time = total_time + cutime + cstime; // also include time spent from children processes
    float elapsed_time_in_secs = uptime - (float)starttime/sysconf(_SC_CLK_TCK);
    float totel_time_secs = (float)total_time/sysconf(_SC_CLK_TCK);
    float cpu_usage = totel_time_secs / elapsed_time_in_secs;

    return cpu_usage;
} 

// Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(Pid()); 
}

// Return this process's memory utilization
string Process::Ram() const { 
    return LinuxParser::Ram(Pid()); 
}

// Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(Pid()); 
}

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid());
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process &a) { 
    return Process::CpuUtilization() < a.Process::CpuUtilization();
}