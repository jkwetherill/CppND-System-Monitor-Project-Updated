
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#include <iostream>

#define Hertz sysconf(_SC_CLK_TCK)

long int LinuxParser::str2long(std::string Str)
{
    // converts a string into an integer
    std::stringstream ss{Str};
    long int n;
    ss >> n;
    return n;
}

// DONE: An example of how to read data from the filesystem
std::string LinuxParser::OperatingSystem() {
  /* Information about the operating system exists in the /etc/os-release file*/

  // create a file stream
  std::ifstream filestream(kOSPath);

  // assert (filestream.is_open());
  if (!filestream.is_open()) return "";

  std::regex reg_expr{"(PRETTY_)*NAME=\"(.+)\""};

  std::smatch matches;

  std::string line;

  std::getline(filestream, line);

  //std::cout << line << "\n";

  if (!std::regex_search(line, matches, reg_expr))
    return "Unable to parse OS Path file.";
    //throw std::runtime_error("Unable to parse OS Path file.");

  return matches[2];
}
  /*

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
    */


// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  // create a file stream
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (!filestream.is_open()) return -1;

  std::string line, key;
  long int MemTotal{-1}, MemFree{-1}, MemAvailable{-1}, Buffers{-1};
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key;

    if (key == "MemTotal:")
      linestream >> MemTotal;
    else if (key == "MemFree:")
      linestream >> MemFree;
    else if (key == "MemAvailable:")
      linestream >> MemAvailable;
    else if (key == "Buffers:")
      linestream >> Buffers;
  }

  if ((MemTotal == -1) || (MemFree == -1) || (MemAvailable == -1) ||
      (Buffers == -1)) {
    return -1;
    //throw std::runtime_error("Unable to parse mem info file.");
  };

  /*
htop author here. These are the calculations I make to get the numbers for the
green, blue and yellow bars in the memory meter:

Total used memory = MemTotal - MemFree */

  long int total_used_memory = MemTotal - MemFree;

  /*
  Non cache/buffer memory (green) = Total used memory - (Buffers + Cached
  memory) Buffers (blue) = Buffers Cached memory (yellow) = Cached +
  SReclaimable - Shmem Swap = SwapTotal - SwapFree */

  return static_cast<float>(total_used_memory) / static_cast<float>(MemTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  // create a file stream
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (!filestream.is_open()) return -1;
  long int uptime_s;
  std::string line;
  std::getline(filestream, line);
  std::stringstream stream(line);
  stream >> uptime_s;
  return uptime_s;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<std::string> lines;

  // create a file stream
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (!filestream.is_open()) return lines;

  std::string line;
  while (std::getline(filestream, line)) {
    lines.push_back(line);
  }

  return lines;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  // Information about the total number of processes on the system exists in the
  // /proc/meminfo file.
  std::ifstream filestream(kProcDirectory + kStatFilename);
  // assert(filestream.is_open());
  if (!filestream.is_open()) return -1;

  std::string line, key;
  int value;

  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "processes") return value;
  }
  // throw an error here
  // throw std::runtime_error("Unable to parse total processes running.");
  return -1;  // this line should never execute
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  //assert(filestream.is_open());
  if (!filestream.is_open()) return -1;

  std::string line, key;
  int value;

  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "procs_running") return value;
  }
  // throw an error here
    //throw std::runtime_error("Unable to parse number of processes running.");
    return -1;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  // assert(filestream.is_open());
  if (!filestream.is_open()) return "";
  std::string line;
  std::getline(filestream, line);
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {   // create a file stream
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  //assert(filestream.is_open());
  if (!filestream.is_open()) return "cannot open file";
  //std::regex reg_expr{R"(VmSize:\s+(\d+\s\w+))"};
  std::regex reg_expr{R"(VmSize:\s+(\d+))"};
  std::string line;
  
  while (std::getline(filestream, line)) {
    std::smatch matches;
    if (std::regex_search(line, matches, reg_expr)) {
      long int ram_kb = str2long(matches[1]);
      double ram_mb = static_cast<double>(ram_kb)/1000;
      char out[256];
      sprintf(out, "%.2f", ram_mb);
      std::string out_string(out);
      return out_string;
    }
  }
  //throw std::runtime_error("Unable to parse memory used by process # " + pid);
  return ""; //failed to parse, but don't crash the monitor
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kOSPath + to_string(pid) + kStatusFilename);
  if(!filestream.is_open()) return "";
  std::string line, key, value;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "Uid") {
      return value;
    }
  }
  //throw std::runtime_error("Unable to parse user ID used by process # " + pid);
  return ""; // ideally this line is never executed
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {   // create a file stream
  std::ifstream filestream(kPasswordPath);

  if(!filestream.is_open()) return "";

  std::regex reg_expr{R"((\w+):x:)" + Uid(pid) + ".+"};

  std::string line;

  while (std::getline(filestream, line)) {
    std::smatch matches;
    if (std::regex_search(line, matches, reg_expr)) {
      assert(matches.size() == 2);
      return matches[1];
    }
  }
  //throw std::runtime_error("Unable to parse user associated with the process " + pid);
  return ""; //failed to parse, but don't crash the monitor
}
  // throw an error if the code reaches this point }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long int LinuxParser::UpTime(int pid) 
{ 
    //long int start_time_jiffies = 0;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if(!filestream.is_open()) return -1;
    /*for(int i = 0; i != 22; i++)
    {
        filestream >> start_time_jiffies;
    }*/

    std::string line;
    std::getline(filestream, line);

    /*
    std::regex reg_expr{
        R"(\d+ \S+ \w+ \d+ \d+ \d+ \d+ [\d-]+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ \d+ (\d+))"};
    std::smatch matches;*/


  std::regex reg_expr{R"(\d+ \S+ \w+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ [\d-]+ (\d+) (\d+) (\d+) (\d+) [\d-]+ [\d-]+ [\d-]+ [\d-]+ (\d+))"};
  std::smatch matches;


    if (!std::regex_search(line, matches, reg_expr)) return -1;
    
    long int start_time_jiffies = str2long(matches[5]);

    return UpTime() - start_time_jiffies/Hertz;
    //return start_time_jiffies/Hertz;



 


}



