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

int main() {

    srand(time(NULL)); // random seed

    int current_time = 0;   //   Time 
    int missDeadline_flag = 0;
    int DA_LC_flag = 1;
if (Self_Test)
{
    Print_Task_Set();
    print_result_of_DA();
    print_result_of_DA_LC();
}

    if (Generate_test_on)
    {
       
    //generate_task_set 
    generate_task_set(Global_Tasks);
    Print_Task_Set();
    DA_LC_flag = print_result_of_DA_LC();

    }
    
    if (DA_LC_flag)
    {
        printf("\nPass DA-LC Test. No need to run G-FP simulation \n");
    }else{
    
        printf("\nFail DA-LC Test. Need to run G-FP simulation \n");
    
    if (Simulation_print)
{
    int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);

    printf("\nSimulation Start, Simulation time(LCM) = %d M = %d \n",Simulation_time,NUMBER_PROCESSORS);



    for(current_time = 0; current_time < Simulation_time; current_time++) {
        
        // printf("Time %d: \n", current_time);

        Task Ready_Queue[NUMBER_TASK];
        if (check_deadline(current_time))
        {
            missDeadline_flag = 1;
        }
        
        
        check_realse(current_time);
        

        int Ready_Task_Count = Add_Task_to_RQ(Global_Tasks, Ready_Queue);

        qsort(Ready_Queue, Ready_Task_Count, sizeof(Task), compare_task_priority);

        // int idle_processor_count = check_processor_idle_count(processor);
    

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



    if (!missDeadline_flag)
    {
        printf("\nSimulation Result: All Task Meet Deadline \n\n");
    }else {

        printf("\nThese Tasks Miss Deadline. Need to try G-DP Scheduling. \n\n");
    }
    
    
}

}

return 0;
}

//./main.exe | tee output.txt