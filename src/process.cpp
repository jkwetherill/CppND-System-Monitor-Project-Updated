#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>
#include <fstream>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;


// TODO: Return this process's ID
int Process::Pid() const { return pid_; }



#define Hertz sysconf(_SC_CLK_TCK)

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) +
                           LinuxParser::kStatFilename);


  //https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599


  if(!filestream.is_open())
  return -1;

  std::string line;
  std::getline(filestream, line);

  std::regex reg_expr{R"(\d+ \S+ \w+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ (\d+) (\d+) (\d+) (\d+) [\d-]+ [\d-]+ [\d-]+ [\d-]+ (\d+))"};
  std::smatch matches;

  if (!std::regex_search(line, matches, reg_expr)) return -1;

  long int utime_jiffies      = LinuxParser::str2long(matches[1]); // CPU time in user code (jiffies)
  long int stime_jiffies      = LinuxParser::str2long(matches[2]); // CPU time in kernel code (jiffies)
  long int cutime_jiffies     = LinuxParser::str2long(matches[3]); //Waited-for children's CPU time spent in user code (in clock ticks)
  long int cstime_jiffies     = LinuxParser::str2long(matches[4]); //Waited-for children's CPU time spent in kernel code (in clock ticks)
  long int start_time_jiffies = LinuxParser::str2long(matches[5]); //Time when the process started, measured in clock ticks

  //std::string ParsedStartTime = matches[5];

  //std::cout << line << "\n";
  //std::cout << "Parsed String (Start Time (jiffies)): " << ParsedStartTime << "\n";


  /*
  Calculation
  First we determine the total time spent for the process:
  */

  long int total_time_jiffies = utime_jiffies + stime_jiffies;

  /*
  We also have to decide whether we want to include the time from children
  processes. If we do, then we add those values to total_time:
  */

/*
  total_time_jiffies =
      total_time_jiffies + cutime_jiffies + cstime_jiffies;
      */

  /*
  Next we get the total elapsed time in seconds since the process started:
  */

 // basic file operations



  double start_time_s = static_cast<double>(start_time_jiffies)/Hertz;

  double time_elapsed_s = LinuxParser::UpTime() - start_time_s;

  //std::cout << "HERTZ: " << Hertz << "\n";

  //std::cout << "start time (jiffies): " << start_time_jiffies << "\n";
  //std::cout << "start time (s): " << start_time_s << " \n";
  //std::cout << "Up time (s) " << LinuxParser::UpTime() << "\n";

  float seconds = static_cast<float> (time_elapsed_s);

  double total_time_s = static_cast<double>(total_time_jiffies/Hertz);

  //std::cout << "total time process time (jiffies): " << total_time_jiffies << "\n";
  //std::cout << "total process time (s) :" << total_time_s << "\n";

  double CPUFraction = total_time_s / time_elapsed_s;

  //std::cout << "CPU Fraction : " << CPUFraction << "\n";



  //std::cout << "Time Elapsed (s): " << seconds << "\n";

  //float seconds =
      //static_cast<float>(LinuxParser::UpTime() - start_time_jiffies / Hertz);

  /*
  Finally we calculate the CPU usage percentage: */
 

  //return total_time_jiffies / Hertz / seconds;

  return static_cast<float> (CPUFraction);
  

}

// TODO: Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }


/*
int Process::UID() const
{
// create a file stream
  std::ifstream filestream(LinuxParser::kOSPath + to_string(pid_) + "/status");

  assert(filestream.is_open());

  int UID = 0;
  std::string line, key;
  int value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "Uid") {
      return value;
    }
  }
  // throw an error if the code reaches this point
}
*/

// TODO: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(pid_); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& other_process) const 
{ 
  return CpuUtilization() > other_process.CpuUtilization();

}
