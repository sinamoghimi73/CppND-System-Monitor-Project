#include "processor.h"

#include <chrono>
#include <thread>

#include "linux_parser.h"

// TODO : Return the aggregate CPU utilization
Processor::Processor() { Utilization(); }

float Processor::Utilization() {
  jiffies_old_ = LinuxParser::Jiffies();
  active_jiffy_old_ = LinuxParser::ActiveJiffies();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  jiffies_new_ = LinuxParser::Jiffies();
  active_jiffy_new_ = LinuxParser::ActiveJiffies();

  jiffies_delta = (jiffies_new_ - jiffies_old_) != 0;
  switch (jiffies_delta) {
  case 0:
    return 0.0;
    break;
  default:
    return (active_jiffy_new_ - active_jiffy_old_) /
           static_cast<float>(jiffies_new_ - jiffies_old_);
    break;
  }
}
