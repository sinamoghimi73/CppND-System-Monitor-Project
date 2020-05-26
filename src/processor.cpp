#include "processor.h"

#include <unistd.h>

#include <chrono>
#include <thread>

#include "linux_parser.h"

// DONE : Return the aggregate CPU utilization
Processor::Processor() {
  jiffies_old_ = 0;
  jiffies_new_ = 0;
  idle_jiffy_old_ = 0;
  idle_jiffy_new_ = 0;
  totald = 0;
  idled = 0;
}

float Processor::Utilization() {
  jiffies_old_ = LinuxParser::Jiffies();
  idle_jiffy_old_ = LinuxParser::IdleJiffies();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  jiffies_new_ = LinuxParser::Jiffies();
  idle_jiffy_new_ = LinuxParser::IdleJiffies();

  totald = jiffies_new_ - jiffies_old_;
  idled = idle_jiffy_new_ - idle_jiffy_old_;

  total_delta = (jiffies_new_ - jiffies_old_) != 0;
  switch (total_delta) {
  case 0:
    return 0.0;
    break;
  default:
    return static_cast<float>(totald - idled) / totald;
    break;
  }
}
