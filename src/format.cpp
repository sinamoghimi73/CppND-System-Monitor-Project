#include "format.h"

#include <string>
#include <vector>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  std::vector<int> TimeFormat(4, 0);
  std::vector<int> TimeScale{86400, 3600, 60};

  for (int i{}; i < 3; i++) {
    TimeFormat[i] = seconds / TimeScale[i];
    seconds %= TimeScale[i];
  }
  TimeFormat[3] = seconds;

  bool flag = (TimeFormat[0] != 0) ? 0 : 1;
  switch (flag) {
  case 1:
    return std::to_string(TimeFormat[1]) + ":" + std::to_string(TimeFormat[2]) +
           ":" + std::to_string(TimeFormat[3]);
    break;
  default:
    return std::to_string(TimeFormat[0]) + ":" + std::to_string(TimeFormat[1]) +
           ":" + std::to_string(TimeFormat[2]) + ":" +
           std::to_string(TimeFormat[3]);
    break;
  }
}