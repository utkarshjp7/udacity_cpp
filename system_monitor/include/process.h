#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid): pid_(pid) {
      user_ = LinuxParser::User(Pid());
      command_ = LinuxParser::Command(Pid());
  };
  int Pid() const;                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  float CpuUtilization() const;                  // See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp

  // Declare any necessary private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
};

#endif