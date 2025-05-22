#include <stdio.h>
#include <stdbool.h>
#include "RTA_DP_OPA.h"


// sort task set no need

// rta calculation
// Task Task_dup_RTA_DP_OPA [NUMBER_TASK];


int Improved_compare_task_priority_RTA_DP_OPA(const void* a, const void* b) {
    Task* ta = (Task*)a;
    Task* tb = (Task*)b;

    if (ta->priority == tb->priority)
    {
        return (ta->id - tb->id); //same prioity sort by id, small id hp
    }else{
    //  priority small mins hp
    return (ta->priority - tb->priority);
    }
    

}




int Improved_M_R_k_calculation_OPA(int Task_i_index, int R_k, Task* Task_Set_need_to_test){

int D_i = Task_Set_need_to_test[Task_i_index].deadline;
int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
int T_i = Task_Set_need_to_test[Task_i_index].period;

    int N_I_L = floor(((double)R_k + (double)D_i - (double)C_i)/(double)T_i);
    int M_R_K = N_I_L * C_i + min(C_i,max(R_k + D_i - C_i - N_I_L * T_i,0));
return M_R_K;

}

int Improved_Supper_UB_calculation_OPA(int Task_k_index, int R_k, Task* Task_Set_need_to_test){
    int Supper_UB;
    Supper_UB = R_k - Task_Set_need_to_test[Task_k_index].execution_time + 1;
return Supper_UB;
}



int Improved_W_r_p_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){
    int D_i = Task_Set_need_to_test[Task_i_index].deadline;
    int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
    int T_i = Task_Set_need_to_test[Task_i_index].period;
    int P_i = Task_Set_need_to_test[Task_i_index].priority_promotion_time;
    int P_k = Task_Set_need_to_test[Task_k_index].priority_promotion_time;
    int W_r_p = 0;

    int N_i_low = floor(((double)P_k + (double)D_i - (double)C_i) / (double)T_i);
    int E_p = max(P_k + D_i - C_i - N_i_low * T_i,0);


    int C_i_new_for_i_bigger_k = min(C_i,D_i - P_i);
    int N_i_low_new_for_i_bigger_k = floor(((double)P_k + (double)D_i - (double)C_i_new_for_i_bigger_k) / (double)T_i);
    int E_p_new_for_i_bigger_k = max(P_k + D_i - C_i_new_for_i_bigger_k - N_i_low_new_for_i_bigger_k * T_i,0);

if (Task_i_index < Task_k_index)
{
    W_r_p = N_i_low * C_i + min(E_p,C_i);

}else{
    W_r_p = N_i_low_new_for_i_bigger_k * C_i_new_for_i_bigger_k + min(max(E_p_new_for_i_bigger_k - P_i,0),C_i_new_for_i_bigger_k);
}

return W_r_p;
}

int Improved_W_p_d_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){

    int D_i = Task_Set_need_to_test[Task_i_index].deadline;
    int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
    int T_i = Task_Set_need_to_test[Task_i_index].period;
    int P_i = Task_Set_need_to_test[Task_i_index].priority_promotion_time;
    int P_k = Task_Set_need_to_test[Task_k_index].priority_promotion_time;
    int D_k = Task_Set_need_to_test[Task_k_index].deadline;

    int N_i_low = floor(((double)P_k + (double)D_i - (double)C_i) / (double)T_i);
    int E_p = max(P_k + D_i - C_i - N_i_low * T_i,0);

    int W_p_d = 0;

    int N_high = ceil(((double)P_k + (double)D_i - (double)C_i) / (double)T_i);
    int L_high = max(D_k + D_i - C_i - N_high * T_i - P_i ,0);
    int C_i_new = min(D_i - P_i,C_i);
    int E_d = min(C_i_new, max(L_high - floor((double)L_high / (double)T_i) * T_i,0));
    int E_addi = min(D_i - P_i, C_i - E_p);

    if (Task_i_index < Task_k_index)
    {
        
        W_p_d = floor((double)L_high / (double)T_i) * C_i_new + E_d + max(E_addi,0);
    }else{
        W_p_d = 0;
    }
    
    return W_p_d;

}


int Improved_W_i_Dk_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){

    int low_window = Improved_W_r_p_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test);
    int high_window = Improved_W_p_d_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test);
    // printf("low_window is %d  high_window is %d \n",low_window,high_window);

   return  low_window + high_window;
    
}




int Improved_I_R_K_calculation_with_W_OPA(int R_K_pre, int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){
    int I_R_K;

    int W_i_Dk = max(Improved_W_i_Dk_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test),0);
    int M_R_k = Improved_M_R_k_calculation_OPA(Task_i_index, R_K_pre, Task_Set_need_to_test);
    int Supper_UB = Improved_Supper_UB_calculation_OPA(Task_k_index, R_K_pre, Task_Set_need_to_test);

        I_R_K = min(W_i_Dk,min(M_R_k,Supper_UB));
    // I_R_K = min(W_i_Dk_calculation_OPA(Task_i_index, Task_k_index),min(M_R_k_calculation_OPA(Task_i_index, R_K_pre),Supper_UB_calculation_OPA(Task_k_index, R_K_pre)));

    // I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));

    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}








int Improved_total_IRK_calculation_OPA(int R_pre, int Task_k_index, int Number_of_remain_Task, Task* Task_Set_need_to_test){

    int I_R_K = 0;
for (int Task_i_index = 0; Task_i_index < Number_of_remain_Task; Task_i_index++)
{
    if (Task_i_index < Task_k_index)
    {
        I_R_K = I_R_K + Improved_I_R_K_calculation_with_W_OPA(R_pre, Task_i_index, Task_k_index, Task_Set_need_to_test);
    }else{
        if(Task_i_index != Task_k_index){
        I_R_K = I_R_K + Improved_I_R_K_calculation_with_W_OPA(R_pre, Task_i_index, Task_k_index, Task_Set_need_to_test);
        }
    }


    
}

return I_R_K;

}


int Improved_check_RTA_DP_OPA_for_one_task_k(Task* Task_Set_need_to_test, int Task_k_index, int Number_of_remain_Task) {
    int R_initial = Task_Set_need_to_test[Task_k_index].execution_time;
    int R_k = R_initial + floor((double)Improved_total_IRK_calculation_OPA(R_initial, Task_k_index, Number_of_remain_Task, Task_Set_need_to_test)/(double)NUMBER_PROCESSORS);

    int R_pre = R_initial;
    int Test_result;
    int tmep_count = 0;
while (R_k != R_pre && tmep_count < 1000)
{
    R_pre = R_k;
    int interf_temp = floor((double)Improved_total_IRK_calculation_OPA(R_pre, Task_k_index, Number_of_remain_Task, Task_Set_need_to_test)/(double)NUMBER_PROCESSORS);
    R_k = R_initial + interf_temp;
    // printf("Iteration %d : Rk is %d  \n",tmep_count,R_k);
    tmep_count++;
}




if (R_k < Task_Set_need_to_test[Task_k_index].deadline)
{
    Test_result = 1;
}else{
    Test_result = 0;
}

    return Test_result;
}



int Improved_RTA_DP_OPA_Result(Task* Task_Set_need_to_test, int Number_of_remain_Task){
    int RTA_DP_flag = 1;
    for (int Task_k_index = 1; Task_k_index < Number_of_remain_Task; Task_k_index++)
    {
        if (Improved_check_RTA_DP_OPA_for_one_task_k(Task_Set_need_to_test, Task_k_index, Number_of_remain_Task)){
            // printf("Task id %d Pass The RTA_DP Test \n",Task_k_index);
        }else{
            // printf("Task id %d fail The RTA_DP Test \n",Task_k_index);
            RTA_DP_flag = 0;
        }
    }
    return RTA_DP_flag;
}




int Improved_check_RTA_DP_OPA(Task* Task_Set_remain, int Number_of_remain_Task){

for (int i = 0; i < Number_of_remain_Task; i++)
{
    Task_Set_remain[i].priority = 0;
}
// Print_Task_Set();

Task *Task_set_temp_opa = (Task *)calloc(Number_of_remain_Task, sizeof(Task));
Task *Task_set_need_to_try = (Task *)calloc(Number_of_remain_Task, sizeof(Task));

for (int i = 0; i < Number_of_remain_Task; i++)
{
    Task_set_need_to_try[i] = Task_Set_remain[i];
}


    int priority_assign_flag = 0;
    int RTA_DP_OPA_TEST_flag = 1;


// qsort(Task_Set_remain, priority_test, sizeof(Task), compare_task_priority_RTA_DP_OPA);


for (int priority_test = Number_of_remain_Task; priority_test >= 1; priority_test--)
{
    
    // 
    // for (int Test_index = Number_of_remain_Task - 1; Test_index >= 0; Test_index--)
    for (int Test_index = 0; Test_index < Number_of_remain_Task; Test_index++)
    {
        if (Task_set_need_to_try[Test_index].priority == 0){

            for (int i = 0; i < Number_of_remain_Task; i++)
                {
                    Task_set_temp_opa[i] = Task_set_need_to_try[i];
                }


            if (priority_test == 1)
            {
                Task_set_need_to_try[Test_index].priority = priority_test + NUMBER_TASK;
                Task_set_need_to_try[Test_index].priority_promotion = priority_test;
                priority_assign_flag = 1;
            }else{

                Task_set_temp_opa[Test_index].priority = priority_test + NUMBER_TASK;
                qsort(Task_set_temp_opa, Number_of_remain_Task, sizeof(Task), Improved_compare_task_priority_RTA_DP_OPA);

                // Print_Task_Set_general(Task_set_temp_opa);
                if (Improved_check_RTA_DP_OPA_for_one_task_k(Task_set_temp_opa, priority_test - 1, Number_of_remain_Task))
                {
                            Task_set_need_to_try[Test_index].priority = priority_test + NUMBER_TASK;
                            Task_set_need_to_try[Test_index].priority_promotion = priority_test;
                            priority_assign_flag = 1;
                            // Print_Task_Set_general(Task_set_need_to_try);
                            break;
                }else{
                            Task_set_temp_opa[Test_index].priority = 0;
                        }
                

            }
            

        }
    }
    



    if (!priority_assign_flag){
    RTA_DP_OPA_TEST_flag = 0;
    break;
    }else{
            priority_assign_flag = 0;
        }
}

// if (!RTA_DP_OPA_TEST_flag)
// {
//     Print_Task_Set_general(Task_Set_remain);
//     printf("%d\n",Number_of_remain_Task);
// }


// Print_Task_Set_general(Task_set_need_to_try);

return RTA_DP_OPA_TEST_flag;
}




int Improved_W_i_Dk_calculation_for_DP_Try(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){
    int I_R_K;

    int W_i_Dk = max(Improved_W_i_Dk_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test),0);


    return W_i_Dk;
}



int Improved_DP_try_I_R_K_calculation_OPA(int Task_k_index, int Number_of_remain_Task, Task* Task_Set_need_to_test){

    int I_R_K = 0;
    int I_R_K_bound = Task_Set_need_to_test[Task_k_index].deadline - Task_Set_need_to_test[Task_k_index].execution_time + 1;

for (int Task_i_index = 0; Task_i_index < Number_of_remain_Task; Task_i_index++)
{
    if (Task_i_index < Task_k_index)
    {
        I_R_K = I_R_K + min(Improved_W_i_Dk_calculation_for_DP_Try( Task_i_index, Task_k_index, Task_Set_need_to_test),I_R_K_bound);
        // printf("Task k is task %d Task i is task %d IRK = %d\n",Task_Set_need_to_test[Task_k_index].id, Task_Set_need_to_test[Task_i_index].id,I_R_K);
    }else{
        if(Task_i_index != Task_k_index){
        I_R_K = I_R_K + min(Improved_W_i_Dk_calculation_for_DP_Try( Task_i_index, Task_k_index, Task_Set_need_to_test),I_R_K_bound);
        // printf("Task k is task %d Task i is task %d IRK = %d\n",Task_Set_need_to_test[Task_k_index].id, Task_Set_need_to_test[Task_i_index].id,I_R_K);
        }
    }


    
}

return I_R_K;
}



int Improved_check_DP_try_OPA_for_one_task_k(Task* Task_Set_need_to_test, int Task_k_index, int Number_of_remain_Task) {

    
    int Test_result = 0;

    int DP_try_result_value = Task_Set_need_to_test[Task_k_index].execution_time + floor((double)Improved_DP_try_I_R_K_calculation_OPA(Task_k_index, Number_of_remain_Task, Task_Set_need_to_test)/(double)NUMBER_PROCESSORS);
    // printf("Now test the Task_k_id = %d  DP_try_result_value = %d  deadline is %d\n",Task_k_index,DP_try_result_value,Task_Set_need_to_test[Task_k_index].deadline);
if (DP_try_result_value <= Task_Set_need_to_test[Task_k_index].deadline)
{
    Test_result = 1;
}else{
    Test_result = 0;
}
    // printf("result is %d\n",Test_result);
    return Test_result;
}


int Improved_check_DP_try(Task* Task_Set_need_to_test, int Number_of_remain_Task){
int result = 1;
for (int Task_k_index = 0; Task_k_index < NUMBER_TASK; Task_k_index++)
{
    if (!Improved_check_DP_try_OPA_for_one_task_k(Task_Set_need_to_test, Task_k_index, Number_of_remain_Task))
    {
        result = 0;
    }
    
    
}

return result;

}



int Improved_check_DP_try_OPA(Task* Task_Set_remain, int Number_of_remain_Task){

for (int i = 0; i < Number_of_remain_Task; i++)
{
    Task_Set_remain[i].priority = 0;
}
// Print_Task_Set();

Task *Task_set_temp_opa = (Task *)calloc(Number_of_remain_Task, sizeof(Task));
Task *Task_set_need_to_try = (Task *)calloc(Number_of_remain_Task, sizeof(Task));

for (int i = 0; i < Number_of_remain_Task; i++)
{
    Task_set_need_to_try[i] = Task_Set_remain[i];
}


    int priority_assign_flag = 0;
    int RTA_DP_OPA_TEST_flag = 1;


// qsort(Task_Set_remain, priority_test, sizeof(Task), compare_task_priority_RTA_DP_OPA);


for (int priority_test = Number_of_remain_Task; priority_test >= 1; priority_test--)
{
    
    // 
    // for (int Test_index = Number_of_remain_Task - 1; Test_index >= 0; Test_index--)
    for (int Test_index = 0; Test_index < Number_of_remain_Task; Test_index++)
    {
        if (Task_set_need_to_try[Test_index].priority == 0){

            for (int i = 0; i < Number_of_remain_Task; i++)
                {
                    Task_set_temp_opa[i] = Task_set_need_to_try[i];
                }


            if (priority_test == 1)
            {
                Task_set_need_to_try[Test_index].priority = priority_test + NUMBER_TASK;
                Task_set_need_to_try[Test_index].priority_promotion = priority_test;
                priority_assign_flag = 1;
            }else{

                Task_set_temp_opa[Test_index].priority = priority_test + NUMBER_TASK;
                qsort(Task_set_temp_opa, Number_of_remain_Task, sizeof(Task), Improved_compare_task_priority_RTA_DP_OPA);

                // Print_Task_Set_general(Task_set_temp_opa);
                if (Improved_check_DP_try_OPA_for_one_task_k(Task_set_temp_opa, priority_test-1 , Number_of_remain_Task))
                {
                            Task_set_need_to_try[Test_index].priority = priority_test + NUMBER_TASK;
                            Task_set_need_to_try[Test_index].priority_promotion = priority_test;
                            priority_assign_flag = 1;
                            // Print_Task_Set_general(Task_set_need_to_try);
                            // printf("Task id %d is assigned to priority %d\n",Task_set_need_to_try[Test_index].id,priority_test);
                            break;
                }else{
                            Task_set_temp_opa[Test_index].priority = 0;
                            // printf("Task id %d try to assign to priority %d failed\n",Task_set_need_to_try[Test_index].id,priority_test);
                        }
                

            }
            

        }
    }
    



    if (!priority_assign_flag){
    RTA_DP_OPA_TEST_flag = 0;
    break;
    }else{
            priority_assign_flag = 0;
        }
}

// if (!RTA_DP_OPA_TEST_flag)
// {
//     Print_Task_Set_general(Task_Set_remain);
//     printf("%d\n",Number_of_remain_Task);
// }


// Print_Task_Set_general(Task_set_need_to_try);

return RTA_DP_OPA_TEST_flag;
}


