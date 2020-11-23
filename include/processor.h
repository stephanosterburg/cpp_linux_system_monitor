#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

  // Declare any necessary private members
 private:
  long jiffies_start_sum = 0;
  long jiffies_start_act = 0;
  long jiffies_end_sum = 0;
  long jiffies_end_act = 0;
};

#endif