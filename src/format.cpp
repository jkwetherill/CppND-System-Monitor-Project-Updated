#include <string>

#include "format.h"
#include "stdio.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

#define HOUR_SEC 3600
#define MIN_SEC 60
std::string Format::ElapsedTime(long seconds) 
{
  int hh = static_cast<int>(seconds / HOUR_SEC);
  seconds -= hh * HOUR_SEC;
  int mm = static_cast<int>(seconds / MIN_SEC);
  seconds -= mm * MIN_SEC;
  char out[256];
  sprintf(out, "%02d:%02d:%02d", hh, mm, static_cast<int>(seconds));
  std::string out_string(out);
  return out_string;

  // return std::to_string(hh) + ":" + std::to_string(mm) + ":"  +
  // std::to_string(seconds);
}
