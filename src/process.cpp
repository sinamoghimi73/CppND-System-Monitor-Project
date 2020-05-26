#include "process.h"

#include <unistd.h>

#include <cctype>
#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
Process::Process(int pid)
    : pid_{pid}, user_{LinuxParser::User(pid)}, command_{LinuxParser::Command(
                                                    pid)} {}
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  return static_cast<float>(LinuxParser::ActiveJiffies(pid_) * 100) /
         sysconf(_SC_CLK_TCK) / LinuxParser::UpTime(pid_);
}

// DONE: Return the command that generated this process
string Process::Command() { return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return user_; }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const {
  return (std::stoi(LinuxParser::Ram(pid_)) >
          std::stoi(LinuxParser::Ram(a.pid_)))
             ? true
             : false;
}
