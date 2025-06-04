#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "G_MP.h"

Task DP_Temp[NUMBER_TASK];

void Set_Task_manually(){

    Global_Tasks[0] = (Task){ .id = 0, .priority = 2, .period = 185,  .priority_promotion = 1, .execution_time = 89,  .remaining_time = 0, .deadline = 185,  .next_deadline = 185,  .release_time = 0, .next_release_time = 0, .priority_promotion_time = 0, .next_priority_promotion_time = 0 };
    
    Global_Tasks[1] = (Task){ .id = 1, .priority = 1, .period = 463,  .priority_promotion = 2, .execution_time = 372,  .remaining_time = 0, .deadline = 463,  .next_deadline = 463,  .release_time = 0, .next_release_time = 0, .priority_promotion_time = 3, .next_priority_promotion_time = 3 };
    
    Global_Tasks[2] = (Task){ .id = 2, .priority = 3, .period = 819, .priority_promotion = 3, .execution_time = 180,  .remaining_time = 0, .deadline = 819,  .next_deadline = 819,  .release_time = 0, .next_release_time = 0, .priority_promotion_time = 6, .next_priority_promotion_time = 6 };
    
    // Global_Tasks[3] = (Task){ .id = 3, .priority = 4, .period = 32, .priority_promotion = 4, .execution_time = 17, .remaining_time = 0, .deadline = 31, .next_deadline = 31, .release_time = 0, .next_release_time = 0, .priority_promotion_time = 27, .next_priority_promotion_time = 27 };
    
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


void Set_PPP_Utilization_based(){

for (int i = 0; i < NUMBER_TASK; i++)
{
    int ALL_C_j = 0;
    for (int j = 0; j < i; j++)
    {
        ALL_C_j = ALL_C_j + Global_Tasks[j].execution_time;
    }

    double U_i = (double)Global_Tasks[i].execution_time / (double)Global_Tasks[i].period;
    int D_C = Global_Tasks[i].deadline - Global_Tasks[i].execution_time;
    // int prority_set = floor((1.0 - U_i/TOTAL_UTILIZATION) * (double)D_C);
    
    double U_i_minus1 = 1.0 - U_i;

    // int prority_set = floor((1.0 - U_i) * (double)D_C);
    // int prority_set = floor(pow(U_i_minus1, 2) * (double)Global_Tasks[i].deadline);
    // int prority_set = floor((double)Global_Tasks[i].deadline * 0.9);
    int prority_set = (double)Global_Tasks[i].deadline * PPPprefix;
    // int prority_set = max(Global_Tasks[i].deadline - Global_Tasks[i].execution_time - floor((double)ALL_C_j / (double)NUMBER_PROCESSORS), 0);
    // int heuristic_calcuation = (double)1.0 - (0.02 * (double)NUMBER_PROCESSORS/(double)TOTAL_UTILIZATION);
    // double heuristic_calcuation = ((double)NUMBER_PROCESSORS- 0.8 - (double)TOTAL_UTILIZATION)/(double)10;
    // double heuristic_calcuation = ((double)NUMBER_PROCESSORS- 0.4 - (double)TOTAL_UTILIZATION)/(double)40;
    // int prority_set = Global_Tasks[i].deadline * ((double)1.0 - max(heuristic_calcuation,0));

    // int prority_set = max(Global_Tasks[i].deadline - Global_Tasks[i].execution_time - ALL_C_j, 0);
    // Global_Tasks[i].next_priority_promotion_time = Global_Tasks[i].priority_promotion_time;

    Global_Tasks[i].priority_promotion_time = prority_set;
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
        if (DP_Temp[i].next_priority_promotion_time == Current_time_DP)
        {
            // DP_Temp[i].priority_promotion = 0;
            // DP_Temp[i].priority_promotion = DP_Temp[i].id + 1;
            DP_Temp[i].priority_promotion = DP_Temp[i].priority_promotion - 1;
        }
    }

}

void Reset_Gloabl_Task(){  //DP_Temp

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
        if(DP_Temp[i].next_release_time == Current_time_DP) {
            DP_Temp[i].remaining_time = DP_Temp[i].execution_time; // set remaining_time

            DP_Temp[i].priority_promotion = DP_Temp[i].priority;

            if (DP_Temp[i].next_priority_promotion_time == Current_time_DP)
            {
                DP_Temp[i].priority_promotion = 0;
            }

            DP_Temp[i].next_priority_promotion_time = Current_time_DP + DP_Temp[i].priority_promotion_time;

            DP_Temp[i].next_release_time = Current_time_DP + DP_Temp[i].period; //update next_release_time
        }
    }

}

void Run_Task_one_step_DP(Task* DP_Temp, Task* Ready_Queue, int current_time, int Num_Task_need_run){
    // printf("Time %d \n",Current_time_DP);
    for(int i = 0; i < Num_Task_need_run; i++){
        
        int running_id = Ready_Queue[i].id;

        // running for 1 time
        DP_Temp[running_id].remaining_time--;

        // printf("Running tasks id %d \n", Global_Tasks[running_id].id);

    }

}


int check_deadline_DP(int current_time){

    int missDeadline_flag = 0;
    for(int i = 0; i < NUMBER_TASK; i++){
        
        if(DP_Temp[i].next_deadline == current_time) {

            if (DP_Temp[i].remaining_time > 0)
            {
                // printf("Tasks:%d miss the deadline at time %d \n", Global_Tasks[i].id, current_time);
                missDeadline_flag = 1;
            }
            DP_Temp[i].next_deadline = current_time + DP_Temp[i].next_release_time;


        }
    }
    return missDeadline_flag;
}

void G_DP_Scheduling(int Simulation_time){
    
    for (int i = 0; i < NUMBER_TASK; i++)
    {
        DP_Temp[i]= Global_Tasks[i];
    }
    

    int Current_time_DP = 0;
    // Set_PPP();
    int DP_missDeadline_flag = 0;

    for (Current_time_DP = 0; Current_time_DP < Simulation_time; Current_time_DP++)
    {
        // printf("Time %d \n",Current_time_DP);
        Task Ready_Queue_DP[NUMBER_TASK];

        if (check_deadline_DP(Current_time_DP))
        {
            DP_missDeadline_flag = 1;
        }

        check_realse_DP(Current_time_DP);
        check_PPP(Current_time_DP);


        int Ready_Task_Count = Add_Task_to_RQ(DP_Temp, Ready_Queue_DP);

        qsort(Ready_Queue_DP, Ready_Task_Count, sizeof(Task), compare_task_priority_DP);

        if (Ready_Task_Count >= NUMBER_PROCESSORS)
        {
            Run_Task_one_step_DP(DP_Temp, Ready_Queue_DP, Current_time_DP, NUMBER_PROCESSORS);
        }
        
        if (Ready_Task_Count < NUMBER_PROCESSORS)
        {
            Run_Task_one_step_DP(DP_Temp, Ready_Queue_DP, Current_time_DP, Ready_Task_Count);
        }
        // Print_Task_Set();
        reset_tasks_queue(Ready_Queue_DP, NUMBER_TASK);
    }

    if (DP_missDeadline_flag)
    {
        // printf("DP_Scheduling Finished, These Task Miss Deadline");
        printf("fail DP\n");
    }else{
        printf("DP_Scheduling Finished, All Task Meet Deadline!\n");
        // Reset_Gloabl_Task();
        // Print_Task_Set();
    }
            
    
}

int G_DP_Scheduling_Test(int Simulation_time){

    int Current_time_DP = 0;
    int DP_missDeadline_flag = 0;

    for (Current_time_DP = 0; Current_time_DP < Simulation_time; Current_time_DP++)
    {
        // printf("Time %d \n",Current_time_DP);
        Task Ready_Queue_DP[NUMBER_TASK];

        if (check_deadline_DP(Current_time_DP))
        {
            DP_missDeadline_flag = 1;
        }

        check_realse_DP(Current_time_DP);
        check_PPP(Current_time_DP);


        int Ready_Task_Count = Add_Task_to_RQ(DP_Temp, Ready_Queue_DP);

        qsort(Ready_Queue_DP, Ready_Task_Count, sizeof(Task), compare_task_priority_DP);

        if (Ready_Task_Count >= NUMBER_PROCESSORS)
        {
            Run_Task_one_step_DP(DP_Temp, Ready_Queue_DP, Current_time_DP, NUMBER_PROCESSORS);
        }
        
        if (Ready_Task_Count < NUMBER_PROCESSORS)
        {
            Run_Task_one_step_DP(DP_Temp, Ready_Queue_DP, Current_time_DP, Ready_Task_Count);
        }
        // Print_Task_Set();
        reset_tasks_queue(Ready_Queue_DP, NUMBER_TASK);
    }

    if (DP_missDeadline_flag)
    {
        // printf("DP_Scheduling Finished, These Task Miss Deadline");
        return 0;
    }else{
        // printf("DP_Scheduling Finished, All Task Meet Deadline!\n");
        // Reset_Gloabl_Task();
        // Print_Task_Set();
        return 1;
    }
}

void Reset_DP_Temp(){  //DP_Temp

    for (int i = 0; i < NUMBER_TASK; i++)
    {
        DP_Temp[i]= Global_Tasks[i];
    }

}

void try_all_ppp(int Simulation_time){
    int try_all_ppp_result=0;
    for (int i = 0; i < NUMBER_TASK; i++)
    {
        DP_Temp[i]= Global_Tasks[i];
    }

    for (int pp0 = 0; pp0 < Global_Tasks[0].deadline; pp0++)
    {
        // For task 1:
        for (int pp1 = 0; pp1 < Global_Tasks[1].deadline; pp1++)
        {
            // For task 2:
            for (int pp2 = 0; pp2 < Global_Tasks[2].deadline; pp2++)
            {
                // For task 3:
                for (int pp3 = 0; pp3 < Global_Tasks[3].deadline; pp3++)
                {
                    // Set each task's priority_promotion_time:
                    DP_Temp[0].priority_promotion_time = pp0;
                    DP_Temp[0].next_priority_promotion_time = pp0;
                    DP_Temp[1].priority_promotion_time = pp1;
                    DP_Temp[1].next_priority_promotion_time = pp1;
                    DP_Temp[2].priority_promotion_time = pp2;
                    DP_Temp[2].next_priority_promotion_time = pp2;
                    DP_Temp[3].priority_promotion_time = pp3;
                    DP_Temp[3].next_priority_promotion_time = pp3;

                    int dp_result = G_DP_Scheduling_Test(Simulation_time);
                    if (dp_result)
                    {   try_all_ppp_result = 1;

                        
                        // Print_Task_Set_general(DP_Temp);
                          printf("The possible schedulable priority_promotion_time Setting can be : pp0 = %d pp1 = %d pp2 = %d pp3 = %d\n",pp0,pp1,pp2,pp3);
                        Reset_DP_Temp();
                    }else{Reset_DP_Temp();}
                    
                    // Now call your test function with this combination:
                    
                }
            }
        }
    }
        
    if (try_all_ppp_result)
    {
        printf("DP_Scheduling Finished, All Task Meet Deadline Based on these Priority Promotion Point\n");
        // printf("The Original Task Set Is:\n");
        // Print_Task_Set();
    }else{
        printf("fail DP try\n");
    }
    
    
}
