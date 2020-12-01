#include <unistd.h>
#include <chrono>
#include <thread>
#include <vector>
#include <string>

#include "linux_parser.h"
#include "processor.h"

using std::vector;
using std::string;

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long total_jiffies_start = LinuxParser::Jiffies();
  long active_jiffies_start = LinuxParser::ActiveJiffies();

  std::this_thread::sleep_for(std::chrono::seconds(1));

  long total_jiffies_end = LinuxParser::Jiffies();
  long active_jiffies_end = LinuxParser::ActiveJiffies();

  float delta_total = total_jiffies_end - total_jiffies_start;
  float delta_active = active_jiffies_end - active_jiffies_start;
  float cpu_utilized = delta_active / delta_total;

  return cpu_utilized;
}