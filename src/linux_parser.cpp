#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read name of operating system (pretty name)
// file consists of key-value pairs 
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath); // create file stream
  if (filestream.is_open()) { // check if we can open file stream
    while (std::getline(filestream, line)) { // as long as we can read another line from the file stream, keep going
      // we split each line into a key-value pair (two tokens)
      // character replacement, since keys have spaces etc. 
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line); // create line stream
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' '); // revert replacements made earlier
          return value;
        }
      }
    }
  }
  return value;
}

// Get OS kernel by parsing one line file 
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename); // input file stream from the path for the OS kernel
  if (stream.is_open()) { // check if we are able to open the stream
    std::getline(stream, line); // get a line from the stream and store it in our line string 
    std::istringstream linestream(line); // create string stream from string (allows to pull tokens off of the string)
    linestream >> os >> version >> kernel; // kernel is thrid token in file
  }
  return kernel;
}

// Get process ids
// essentially every directory name in /proc/ that is integer
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  // read MemTotal and MemFree
  string line, name, memTotalString, memFreeString;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename); 
  if (filestream.is_open()) { 

    // first line is MemTotal
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> name >> memTotalString;

    // second line is MemFree
    std::getline(filestream, line);
    std::istringstream linestream2(line);
    linestream2 >> name >> memFreeString;
  }

  // calculation of utilization
  // https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
  return (std::stof(memTotalString) - std::stof(memFreeString))/std::stof(memTotalString); 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return stol(uptime);  // string to long
  }

  return 0;
  
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> util;
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open()) { 
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string s;
    linestream >> s; // first value is "cpu" not a number
    while(linestream >> s) {
      util.emplace_back(s);
    }
  }
  return util; 
}

// Read and return CPU utilization of a process
vector<string> LinuxParser::CpuUtilization(int pid) { 
  vector<string> util;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename); 
  if (filestream.is_open()) { 
    std::getline(filestream, line);
    std::istringstream linestream(line);
    string s;
    while(linestream >> s) {
      util.emplace_back(s);
    }
  }
  return util; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open()) { 
    while (std::getline(filestream, line)) { 
      std::istringstream linestream(line); // create line stream
      while (linestream >> key >> value) {
        if (key == "processes") {
          return std::stoi(value);
        }
      }
    }
  }

  // -1 to show error
  return -1; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {  
  string line, key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open()) { 
    while (std::getline(filestream, line)) { 
      std::istringstream linestream(line); // create line stream
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return std::stoi(value);
        }
      }
    }
  }

  // -1 to show error
  return -1; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename); 
  if (filestream.is_open()) { 
    std::getline(filestream, value);
  }
  return value; 

}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
    string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename); 
  if (filestream.is_open()) { 
    while (std::getline(filestream, line)) { 
      std::istringstream linestream(line); // create line stream
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return std::to_string((int)(stoi(value)*0.001)); // kB to MB
        }
      }
    }
  } 
  return "-99"; // error 
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename); 
  if (filestream.is_open()) { 
    while (std::getline(filestream, line)) { 
      std::istringstream linestream(line); // create line stream
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  } 
  return "-99"; // error
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, user, x, uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) { 
    while (std::getline(filestream, line)) { 
      std::replace(line.begin(), line.end(), ':', ' '); // replace such that >> works
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return user;
        }
      }
    }
  }
  return "error"; 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) { 
    std::getline(filestream, line);
    std::istringstream linestream(line);
    int count = 0;
    while (linestream >> value) {
      if (++count == 22) {
        return LinuxParser::UpTime() -  stol(value) / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return 0; 
}