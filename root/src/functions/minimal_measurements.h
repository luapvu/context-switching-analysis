
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdint.h> //standard integer type library
#include <time.h> //time library for timespec, and timing functions



unsigned long long timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p) //calculates the time difference between time B with time A
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) - //takes in a timespec struct which has a nanosecon member
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}



void doNothing(){  //do nothing, accepts no args and returns nothing. The absolute minimal cost of a function

}

void minimumFunc(){
  unsigned long long time_diff = 0; //64 bit integer storing the time difference
  unsigned long long total = 0; //will contain the total sum of time

  struct timespec start_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for starting timer
  struct timespec stop_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for ending timer
  int i = 0;
  for(i = 0; i < n; i++){
    clock_gettime(CLOCK_MONOTONIC, &start_timer);  //starts the timer before calling the min cost function
    doNothing();
    clock_gettime(CLOCK_MONOTONIC, &stop_timer); //ends the timer as soon as the function completes
    time_diff= timespecDiff(&stop_timer,&start_timer); //calculates the time difference between start and stop
    total += time_diff; //want to sum all of these context switches to get a good approximate average

  }
  total = total /n; //since total holds the value of n summations  we must take the average and divide by n-iterations
  printf("Minimal function call cost is : %llu ns\n", total); //prints out the average of a read and write context switch


}

void minimumSysCall(){
  unsigned long long time_diff = 0; //64 bit integer storing the time difference
  unsigned long long total = 0; //will contain the total sum of time

  struct timespec start_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for starting timer
  struct timespec stop_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for ending timer

  int i = 0;
  for(i = 0; i < n; i++){
    clock_gettime(CLOCK_MONOTONIC, &start_timer);  //starts the timer before calling the min cost function
    getpid(); //system call which just gets process id.
    clock_gettime(CLOCK_MONOTONIC, &stop_timer); //ends the timer as soon as the function completes
    time_diff= timespecDiff(&stop_timer,&start_timer); //calculates the time difference between start and stop
    total += time_diff; //want to sum all of these context switches to get a good approximate average

  }
  total = total /n; //since total holds the value of n summations  we must take the average and divide by n-iterations
  printf("Minimal system call cost is : %llu ns\n", total);//prints out the average of a read and write context switch


}
