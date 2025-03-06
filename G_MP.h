#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "main.h"
#include "Simulator_1.h"

void Set_Task_manually();
void Set_PPP();
void check_PPP(int Current_time_DP);
void check_realse_DP(int current_time);
void G_DP_Scheduling(int Simulation_time);
void Run_Task_one_step_DP(Task* Global_Tasks, Task* Ready_Queue, int current_time, int Num_Task_need_run);
void Reset_Gloabl_Task();