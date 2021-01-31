#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  // necessary private members
 private:
    static int previdle, previowait, prevuser, prevnice, prevsystem, previrq, prevsoftirq, prevsteal;
};

#endif