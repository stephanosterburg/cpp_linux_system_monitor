#include <unistd.h>

#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
    long jiffies_total = LinuxParser::Jiffies();
    long jiffies_active = LinuxParser::ActiveJiffies();

    this->cpu_utilization_ = (float)jiffies_active / (float)jiffies_total;

    return cpu_utilization_;
}