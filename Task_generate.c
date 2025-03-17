#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "Task_generate.h"

int compare_increasing(const void *a, const void *b) {
    return (*(int *)a - *(int *)b); 
}

void Unifast_allocate(double U_total){
    
    for (int i = 0; i < NUMBER_TASK - 1; i++) {

        double rand_val = (double)rand() / RAND_MAX;
        Utilization[i] = U_total * (1 - (double)pow(rand_val, 1.0 / (NUMBER_TASK - i - 1)));
        U_total = U_total - Utilization[i];
    }

    Utilization[NUMBER_TASK - 1] = U_total;

}

int check_U(){
    for (int i = 0; i < NUMBER_TASK; i++)
    {
    if (Utilization[i] > 1.0 )//|| Utilization[i] < 0.1
    {
        return 1;
    }
    
    
    }
    return 0;
}

void generate_period(){

    // period // [min_period, max_period]
    for (int i = 0; i < NUMBER_TASK; i++) {
        
        Period[i] = (rand() % (MIN_PERIOD - MAX_PERIOD + 1)) + MIN_PERIOD;
    }

    qsort(Period, NUMBER_TASK, sizeof(int), compare_increasing);
    
}

void generate_task_set(Task *tasks)
{   
    int retry_times = 0;
    Unifast_allocate(TOTAL_UTILIZATION);

    while (check_U() && retry_times < 1000)
    {
        Unifast_allocate(TOTAL_UTILIZATION);
        retry_times++;
    }
    
    generate_period();

    for (int i = 0; i < NUMBER_TASK; i++)
    {
        int execution_time = (int)ceil(Utilization[i] * Period[i]);
        if (execution_time < 1) {
        execution_time = 1;  // c >= 1
        }

        // 
        tasks[i].id              = i;
        tasks[i].priority        = i + 1 + NUMBER_TASK; 
        tasks[i].period          = Period[i];
        tasks[i].execution_time  = execution_time;
        tasks[i].remaining_time  = 0;

        // deadline = period 
        tasks[i].deadline        = Period[i];
        tasks[i].next_deadline   = Period[i]; 
        tasks[i].release_time    = 0;
        tasks[i].next_release_time = 0;

        // pp = -1
        tasks[i].priority_promotion_time = -1;
        tasks[i].next_priority_promotion_time = -1;
        tasks[i].priority_promotion = i + 1 + NUMBER_TASK;

    }
    

    // for (int i = 0; i < NUMBER_TASK; i++)
    // {
    //     printf("%f %d \n",Utilization[i],Period[i]);
    // }



    
    
// TOTAL_UTILIZATION  MIN_PERIOD  MAX_PERIOD





}

void Print_Task_Set(){

float Real_utilization = 0;
for (int i = 0; i < NUMBER_TASK; i++)
{
    printf("Task %d State: priority:%d period:%d  execution_time:%d remaining_time:%d ", Global_Tasks[i].id, Global_Tasks[i].priority, Global_Tasks[i].period, Global_Tasks[i].execution_time, Global_Tasks[i].remaining_time);
    printf("deadline:%d next_deadline:%d release_time:%d next_release_time:%d \n", Global_Tasks[i].deadline, Global_Tasks[i].next_deadline, Global_Tasks[i].release_time, Global_Tasks[i].next_release_time);
    printf("Priority Promotion Time:%d next_Priority Promotion Time:%d Priotity_promotion:%d \n", Global_Tasks[i].priority_promotion_time, Global_Tasks[i].next_priority_promotion_time, Global_Tasks[i].priority_promotion);
    Real_utilization = Real_utilization + (float)Global_Tasks[i].execution_time / Global_Tasks[i].period;
}



printf("Set Utilization: %f Real Utilization: %f \n",TOTAL_UTILIZATION,Real_utilization);

int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);
printf("LCM = %d \n",Simulation_time);

}


void Print_Task_Set_general(Task* Task_need_to_print){

    float Real_utilization = 0;
    for (int i = 0; i < NUMBER_TASK; i++)
    {
        printf("Task %d State: priority:%d period:%d  execution_time:%d remaining_time:%d ", Task_need_to_print[i].id, Task_need_to_print[i].priority, Task_need_to_print[i].period, Task_need_to_print[i].execution_time, Task_need_to_print[i].remaining_time);
        printf("deadline:%d next_deadline:%d release_time:%d next_release_time:%d \n", Task_need_to_print[i].deadline, Task_need_to_print[i].next_deadline, Task_need_to_print[i].release_time, Task_need_to_print[i].next_release_time);
        printf("Priority Promotion Time:%d next_Priority Promotion Time:%d Priotity_promotion:%d \n", Task_need_to_print[i].priority_promotion_time, Task_need_to_print[i].next_priority_promotion_time, Task_need_to_print[i].priority_promotion);
        Real_utilization = Real_utilization + (float)Task_need_to_print[i].execution_time / Task_need_to_print[i].period;
    }
    
    
    
    printf("Set Utilization: %f Real Utilization: %f \n",TOTAL_UTILIZATION,Real_utilization);
    
    int Simulation_time = Calculate_LCM(Task_need_to_print,NUMBER_TASK);
    printf("LCM = %d \n",Simulation_time);
    
    }