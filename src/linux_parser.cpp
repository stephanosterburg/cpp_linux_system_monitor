#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  string line;
  string key;
  string value;
  float total_mem = 0.0;
  float free_mem = 0.0;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") total_mem = stof(value);
      if (key == "MemFree:") free_mem = stof(value);
    }
    return (total_mem - free_mem) / total_mem;
  }

  return 0.0;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptime;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;

    return stol(uptime);
  }

  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  long result = 0;

  vector<string> utilized = LinuxParser::CpuUtilization();
  if (!utilized.empty()) {
    for (int i = kUser_; i <= kSteal_; i++) {
      if (utilized[i] += "") result += stol(utilized[i]);
    }
  }

  return result;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string value;
  long result = 0;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);

    // Get all time values in the pid stats file (14-17) 
    // see https://man7.org/linux/man-pages/man5/proc.5.html
    int i = 0;
    while (linestream >> value) {
      if (i >= 13 && i <= 16) result += stol(value);
      i++;
    }
  }

  return result;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::ActiveJiffies() - LinuxParser::IdleJiffies();
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long result = 0;
  vector<string> utilized = LinuxParser::CpuUtilization();

  if (!utilized.empty()) {
    for (int i = kIdle_; i <= kIOwait_; i++)  {
      if (utilized[i] += "") result += stol(utilized[i]);
    }
  }

  return result;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  string values;
  vector<string> cpu_utilization;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> values) {
      // https://en.cppreference.com/w/cpp/container/vector/push_back
      if (values != "cpu") cpu_utilization.push_back(std::move(values));
    }
  }

  return cpu_utilization;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;

  std::ifstream stream(kPasswordPath + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> key >> line){
      if (key == "processes") return stoi(value);
    }
  }

  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;

  std::ifstream stream(kPasswordPath + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);

    while (linestream >> key >> line){
      if (key == "procs_runnings") return stoi(value);
    }
  }

  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string value;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, value);
    return value;
  }

  return string();
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::stringstream memory;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "VmSize") {
        memory << std::setprecision(2) << stof(value) / 1000;
        return memory.str();
      }
    }
  }

  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> value){
      if (key == "Uid:") return value;
    }
  }

  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string username;
  string uid = LinuxParser::Uid(pid);

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> value >> key) {
        if (key == uid) return username;
      }
    }
  }

  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  vector<string> values;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) values.push_back(line);

    return stol(values[21]) / sysconf(_SC_CLK_TCK);
  }

  return 0;
}