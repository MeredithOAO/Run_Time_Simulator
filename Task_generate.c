#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "task_generate.h"




void generate_task_set(Task *tasks, int num_tasks, double total_util, int min_period, int max_period)
{
//
double *util = (double *)malloc(sizeof(double) * num_tasks);
double sumU = total_util;
int i;

// UUnifast allocate the Utilization
for (i = 0; i < num_tasks - 1; i++) {
double rand_val = (double)rand() / RAND_MAX;  // rand [0,1) 
// ui = sumU * (1 - x^(1/(n-i)))
double tmp = pow(rand_val, 1.0 / (num_tasks - i - 1));
util[i] = sumU * (1 - tmp);
sumU = sumU * tmp;
}
// last task
util[num_tasks - 1] = sumU;

// period and c
for (i = 0; i < num_tasks; i++) {
// [min_period, max_period]
int period = (rand() % (max_period - min_period + 1)) + min_period;

// c = ui * period
// 向上取整，以保证任务 C_i 不为 0
int execution_time = (int)ceil(util[i] * period);
if (execution_time < 1) {
execution_time = 1;  // c >= 1
}

// 
tasks[i].id              = i;
tasks[i].priority        = i + 1; 
tasks[i].period          = period;
tasks[i].execution_time  = execution_time;
tasks[i].remaining_time  = 0;

// deadline = period 
tasks[i].deadline        = period;
tasks[i].next_deadline   = period; 
tasks[i].release_time    = 0;
tasks[i].next_release_time = 0;
}

free(util);
}


void Print_Task_Set(){

float Real_utilization = 0;
for (int i = 0; i < NUMBER_TASK; i++)
{
    printf("Task %d State: priority:%d period:%d  execution_time:%d remaining_time:%d ", Global_Tasks[i].id, Global_Tasks[i].priority, Global_Tasks[i].period, Global_Tasks[i].execution_time, Global_Tasks[i].remaining_time);
    printf("deadline:%d next_deadline:%d release_time:%d next_release_time:%d \n", Global_Tasks[i].deadline, Global_Tasks[i].next_deadline, Global_Tasks[i].release_time, Global_Tasks[i].next_release_time);
    Real_utilization = Real_utilization + (float)Global_Tasks[i].execution_time / Global_Tasks[i].period;
}



printf("Set Utilization: %f Real Utilization: %f \n",TOTAL_UTILIZATION,Real_utilization);

int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);
printf("LCM = %d \n",Simulation_time);

}