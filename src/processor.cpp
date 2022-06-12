#include "processor.h"



int str2int(std::string Str)
{
    // converts a string into an integer
    std::stringstream ss{Str};
    int n;
    ss >> n;
    return n;
}


void Processor::Reset()
{

    //reset all internal states to 0
 user_ = 0;
 nice_ = 0;
 system_ = 0;
 idle_ = 0;
 iowait_ = 0;
 iqr_ = 0;
 softiqr_ = 0;
 steal_ = 0;
 guest_ = 0;
 guest_nice_ = 0;
 cpu_percentage_ = 0;
}


long int Processor::NonIdle()
{
    return user_ + nice_ + system_ + iqr_ + softiqr_ + steal_;
}

long int Processor::Idle()
{
    return idle_ + iowait_;
}

long int Processor::Total()
{
    return Idle() + NonIdle();
}



// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 

  /*
    //create a file stream
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

    if (!filestream.is_open())
        return cpu_percentage_;*/

    
    std::vector<std::string> lines = LinuxParser::CpuUtilization();

    // store previous state
    float PrevTotal = Total();
    float PrevIdle = Idle();

    // reset states to 0 
    Reset();


    std::regex reg_expr{R"(cpu\d\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+)\s(\d+))"};

    std::smatch matches;

    for (std::string line : lines) {
      if (!std::regex_search(line, matches, reg_expr)) continue;

      // assert(matches.size() == 11);
      if (matches.size() != 11) continue;

      // update states
      user_ += str2int(matches[1]);
      nice_ += str2int(matches[2]);
      system_ += str2int(matches[3]);
      idle_ += str2int(matches[4]);
      iowait_ += str2int(matches[5]);
      iqr_ += str2int(matches[6]);
      softiqr_ += str2int(matches[7]);
      steal_ += str2int(matches[8]);
      guest_ += str2int(matches[9]);
      guest_nice_ += str2int(matches[10]);

      float totald = Total() - PrevTotal;
      float idled = Idle() - PrevIdle;
      cpu_percentage_ = (totald - idled)/ totald;

      PrevTotal = Total();
      PrevIdle = Idle();
    }

    return cpu_percentage_; 
}


/*
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


 float user_ = 0.0;
 float nice_ = 0.0;
 float system_ = 0.0;
 float idle_ = 0.0;
 float iowait_ = 0.0;
 float iqr_ = 0.0;
 float softiqr_ = 0.0;
 float steal_ = 0.0;
 float guest_ = 0.0;
 float guest_nice_ = 0.0;
  */
