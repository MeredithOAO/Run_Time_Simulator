#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "main.h"
#include "Simulator_1.h" 
#include "Task_generate.h"
#include "DA_LC_Test.h"


// Processor processor[NUMBER_PROCESSORS];
Processor processor[NUMBER_PROCESSORS] = {
    {.processor_id = 0, .runnin_task_id = -1, .Busy_State = false},
    {.processor_id = 1, .runnin_task_id = -1, .Busy_State = false}
};

// Task Global_Tasks[NUMBER_TASK];
Task Global_Tasks[NUMBER_TASK] = {
    { .id = 0, .priority = 1, .period = 4,   .execution_time = 1,  .remaining_time = 0, .deadline = 3,  .next_deadline = 3,  .release_time = 0, .next_release_time = 0 },
    { .id = 1, .priority = 2, .period = 8,   .execution_time = 5,  .remaining_time = 0, .deadline = 6,  .next_deadline = 6,  .release_time = 0, .next_release_time = 0 },
    { .id = 2, .priority = 3, .period = 16,  .execution_time = 7,  .remaining_time = 0, .deadline = 9,  .next_deadline = 9,  .release_time = 0, .next_release_time = 0 },
    { .id = 3, .priority = 4, .period = 32,  .execution_time = 17, .remaining_time = 0, .deadline = 31, .next_deadline = 31, .release_time = 0, .next_release_time = 0 }
};

void main() {

    srand(time(NULL)); // random seed

    int current_time = 0;   //   Time 

if (Self_Test)
{
    Print_Task_Set();
    print_result_of_DA();
    printf("get_i_Diff: %d \n", get_i_Diff(3,2));
}

    if (Generate_test_on)
    {
       
    //generate_task_set 
    generate_task_set(Global_Tasks, NUMBER_TASK, TOTAL_UTILIZATION, MIN_PERIOD, MAX_PERIOD);
    Print_Task_Set();

    }
    

    
    if (Simulation_print)
{
    int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);

    printf("LCM = %d \n",Simulation_time);



    for(current_time = 0; current_time < Simulation_time; current_time++) {
        
        printf("Time %d: \n", current_time);

        Task Ready_Queue[NUMBER_TASK];
        check_deadline(current_time);
        check_realse(current_time);
        

        int Ready_Task_Count = Add_Task_to_RQ(Global_Tasks, Ready_Queue);

        qsort(Ready_Queue, Ready_Task_Count, sizeof(Task), compare_task_priority);

        int idle_processor_count = check_processor_idle_count(processor);
    

        if (Ready_Task_Count >= NUMBER_PROCESSORS)
        {
            Run_Task_one_step(Global_Tasks, Ready_Queue, current_time, NUMBER_PROCESSORS);
        }
        
        if (Ready_Task_Count < NUMBER_PROCESSORS)
        {
            Run_Task_one_step(Global_Tasks, Ready_Queue, current_time, Ready_Task_Count);
        }
        
        
 

        // for (int i = 0; i < NUMBER_TASK; i++)
        // {
        //     printf("Task %d State: priority:%d remaining_time:%d next_release_time:%d \n", Global_Tasks[i].id, Global_Tasks[i].priority, Global_Tasks[i].remaining_time, Global_Tasks[i].next_release_time);
        // }


        reset_tasks_queue(Ready_Queue,NUMBER_TASK);
    }
}
}

//./main.exe | tee output.txt