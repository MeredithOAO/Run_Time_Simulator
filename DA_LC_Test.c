#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#include "DA_LC_Test.h"

Task OPA_Assign_Task_Set[NUMBER_TASK];

int compare_descending(const void *a, const void *b) {
    return (*(int *)b - *(int *)a); // Reverse order
}

int get_I_i_D_Dk_Ck(int Task_k_index, int HP_index){

    int D_i = Global_Tasks[HP_index].deadline;
    int C_i = Global_Tasks[HP_index].execution_time;
    int T_i = Global_Tasks[HP_index].period;

    int D_k = Global_Tasks[Task_k_index].deadline;
    int C_k = Global_Tasks[Task_k_index].execution_time;

    int N_i_D_L = floor((double)(D_k + D_i - C_i) / (double)T_i);

    int W_i_D_L = N_i_D_L * C_i + min(C_i, D_k + D_i - C_i - N_i_D_L * T_i);

    int I_i_D_Dk_Ck = min(W_i_D_L, D_k - C_k + 1);
    
    return I_i_D_Dk_Ck;
}


int get_total_inter_of_task_k(int Task_k_index){

    int total_inter_of_task_k = 0;

    for (int HP_index = Task_k_index - 1; HP_index >= 0; HP_index--)
    {
        total_inter_of_task_k = total_inter_of_task_k + get_I_i_D_Dk_Ck(Task_k_index, HP_index);
    }
    
 return total_inter_of_task_k;
}


int check_DA(int Task_k_index){

     int total_inter_of_task_k = get_total_inter_of_task_k(Task_k_index);

    // double a = (double)total_inter_of_task_k / (double)NUMBER_PROCESSORS;

    if((double)Global_Tasks[Task_k_index].deadline >= floor((double)Global_Tasks[Task_k_index].execution_time + (double)total_inter_of_task_k / (double)NUMBER_PROCESSORS)){
        return 1;
    }else{
        return 0;
    }
}


void print_result_of_DA(){
    printf("Result of DA Test:  \n");
    for (int Task_k_index = NUMBER_TASK - 1; Task_k_index >= 1; Task_k_index--)
    {
        if (check_DA(Task_k_index)){
            printf("Task id %d Pass The DA Test \n",Task_k_index);
        }else{
            printf("Task id %d Fail The DA Test \n",Task_k_index);
        }
    }
    

}


int get_I_i_NC_Dk_Ck(int Task_k_index, int HP_index){

    int D_i = Global_Tasks[HP_index].deadline;
    int C_i = Global_Tasks[HP_index].execution_time;
    int T_i = Global_Tasks[HP_index].period;

    int D_k = Global_Tasks[Task_k_index].deadline;
    int C_k = Global_Tasks[Task_k_index].execution_time;

    int N_i_NC_Dk = floor((double)D_k / (double)T_i);

    int W_i_NC_Dk = N_i_NC_Dk * C_i + min(C_i, D_k + D_k - N_i_NC_Dk * T_i);

    int I_i_NC_Dk_Ck = min(W_i_NC_Dk, D_k - C_k + 1);
    
    return I_i_NC_Dk_Ck;
}

int get_total_iNC_of_task_k(int Task_k_index){

  

        int total_iNC_of_task_k = 0;
    
        for (int HP_index = Task_k_index - 1; HP_index >= 0; HP_index--)
        {
            total_iNC_of_task_k = total_iNC_of_task_k + get_I_i_NC_Dk_Ck(Task_k_index, HP_index);
        }
        
     return total_iNC_of_task_k;
    
}

int get_i_Diff(int Task_k_index, int HP_index){
    int i_Diff = get_I_i_D_Dk_Ck(Task_k_index, HP_index) - get_I_i_NC_Dk_Ck(Task_k_index, HP_index);
    return i_Diff;
}


int get_total_i_Diff(int Task_k_index){

    int All_i_Diff[NUMBER_TASK];
    int total_i_Diff = 0;

    for (int i_Diff_index = 0; i_Diff_index < NUMBER_TASK; i_Diff_index++)
    {
        All_i_Diff[i_Diff_index] = 0;
    }
    

    for (int HP_index = 0; HP_index <= Task_k_index - 1 ; HP_index++)
    {
        All_i_Diff[HP_index] = get_i_Diff(Task_k_index, HP_index);
    }
    
    qsort(All_i_Diff, NUMBER_TASK, sizeof(int), compare_descending);

    for (int Max_k_m = 0; Max_k_m < NUMBER_PROCESSORS - 1; Max_k_m++)
    {
        total_i_Diff = total_i_Diff + All_i_Diff[Max_k_m];
    }
    
    return total_i_Diff;

}


int check_DA_LC(int Task_k_index){

    int total_iNC_of_task_k = get_total_iNC_of_task_k(Task_k_index);
    int total_i_Diff = get_total_i_Diff(Task_k_index);
   // double a = (double)total_inter_of_task_k / (double)NUMBER_PROCESSORS;

   if((double)Global_Tasks[Task_k_index].deadline >= floor((double)Global_Tasks[Task_k_index].execution_time + ((double)total_iNC_of_task_k + (double)total_i_Diff)/ (double)NUMBER_PROCESSORS)){
       return 1;
   }else{
       return 0;
   }
}

int print_result_of_DA_LC(){
    int DA_LC_flag = 1;
    // printf("\nResult of DA-LC Test:(M = %d) \n", NUMBER_PROCESSORS);
    for (int Task_k_index = NUMBER_TASK - 1; Task_k_index >= 1; Task_k_index--)
    {
        if (check_DA_LC(Task_k_index)){
            //  printf("Task id %d Pass The DA-LC-DM Test \n",Task_k_index);
        }else{
            //  printf("Task id %d Fail The DA-LC-DM Test \n",Task_k_index);
            DA_LC_flag = 0;
        }
    }
    return DA_LC_flag;
}