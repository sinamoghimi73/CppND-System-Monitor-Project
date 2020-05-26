#include "format.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  std::vector<std::string> TimeFormat(4);
  std::vector<long> TimeScale{86400, 3600, 60};

  std::ostringstream num{};

  num << seconds / TimeScale[0];
  TimeFormat[0] = num.str();
  seconds %= TimeScale[0];

  for (int i{1}; i < 3; i++) {
    num = {};
    num << std::dec << std::setw(2) << std::setfill('0')
        << seconds / TimeScale[i];
    TimeFormat[i] = num.str();
    seconds %= TimeScale[i];
  }

  num = {};
  num << std::dec << std::setw(2) << std::setfill('0') << seconds;
  TimeFormat[3] = num.str();

  bool flag = (std::stol(TimeFormat[0]) != 0) ? 0 : 1;
  switch (flag) {
    case 1:
      return TimeFormat[1] + ":" + TimeFormat[2] + ":" + TimeFormat[3];
      break;
    default:
      return TimeFormat[0] + ":" + TimeFormat[1] + ":" + TimeFormat[2] + ":" +
             TimeFormat[3];
      break;
  }
}
