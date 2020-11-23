#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  jiffies_start_sum = LinuxParser::Jiffies();
  jiffies_start_act = LinuxParser::ActiveJiffies();

  // Sleep 100 milliseconds
  usleep(100000);

  jiffies_end_sum = LinuxParser::Jiffies();
  jiffies_end_act = LinuxParser::ActiveJiffies();

  double delta_sum = jiffies_end_sum - jiffies_start_sum;
  double delta_act = jiffies_end_act - jiffies_start_act;
  if (delta_sum == 0) return 0.0;

  return delta_act - delta_sum;
}