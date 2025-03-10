#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "G_MP.h"


void Set_Task_manually(){

    Global_Tasks[0] = (Task){ .id = 0, .priority = 1, .period = 4,  .priority_promotion = 1, .execution_time = 1,  .remaining_time = 0, .deadline = 3,  .next_deadline = 3,  .release_time = 0, .next_release_time = 0, .priority_promotion_time = 0, .next_priority_promotion_time = 0 };
    
    Global_Tasks[1] = (Task){ .id = 1, .priority = 2, .period = 8,  .priority_promotion = 2, .execution_time = 5,  .remaining_time = 0, .deadline = 6,  .next_deadline = 6,  .release_time = 0, .next_release_time = 0, .priority_promotion_time = 3, .next_priority_promotion_time = 3 };
    
    Global_Tasks[2] = (Task){ .id = 2, .priority = 3, .period = 16, .priority_promotion = 3, .execution_time = 7,  .remaining_time = 0, .deadline = 9,  .next_deadline = 9,  .release_time = 0, .next_release_time = 0, .priority_promotion_time = 6, .next_priority_promotion_time = 6 };
    
    Global_Tasks[3] = (Task){ .id = 3, .priority = 4, .period = 32, .priority_promotion = 4, .execution_time = 17, .remaining_time = 0, .deadline = 31, .next_deadline = 31, .release_time = 0, .next_release_time = 0, .priority_promotion_time = 27, .next_priority_promotion_time = 27 };
    
}


void Set_PPP(){

for (int i = 0; i < NUMBER_TASK; i++)
{
    int ALL_C_j = 0;
    for (int j = 0; j < i; j++)
    {
        ALL_C_j = ALL_C_j + Global_Tasks[j].execution_time;
    }
    
    Global_Tasks[i].priority_promotion_time = max(Global_Tasks[i].deadline - Global_Tasks[i].execution_time - ALL_C_j, 0);
    Global_Tasks[i].next_priority_promotion_time = Global_Tasks[i].priority_promotion_time;

}

}

int compare_task_priority_DP(const void* a, const void* b) {
    Task* ta = (Task*)a;
    Task* tb = (Task*)b;

    if (ta->priority_promotion == tb->priority_promotion)
    {
        return (ta->id - tb->id); //same prioity sort by id, small id hp
    }else{
    //  priority small mins hp
    return (ta->priority_promotion - tb->priority_promotion);
    }
    

}

void check_PPP(int Current_time_DP){

    for (int i = 0; i < NUMBER_TASK; i++)
    {
        if (Global_Tasks[i].next_priority_promotion_time == Current_time_DP)
        {
            Global_Tasks[i].priority_promotion = 0;
        }
    }

}

void Reset_Gloabl_Task(){

    for (int i = 0; i < NUMBER_TASK; i++)
    {
        Global_Tasks[i].remaining_time = 0;
        Global_Tasks[i].next_deadline = Global_Tasks[i].deadline;
        Global_Tasks[i].next_release_time = Global_Tasks[i].release_time;
    }
    

}

void check_realse_DP(int Current_time_DP){

    // 
    for(int i = 0; i < NUMBER_TASK; i++){
        //
        if(Global_Tasks[i].next_release_time == Current_time_DP) {
            Global_Tasks[i].remaining_time = Global_Tasks[i].execution_time; // set remaining_time

            Global_Tasks[i].priority_promotion = Global_Tasks[i].priority;

            if (Global_Tasks[i].next_priority_promotion_time == Current_time_DP)
            {
                Global_Tasks[i].priority_promotion = 0;
            }

            Global_Tasks[i].next_priority_promotion_time = Current_time_DP + Global_Tasks[i].priority_promotion_time;

            Global_Tasks[i].next_release_time = Current_time_DP + Global_Tasks[i].period; //update next_release_time
        }
    }

}

void Run_Task_one_step_DP(Task* Global_Tasks, Task* Ready_Queue, int current_time, int Num_Task_need_run){
    // printf("Time %d \n",Current_time_DP);
    for(int i = 0; i < Num_Task_need_run; i++){
        
        int running_id = Ready_Queue[i].id;

        // running for 1 time
        Global_Tasks[running_id].remaining_time--;

        // printf("Running tasks id %d \n", Global_Tasks[running_id].id);

    }

}


void G_DP_Scheduling(int Simulation_time){
    
    int Current_time_DP = 0;
    // Set_PPP();
    int DP_missDeadline_flag = 0;

    for (Current_time_DP = 0; Current_time_DP < Simulation_time; Current_time_DP++)
    {
        // printf("Time %d \n",Current_time_DP);
        Task Ready_Queue_DP[NUMBER_TASK];

        if (check_deadline(Current_time_DP))
        {
            DP_missDeadline_flag = 1;
        }

        check_realse_DP(Current_time_DP);
        check_PPP(Current_time_DP);


        int Ready_Task_Count = Add_Task_to_RQ(Global_Tasks, Ready_Queue_DP);

        qsort(Ready_Queue_DP, Ready_Task_Count, sizeof(Task), compare_task_priority_DP);

        if (Ready_Task_Count >= NUMBER_PROCESSORS)
        {
            Run_Task_one_step_DP(Global_Tasks, Ready_Queue_DP, Current_time_DP, NUMBER_PROCESSORS);
        }
        
        if (Ready_Task_Count < NUMBER_PROCESSORS)
        {
            Run_Task_one_step_DP(Global_Tasks, Ready_Queue_DP, Current_time_DP, Ready_Task_Count);
        }
        // Print_Task_Set();
        reset_tasks_queue(Ready_Queue_DP, NUMBER_TASK);
    }

    if (DP_missDeadline_flag)
    {
        printf("DP_Scheduling Finished, These Task Miss Deadline");
    }else{
        printf("DP_Scheduling Finished, All Task Meet Deadline!");
    }
            
    
}

