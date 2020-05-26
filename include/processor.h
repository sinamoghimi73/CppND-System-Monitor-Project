#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long jiffies_old_{}, jiffies_new_{}, idle_jiffy_old_{}, idle_jiffy_new_{},
      totald{}, idled{};
  bool total_delta{};
};

#endif
