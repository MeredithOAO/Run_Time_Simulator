#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "main.h"
#include "Simulator_1.h"
#include "Task_generate.h"

void Set_Task_manually();
void Set_PPP();
int compare_task_priority_DP(const void* a, const void* b);
void check_PPP(int Current_time_DP);
void Reset_Gloabl_Task();
void check_realse_DP(int current_time);
void Run_Task_one_step_DP(Task* Global_Tasks, Task* Ready_Queue, int current_time, int Num_Task_need_run);
void G_DP_Scheduling(int Simulation_time);

int G_DP_Scheduling_Test(int Simulation_time);
void Reset_DP_Temp();
void try_all_ppp(int Simulation_time);