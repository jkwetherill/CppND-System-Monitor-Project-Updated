#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <regex>
#include <string>
#include <iostream>
#include <assert.h>
#include "linux_parser.h"
#include <sstream>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long int user_ = 0;
 long int nice_ = 0;
 long int system_ = 0;
 long int idle_ = 0;
 long int iowait_ = 0;
 long int iqr_ = 0;
 long int softiqr_ = 0;
 long int steal_ = 0;
 long int guest_ = 0;
 long int guest_nice_ = 0;
 float cpu_percentage_ = 0;


 long int NonIdle();
 long int Idle();
 long int Total();
void Reset();

};

#endif
