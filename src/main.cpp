#include "ncurses_display.h"
#include "system.h"
#include <iostream>

int main() {
 
 //std::string  s = LinuxParser::OperatingSystem();
//std::cout << s << "\n";

//int d;
//std::cin >> d;

//eturn 0;

  //std::cout << "In main \n";
  System system;
  /*auto processes = system.Processes();
  
  for (int i = 0 ; i != 10; i++)
  {
    float cpu = processes[i].CpuUtilization();
  }*/

  //return 0;
  //return 0;
  NCursesDisplay::Display(system);
  return 0;
}
