#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() {
  return pid_;
}

// Return this process's CPU utilization
// https://stackoverflow.com/a/16736599/5983691
float Process::CpuUtilization() {
  long total_time = LinuxParser::ActiveJiffies(pid_);
  long start_time = LinuxParser::UpTime(pid_);
  long up_time = LinuxParser::UpTime();
  long seconds = up_time - (start_time / sysconf(_SC_CLK_TCK));

  return (total_time / sysconf(_SC_CLK_TCK)) / seconds;
}

// Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(pid_);
}

// Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(pid_);
}

// Return the user (name) that generated this process
string Process::User() {
  return LinuxParser::User(pid_);
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime(pid_);
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(a.pid_));
}