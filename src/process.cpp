#include "process.h"

#include <cctype>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
Process::Process(int pid)
    : pid_{pid}, user_{LinuxParser::User(pid)}, command_{LinuxParser::Command(
                                                    pid)} {}
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long uptime_old_{}, uptime_new_{}, active_jiffy_old_{}, active_jiffy_new_{};
  uptime_old_ = LinuxParser::UpTime(pid_);
  active_jiffy_old_ = LinuxParser::ActiveJiffies(pid_);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  uptime_new_ = LinuxParser::UpTime(pid_);
  active_jiffy_new_ = LinuxParser::ActiveJiffies(pid_);

  float util{(active_jiffy_new_ - active_jiffy_old_) /
             static_cast<float>(uptime_new_ - uptime_old_)};
  return ((uptime_new_ - uptime_old_) != 0) ? util : 0.0;
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
  return (std::stoi(LinuxParser::Ram(pid_)) >
          std::stoi(LinuxParser::Ram(a.pid_)))
             ? true
             : false;
}