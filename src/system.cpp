#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System()
    : kernel_{LinuxParser::Kernel()}, os_{LinuxParser::OperatingSystem()} {}
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
// bool Compare(Process a, Process b) {
//   return a.CpuUtilization() < b.CpuUtilization();
// }

vector<Process>& System::Processes() {
  std::vector<int> pids{LinuxParser::Pids()};
  for (int const& pid : pids) processes_.emplace_back(Process(pid));
  // std::sort(processes_.begin(), processes_.end(), Compare);
  return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }