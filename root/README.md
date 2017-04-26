The following program is a study of the cost of a functions, system calls, Process context switch vs Threaded context switch.

The results were simulated on a machine with the below specifications:
Description:	Ubuntu 16.04.2 LTS
Release:	16.04
Codename:	xenial

In addition, all results were simulated on a SINGLE core system.

Step 1:
In the main.c file feel free to change the value of:
#define n 8000 // will take an average of n sample measurements for each function.
Such that n will be the average sample rate for all measurements. In the default case we have it set to 8000.


Step 2: 
Ensure you are in the src directory in command line and type "make" to execute the make file.

Step 3: 
After executing "make" an executable called "timer_code" should be created. To run the sample core simply type "make run"

Step 4:
Ensure to type "make clean" when you are done to clean all executable files. 

Step 5: 
Sample output is shown below for a single core system.




pvu@asb9812u-a01:~/Desktop/Ass2_complete/root/src$ make clean
rm -rf *o timer_code
pvu@asb9812u-a01:~/Desktop/Ass2_complete/root/src$ make
gcc -c main.c
gcc main.o -pthread -o timer_code
pvu@asb9812u-a01:~/Desktop/Ass2_complete/root/src$ taskset -c 2 ./timer_code 

The following measurements will be an average of 3500 measurements
------------------------------------------------------------------
Minimal function call cost is : 72 ns
Minimal system call cost is : 75 ns
PROCESS Context Switches take on avg: 1228 ns
THREAD Context Switches take on avg: 518 ns
------------------------------------------------------------------



