#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "DA_LC_OPA.h"

Task OPA_Assign_Task_Set[NUMBER_TASK];
Task Task_set_temp[NUMBER_TASK];

int compare_descending_OPA(const void *a, const void *b) {
    return (*(int *)b - *(int *)a); // Reverse order
}

int get_I_i_D_Dk_Ck_OPA(int Task_k_index, int HP_index){

    int D_i = Task_set_temp[HP_index].deadline;
    int C_i = Task_set_temp[HP_index].execution_time;
    int T_i = Task_set_temp[HP_index].period;

    int D_k = Task_set_temp[Task_k_index].deadline;
    int C_k = Task_set_temp[Task_k_index].execution_time;

    int N_i_D_L = floor((double)(D_k + D_i - C_i) / (double)T_i);

    int W_i_D_L = N_i_D_L * C_i + min(C_i, D_k + D_i - C_i - N_i_D_L * T_i);

    int I_i_D_Dk_Ck = min(W_i_D_L, D_k - C_k + 1);
    
    return I_i_D_Dk_Ck;
}

int get_total_inter_of_task_k_OPA(int Task_k_index){

    int total_inter_of_task_k = 0;

    for (int HP_index = Task_k_index - 1; HP_index >= 0; HP_index--)
    {
        total_inter_of_task_k = total_inter_of_task_k + get_I_i_D_Dk_Ck_OPA(Task_k_index, HP_index);
    }
    
 return total_inter_of_task_k;
}

int get_I_i_NC_Dk_Ck_OPA(int Task_k_index, int HP_index){

    int D_i = Task_set_temp[HP_index].deadline;
    int C_i = Task_set_temp[HP_index].execution_time;
    int T_i = Task_set_temp[HP_index].period;

    int D_k = Task_set_temp[Task_k_index].deadline;
    int C_k = Task_set_temp[Task_k_index].execution_time;

    int N_i_NC_Dk = floor((double)D_k / (double)T_i);

    int W_i_NC_Dk = N_i_NC_Dk * C_i + min(C_i, D_k + D_k - N_i_NC_Dk * T_i);

    int I_i_NC_Dk_Ck = min(W_i_NC_Dk, D_k - C_k + 1);
    
    return I_i_NC_Dk_Ck;
}

int get_total_iNC_of_task_k_OPA(int Task_k_index){

  

        int total_iNC_of_task_k = 0;
    
        for (int HP_index = Task_k_index - 1; HP_index >= 0; HP_index--)
        {
            total_iNC_of_task_k = total_iNC_of_task_k + get_I_i_NC_Dk_Ck_OPA(Task_k_index, HP_index);
        }
        
     return total_iNC_of_task_k;
    
}

int get_i_Diff_OPA(int Task_k_index, int HP_index){
    int i_Diff = get_I_i_D_Dk_Ck_OPA(Task_k_index, HP_index) - get_I_i_NC_Dk_Ck_OPA(Task_k_index, HP_index);
    return i_Diff;
}

int get_total_i_Diff_OPA(int Task_k_index){

    int All_i_Diff[NUMBER_TASK];
    int total_i_Diff = 0;

    for (int i_Diff_index = 0; i_Diff_index < NUMBER_TASK; i_Diff_index++)
    {
        All_i_Diff[i_Diff_index] = 0;
    }
    

    for (int HP_index = 0; HP_index <= Task_k_index - 1 ; HP_index++)
    {
        All_i_Diff[HP_index] = get_i_Diff_OPA(Task_k_index, HP_index);
    }
    
    qsort(All_i_Diff, NUMBER_TASK, sizeof(int), compare_descending_OPA);

    for (int Max_k_m = 0; Max_k_m < NUMBER_PROCESSORS - 1; Max_k_m++)
    {
        total_i_Diff = total_i_Diff + All_i_Diff[Max_k_m];
    }
    
    return total_i_Diff;

}

void Print_Task_Set_OPA(){

    float Real_utilization = 0;
    for (int i = 0; i < NUMBER_TASK; i++)
    {
        printf("Task %d State: priority:%d period:%d  execution_time:%d remaining_time:%d ", OPA_Assign_Task_Set[i].id, OPA_Assign_Task_Set[i].priority, OPA_Assign_Task_Set[i].period, OPA_Assign_Task_Set[i].execution_time, OPA_Assign_Task_Set[i].remaining_time);
        printf("deadline:%d next_deadline:%d release_time:%d next_release_time:%d \n", OPA_Assign_Task_Set[i].deadline, OPA_Assign_Task_Set[i].next_deadline, OPA_Assign_Task_Set[i].release_time, OPA_Assign_Task_Set[i].next_release_time);
        printf("Priority Promotion Time:%d next_Priority Promotion Time:%d Priotity_promotion:%d \n", OPA_Assign_Task_Set[i].priority_promotion_time, OPA_Assign_Task_Set[i].next_priority_promotion_time, OPA_Assign_Task_Set[i].priority_promotion);
        Real_utilization = Real_utilization + (float)OPA_Assign_Task_Set[i].execution_time / OPA_Assign_Task_Set[i].period;
    }
    
    
    
    printf("Set Utilization: %f Real Utilization: %f \n",TOTAL_UTILIZATION,Real_utilization);
    
    int Simulation_time = Calculate_LCM(OPA_Assign_Task_Set,NUMBER_TASK);
    printf("LCM = %d \n",Simulation_time);
    
    }

int check_DA_LC_OPA(Task* Task_Set, int priority_test){
    int Task_k_index = priority_test - 1;
    int total_iNC_of_task_k = get_total_iNC_of_task_k_OPA(Task_k_index);
    int total_i_Diff = get_total_i_Diff_OPA(Task_k_index);
   // double a = (double)total_inter_of_task_k / (double)NUMBER_PROCESSORS;

   if((double)Task_Set[Task_k_index].deadline >= floor((double)Task_Set[Task_k_index].execution_time + ((double)total_iNC_of_task_k + (double)total_i_Diff)/ (double)NUMBER_PROCESSORS)){
       return 1;
   }else{
       return 0;
   }
}

int OPA_Assign_Priority(int DA_LC_state){
    int priority_assign_flag = 0;
    int DA_LC_OPA_TEST_flag = 1;

    for (int i = 0; i < NUMBER_TASK; i++)
    {
        OPA_Assign_Task_Set[i] = Global_Tasks[i];
        OPA_Assign_Task_Set[i].priority = 0;
    }// copy task set and set priority to -1  

    
    for (int priority_test = NUMBER_TASK; priority_test >= 1; priority_test--)
    {

        for (int Test_index = 0; Test_index < NUMBER_TASK; Test_index++)
        {
            if (OPA_Assign_Task_Set[Test_index].priority == 0)
            {
                for (int i = 0; i < NUMBER_TASK; i++)
                {
                    Task_set_temp[i] = OPA_Assign_Task_Set[i];
                }

                if (priority_test == 1)
                {
                    OPA_Assign_Task_Set[Test_index].priority = priority_test;
                    priority_assign_flag = 1;
                }else{

                    Task_set_temp[Test_index].priority = priority_test;

                    qsort(Task_set_temp, NUMBER_TASK, sizeof(Task), compare_task_priority);
    
                        if (check_DA_LC_OPA(Task_set_temp,priority_test))// if (1)
                        {
                            OPA_Assign_Task_Set[Test_index].priority = priority_test;
                            priority_assign_flag = 1;
                            break;
                        }
                }
                

                    

            }
        }

        if (!priority_assign_flag)
        {
            if (DA_LC_state)
            {
                 printf("priority:%d fail OPA\n",priority_test);
            }
            
            
            DA_LC_OPA_TEST_flag = 0;
            break;
        }else{
            priority_assign_flag = 0;
        }
        
    }

    // if (DA_LC_OPA_TEST_flag)
    // {   printf("DA-LC OPA Pass, Priority Assign:\n");
    //     Print_Task_Set();
    // }
    
return DA_LC_OPA_TEST_flag;

}










int OPA_Combined_DP(int DP_Check){
    int priority_assign_flag = 0;
    int OPA_Combined_DP_TEST_flag = 1;

    for (int i = 0; i < NUMBER_TASK; i++)
    {
        OPA_Assign_Task_Set[i] = Global_Tasks[i];
        OPA_Assign_Task_Set[i].priority = 0;
    }// copy task set and set priority to -1

    
    for (int priority_test = NUMBER_TASK; priority_test >= 1; priority_test--)
    {

        for (int Test_index = 0; Test_index < NUMBER_TASK; Test_index++)
        {
            if (OPA_Assign_Task_Set[Test_index].priority == 0)
            {
                for (int i = 0; i < NUMBER_TASK; i++)
                {
                    Task_set_temp[i] = OPA_Assign_Task_Set[i];
                }

                if (priority_test == 1)
                {
                    OPA_Assign_Task_Set[Test_index].priority = priority_test + 1 + NUMBER_TASK;
                    priority_assign_flag = 1;
                }else{

                    Task_set_temp[Test_index].priority = priority_test + 1 + NUMBER_TASK;

                    qsort(Task_set_temp, NUMBER_TASK, sizeof(Task), compare_task_priority);
    
                        if (check_DA_LC_OPA(Task_set_temp,priority_test))
                        {
                            OPA_Assign_Task_Set[Test_index].priority = priority_test + 1 + NUMBER_TASK;
                            priority_assign_flag = 1;
                            break;
                        }else{
                            Task_set_temp[Test_index].priority = 0;
                        }
                }
                

                    

            }
        }

        if (!priority_assign_flag)
        {
            if (DP_Check)
            {
                 
                
                Task *Task_remain_need_to_check_dp = (Task *)calloc(priority_test, sizeof(Task));
                for (int need_to_check_DP_index = 0; need_to_check_DP_index < priority_test ; need_to_check_DP_index++)
                {
                    Task_remain_need_to_check_dp[need_to_check_DP_index] = Task_set_temp[need_to_check_DP_index];
                }
                
                qsort(Task_remain_need_to_check_dp, priority_test, sizeof(Task), compare_task_priority);

                // Print_Task_Set_general(Task_remain_need_to_check_dp);

                if (check_RTA_DP_OPA(Task_remain_need_to_check_dp, priority_test))  //if (RTA_DP_OPA_Result(Task_remain_need_to_check_dp, priority_test))
                {
                    OPA_Combined_DP_TEST_flag = 1;
                }else{OPA_Combined_DP_TEST_flag = 0;}
                

                

                // if (priority_test != 10)
                // {   printf("priority:%d fail OPA\n",priority_test);
                //     Print_Task_Set_general(Task_remain_need_to_check_dp);
                // }
                // printf("priority:%d fail OPA\n",priority_test);
                //     Print_Task_Set_general(Task_remain_need_to_check_dp);
                
                

            }
            
            
            
            break;
        }else{
            priority_assign_flag = 0;
        }
        
    }

    // if (DA_LC_OPA_TEST_flag)
    // {   printf("DA-LC OPA Pass, Priority Assign:\n");
    //     Print_Task_Set();
    // }
    
return OPA_Combined_DP_TEST_flag;

}


