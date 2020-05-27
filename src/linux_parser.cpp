#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    string os, version, kernel;
    string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    return kernel;
  }
  return string();
}

// DONE: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  std::filesystem::path path(kProcDirectory);
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::vector<int> pids{};
    std::string pid{};
    for (auto const &files : std::filesystem::directory_iterator(path)) {
      pid = files.path().stem().string();
      if (isdigit(pid[0]))
        pids.push_back(stoi(pid));
    }
    return pids;
  }
  return {};
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream mem_stream(kProcDirectory + kMeminfoFilename);
  if (mem_stream.is_open()) {
    std::string line{}, key{};
    int used_mem{}, free_mem{}, total_mem{}, value{};
    for (int i{0}; i < 2; i++) {
      std::getline(mem_stream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:")
        total_mem = value;
      else if (key == "MemFree:")
        free_mem = value;
    }
    used_mem = total_mem - free_mem;
    return static_cast<float>(used_mem) / (total_mem);
  }
  return 0.0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream UpTimeStream(kProcDirectory + kUptimeFilename);
  if (UpTimeStream.is_open()) {
    long uptime{};
    string line{};
    std::getline(UpTimeStream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return uptime;
  }
  return 0;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::vector<std::string> cpu_util = CpuUtilization();
  long sum{};
  for (int token{kUser_}; token <= kSteal_; token++)
    sum += std::stol(cpu_util[token]);
  return sum;
}

// DONE: Read and return the number of active jiffies for a PID
std::vector<std::string> LinuxParser::PidStat(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::string line{}, jiffy{};
    std::vector<std::string> contents{};
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (std::getline(linestream, jiffy, ' '))
      contents.emplace_back(jiffy);
    return contents;
  }
  return {};
}

long LinuxParser::ActiveJiffies(int pid) {
  std::vector<std::string> contents = PidStat(pid);
  if (contents.size() != 0) {
    long sum{};
    for (int i{13}; i < 17; i++)
      sum += stol(contents[i]);
    return sum;
  }
  return 0;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return Jiffies() - IdleJiffies(); }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpu_util = CpuUtilization();
  long sum{};
  for (int token{kIdle_}; token <= kIOwait_; token++)
    sum += std::stol(cpu_util[token]);
  return sum;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    string line{}, key{};
    int token_count{10};
    std::vector<std::string> cpu_util(token_count);

    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    for (int token{kUser_}; token <= kGuestNice_; token++)
      linestream >> cpu_util[token];
    return cpu_util;
  }
  return {};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::string line{}, key{};
    int value{}, line_num{6};
    for (int i{}; i <= line_num; i++)
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    return value;
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::string line{}, key{};
    int value{}, line_num{7};
    for (int i{}; i <= line_num; i++)
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    return value;
  }
  return 0;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::string line{};
    int limit{22};
    std::getline(stream, line);
    if (line.empty())
      return "None";
    else if (static_cast<int>(line.length()) <= limit)
      return line;
    else {
      line.resize(limit);
      line.shrink_to_fit();
      return line + "..";
    }
  }
  return string();
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::string line{}, key{};
    std::ostringstream num{};
    float value{};
    int line_num{17};
    for (int i{}; i <= line_num; i++)
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    num << std::fixed << std::setprecision(2) << value / 1000.0;
    if (key == "VmSize:")
      return num.str();
  }
  return "0";
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::string line{}, key{}, value{};
    int line_num{8};
    for (int i{}; i <= line_num; i++)
      std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key >> value;
    if (key == "Uid:")
      return value;
  }
  return string();
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    std::string line{}, key{}, x{}, value{}, uid{Uid(pid)};
    std::istringstream linestream{};
    int limit{8};
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      linestream.str(line);
      linestream >> key >> x >> value;

      if (value == uid) {
        if (static_cast<int>(key.length()) <= limit)
          return key;
        else {
          key.resize(limit);
          key.shrink_to_fit();
          return key + "..";
        }
      }
    }
  }
  return string();
}
// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::vector<std::string> contents = PidStat(pid);
  if (contents.size() != 0)
    return (UpTime() - stol(contents[21]) / sysconf(_SC_CLK_TCK));
  return 0;
}
