#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
//#include "system.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid):pid_(pid){};
  int Pid() const;                               // TODO: See src/process.cpp
  std::string User() const;                      // TODO: See src/process.cpp
  std::string Command() const;                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram() const;                       // TODO: See src/process.cpp
  long int UpTime() const;                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int pid_ = -1;
};

#endif
