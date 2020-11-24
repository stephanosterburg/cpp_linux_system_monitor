#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    int Pid();                               
    std::string User();                      
    std::string Command();                   
    float CpuUtilization();                  
    std::string Ram();                       
    long int UpTime();                       
    bool operator<(Process const& a) const;  

  // Constructor
  Process(int pid) : pid_(pid) {}

  // Declare any necessary private members
  private:
    int pid_;
};

#endif