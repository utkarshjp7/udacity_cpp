#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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
    std::string line, label;
    float mem_total, mem_free, cached, reclaimable, hem;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);

    while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        if (linestream >> label) {
            if (label == kMemTotal)
                linestream >> mem_total;
            else if (label == kMemFree)
                linestream >> mem_free;
            else if (label == kCached)
                linestream >> cached;
            else if (label == kSReclaimable)
                linestream >> reclaimable;
            else if (label == kShem)
                linestream >> hem;
        }
    }

    return (mem_total - mem_free - cached - reclaimable + hem) / mem_total; //htop green+blue
}

// Read and return the system uptime
long LinuxParser::UpTime() {
    long uptime;
    std::string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> uptime;
    }
    return uptime;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> tokens = CpuUtilization();
  long jiffies = 0;
  for (auto token : tokens) {
    jiffies += atol(token.c_str());
  }
  return jiffies;
}

// Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    return Jiffies() - IdleJiffies();
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<string> tokens = CpuUtilization();
    long idle_jiffies = atol(tokens[3].c_str()) + atol(tokens[4].c_str());
    return idle_jiffies;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    string line, label, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    std::ifstream stream(kProcDirectory + kStatFilename);
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> label >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    
    vector<string> result = {user, nice, system, idle, iowait, irq, softirq, steal};
    return result;
}

float LinuxParser::CpuUtilization(int pid) {
    string line;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    std::getline(stream, line);
    std::istringstream linestream(line);

    std::istream_iterator<std::string> begin(linestream), end;
    std::vector<std::string> values(begin, end);
    
    const float utime = stof(values[13]);
    const float stime = stof(values[14]);
    const float starttime = stof(values[21]);
    const float uptime = UpTime();
    const float hertz = sysconf(_SC_CLK_TCK);    
    const float total_time = utime + stime;
    const float seconds = uptime - (starttime / hertz);

    return ((total_time / hertz) / seconds);
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    std::string line, label;
    std::ifstream stream(kProcDirectory + kStatFilename);
    int processes;
    while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> label;
        if (label == kProcs) {
            linestream >> processes;
        }
    }
    return processes;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    std::string line, label;
    std::ifstream stream(kProcDirectory + kStatFilename);
    int running_processes;
    while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> label;
        if (label == kProcsRunning) {
            linestream >> running_processes;
        }
    }
    return running_processes;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
    std::string line;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kCmdlineFilename);
    std::getline(stream, line);
    return line;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
    std::string line, label, ram;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
    while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> label;
        if (label == "VmData:") {
            linestream >> ram;
        }
    }
    return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
    std::string line, label, uid;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
    while (std::getline(stream, line)) {
        std::stringstream linestream(line);
        linestream >> label;
        if (label == "Uid:") {
            linestream >> uid;
        }
    }
    return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
    std::string uid = Uid(pid);
    std::string line, username, x, y;
    std::ifstream passwordstream(LinuxParser::kPasswordPath);
    while (std::getline(passwordstream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> username >> x >> y;
        if (y == uid) {
            break;
        }
    }
    return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
    std::string line;
    std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
    std::getline(stream, line);
    std::istringstream linestream(line);

    std::istream_iterator<std::string> begin(linestream), end;
    std::vector<std::string> values(begin, end);

    long starttime = stol(values[21]);
    long hertz = sysconf(_SC_CLK_TCK);
    return (UpTime() - starttime) / hertz;
}