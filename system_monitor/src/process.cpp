#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() const { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const { return LinuxParser::CpuUtilization(pid_); };

// Return the command that generated this process
string Process::Command() { return command_; };

// Return this process's memory utilization
string Process::Ram() {
    std::string str_ram = LinuxParser::Ram(Pid());
    if (str_ram == "") 
        return "0.0";

    float ram = stof(str_ram) / 1000.0;
    std::stringstream stream("");
    stream << std::fixed << std::setprecision(2) << ram;
    return stream.str();
}

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(); };

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    return this->CpuUtilization() < a.CpuUtilization();
}