#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  Processor();
  float Utilization(); // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
private:
  long jiffies_old_{}, jiffies_new_{}, active_jiffy_old_{}, active_jiffy_new_{};
  bool jiffies_delta{};
};

#endif
