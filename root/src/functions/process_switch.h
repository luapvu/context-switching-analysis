
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdint.h> //standard integer type library
#include <time.h> //time library for timespec, and timing functions







unsigned long long process_switch(){ //calculates the time difference between time B with time A

  unsigned long long time_diff = 0; //64 bit integer storing the time difference
  struct timespec start_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for starting timer
  struct timespec stop_timer; //structure rom time.h with a tv_sec and tv_nsec memeber for seconds and nanoseconds used for ending timer

  int childPipe[2]; //Pipe parent uses to contact child
  int parentPipe[2]; //Pipe child uses to contact parent
  pipe(childPipe); // creating child pipe fd
  pipe(parentPipe); // creating parent pipe fd

  int readBytes = 0; //holds the number of bytes read
  pid_t pid;// will hold the process id, = 0 for child

        if((pid = fork()) == -1){ //if pid is -1 then we know fork failed, exit program.
                perror("fork");
                exit(1);
        }

        if(pid == 0){ // this is the child process

              close(childPipe[0]); // child closes it's read end of it's pipe
              close(parentPipe[1]); // child closes parent's write end of pipe

              unsigned long long total = 0; //will contain the total sum of time of each read and write from the parent to child process summation over the loop
              int i = 0; //index for for loop
              for(i = 0; i < n; i++){
          			readBytes = read(parentPipe[0], &start_timer, sizeof(start_timer)); // blocked until something comes in the pipe. The value received here will be the start timer set by the parent using CLOCK_MONOTONIC
                clock_gettime(CLOCK_MONOTONIC, &stop_timer); //ends the timer by storing the updated value of CLOCK_MONOTONIC from the child's perspective
                time_diff=timespecDiff(&stop_timer,&start_timer); //calculates the time difference between start (sent by parent process) and stop (set as soon as the child read in the data sent by parent)
                total += time_diff; //want to sum all of these reads & writes to get a good approximate average
          			write(childPipe[1], &stop_timer, sizeof(stop_timer)); //now we write back to the parent process and wait go back to being blocked until parent writes back, or we've done this n times

              }

              total = total /n; //since total holds the value of n summations of read and writes, we must take the average and divide by n-iterations which read and write were done
              printf("PROCESS Context Switches take on avg: %llu ns\n",total); //prints out the average of a read and write context switch
              exit(0);
        }else{

          		close(parentPipe[0]); // parent closes it's own read pipe end
              close(childPipe[1]); // parent closes child's write pipe end

              int i = 0; //index for for loop
              for(i = 0; i < n; i++){
              clock_gettime(CLOCK_MONOTONIC, &start_timer);  //starts the timer using CLOCK_MONOTONIC as the reference point, getting and setting START to the CLOCK_MONOTONIC
          		write(parentPipe[1], &start_timer, sizeof(start_timer)); //writing the time reported by CLOCK_MONOTONIC for the start of the write process, therefore the read end of the pipe will know almost exactly when the parent wrote into the pipe
          		readBytes = read(childPipe[0], &stop_timer, sizeof(stop_timer)); //child writes back what time the child ended the timer, we don't really care since the calculations are done in the child process. Parent is blocked until something is written
                                                                  //once the child finally does write back to the parent, the parent will restart the timer and send it to the child again unless n-iterations are complete

              }

        wait(NULL); //wait until the cild complete's it's n'th iteration
       }
}
