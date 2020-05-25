#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
    long prev_total_;
    long prev_idle_;
    bool first_time_ = true;
};

#endif