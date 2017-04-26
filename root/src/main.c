
#define n 8000 // will take an average of n sample measurements for each function.

#include <stdlib.h>
#include <stdio.h>
#include "functions/minimal_measurements.h"
#include "functions/process_switch.h"
#include "functions/thread_switch.h"

int main(void)
{
  printf("\n");
  printf("The following measurements will be an average of %d measurements\n", n);
  printf("------------------------------------------------------------------\n");
  minimumFunc(); //measures the cost of a bare bones function
  minimumSysCall(); //measures the cost of a typical system call
  process_switch(); //measures the cost of a process context switch
  thread_switch(); //measures the cost of a thread context switches
  printf("------------------------------------------------------------------\n");
  printf("\n");

  return(0);
}
