#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "main.h"
#include "Simulator_1.h"
 

double Utilization[NUMBER_TASK];
int Period[NUMBER_TASK];
//
 int compare_increasing(const void *a, const void *b);
 void Unifast_allocate(double U_total);
 void generate_task_set(Task *tasks);
 void Print_Task_Set();
 void generate_period();
 int check_U();
 

