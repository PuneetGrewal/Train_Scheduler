INSTRUCTIONS:

1. Program can be compiled using "make" command which runs the included makefile
 and compiles "mts.c"
2. User then needs to type "mts.c input.txt" where "input.txt" can be any txt
file that follows the following criteria:

    ~Each line has a maximum of 3 elements.
    ~First index is one of the 4 from e, E, w, W which indicate and priority of
    the train_number
    ~Second index is the loading time of the train
    ~Third index is the crossing time of the train

3. My program will print the times according to the format and the times at
which the trains are ready. I was not able to complete the rest of the process.


MAIN FUNCTIONS USED AND EXPLANATIONS:

Major functions:

1. Function SETUP_TRAINS is used to initialize the program and add trains to
the linked list structure. Data is then used from this to print out the trains.
2. Function START_PROCESS is used to start the process and prints out which
trains are ready to, along with the time and direction
3. Main is used to invoke START_PROCESS and SETUP_TRAINS

Minor functions:

1. Function print_time is used to print the time according to the required
format and calls Timer_Stop which is used to stop the time.
2. Function line_number is used to find the number of trains that are included
in the input file
3. Functions priority and direction_of_train are used to add the conditions
of the trains to the data structures


REFERENCES AND SOURCES:


Partial Linked List Structure used from following sources:

https://www.geeksforgeeks.org/data-structures/linked-list/
https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
https://www.learn-c.org/en/Linked_lists
