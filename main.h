#ifndef MAIN_H
#define MAIN_H

// #define MIN(a, b) ((a) < (b) ? (a) : (b))

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_TASK 10
#define NUMBER_PROCESSORS  5

#define Generate_test_on 0
// #define Generate_and_run_test_on 0
#define Simulation_print 0
#define Self_Test 1

#define TOTAL_UTILIZATION 3.8
#define MIN_PERIOD 20
#define MAX_PERIOD 1000

typedef struct {
    int id;
    int priority;
    int priority_promotion;
    int period;       
    int execution_time;         
    int remaining_time;
    int deadline;
    int next_deadline;     
    int release_time; 
    int next_release_time;
    int priority_promotion_time;
    int next_priority_promotion_time;
} Task;

typedef struct {
    int processor_id;
    int runnin_task_id;
    bool Busy_State;
} Processor;

extern Processor processor[NUMBER_PROCESSORS];


extern Task Global_Tasks[NUMBER_TASK];



#endif
