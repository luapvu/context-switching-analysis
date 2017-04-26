#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h> //time library for timespec, and timing functions

int light_Switch = 0; //light switch variable, simulate a thread turning on and off a light switch

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //initialize a mutex
pthread_cond_t conditionA = PTHREAD_COND_INITIALIZER; //initialize a condition for the mutex

unsigned long long time_diff = 0; //64 bit integer storing the time difference
struct timespec start_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for starting timer
struct timespec stop_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for ending timer

void* thread_func1(void *arg){ //tread 1 will run this function, mainly used to turn light switches on by setting it to 1

  int counter = 0; //counter from 0 to n
  while(1){ //will iterate until counter = n iterations

    if(light_Switch == 0){ //if the light switch is off, thread 1 will want to turn the switch on

      pthread_mutex_lock(&mutex); //locks the mutex to access the crictical code, light switch
      light_Switch = 1; //turning the light switch on
      counter++; //keep track of how many times the light switch was turned on

      clock_gettime(CLOCK_MONOTONIC, &start_timer);  //starts the timer using CLOCK_MONOTONIC as the reference point, getting and setting START to the CLOCK_MONOTONIC
      pthread_cond_wait(&conditionA, &mutex); //thread 1 will now release the mutex and wait for a signal sent to conditionA. When condition A is complete, we will exit out of the wait state
      pthread_mutex_unlock(&mutex); //coming back from the wait statement, thread 1 will own mutex still thus release the mutex again

    }
    if(counter >= n){ //break out of the while after n iterations
      break;
    }
  }

  pthread_exit(0); //thread ends
}

void* thread_func2(void *arg){ //function to be run by thread 2, will turn off the light switch

  int counter = 0; //counter from 0 to n
  unsigned long long total = 0; //will contain the total sum of time of all thread context switches
  unsigned long long time_diff = 0; //64 bit integer storing the time difference

  while(1){ //will iterate until counter reachs n
    if(light_Switch == 1){ //if the switch is on, thread 2 wants to turn the switch off
      pthread_mutex_lock(&mutex); //attempt to aquire the mutex, and lock it to thread 2
      clock_gettime(CLOCK_MONOTONIC, &stop_timer); //ends the timer by storing the updated value of CLOCK_MONOTONIC from the child's perspective
      time_diff= timespecDiff(&stop_timer,&start_timer); //calculates the time difference between start (set by thread 1) and stop
      total += time_diff; //want to sum all of these context switches to get a good approximate average

      light_Switch = 0; //turn switch off
      counter++;
      pthread_mutex_unlock(&mutex); //release the mutex, which will be briefely unlocked until conditon signal is sent to A
      pthread_cond_signal(&conditionA);//send signal to A so it can wake up and it will reconsume the mutex

    }
    if(counter >= n){//break out of the while after n iterations
      break;
    }
  }

  total = total /n; //since total holds the value of n summations of context switches, we take an average over n iterations
  printf("THREAD Context Switches take on avg: %llu ns\n",total); //prints out the average of a read and write context switch

  pthread_exit(0);
}


void thread_switch()
{

  pthread_t tid1, tid2; //thread IDs
  pthread_attr_t attr; //generic thread attributes
  pthread_attr_init(&attr); //initializing thread attributes

  pthread_create(&tid1, &attr, thread_func1, NULL); //creating thread 1 to run thread_func1
  pthread_create(&tid2, &attr, thread_func2, NULL); //creating thread 2 to run thread_func2

  //wait for both thread 1 and thread 2 to end
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

}
