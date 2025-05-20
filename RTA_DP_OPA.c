#include <stdio.h>
#include <stdbool.h>
#include "RTA_DP.h"


// sort task set no need

// rta calculation
Task Task_dup_RTA_DP_OPA [NUMBER_TASK];


int compare_task_priority_RTA_DP_OPA(const void* a, const void* b) {
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




int M_R_k_calculation_OPA(int Task_i_index, int R_k, Task* Task_Set_need_to_test){

int D_i = Task_Set_need_to_test[Task_i_index].deadline;
int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
int T_i = Task_Set_need_to_test[Task_i_index].period;

    int N_I_L = floor(((double)R_k + (double)D_i - (double)C_i)/(double)T_i);
    int M_R_K = N_I_L * C_i + min(C_i,max(R_k + D_i - C_i - N_I_L * T_i,0));
return M_R_K;

}

int Supper_UB_calculation_OPA(int Task_k_index, int R_k, Task* Task_Set_need_to_test){
    int Supper_UB;
    Supper_UB = R_k - Task_Set_need_to_test[Task_k_index].execution_time + 1;
return Supper_UB;
}

int s1_calculation_OPA(int L, int Task_i_index, Task* Task_Set_need_to_test){
    int D_i = Task_Set_need_to_test[Task_i_index].deadline;
    int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
    int T_i = Task_Set_need_to_test[Task_i_index].period;
    int P_i = Task_Set_need_to_test[Task_i_index].priority_promotion_time;

    int N_S1 = floor((double)L - (double)D_i + (double)P_i / (double)T_i) + 1;

    int W_S1 = N_S1 * max(D_i - P_i,C_i);

return W_S1;
}


int s2_calculation_OPA(int L, int Task_i_index, Task* Task_Set_need_to_test){
    int D_i = Task_Set_need_to_test[Task_i_index].deadline;
    int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
    int T_i = Task_Set_need_to_test[Task_i_index].period;
    int P_i = Task_Set_need_to_test[Task_i_index].priority_promotion_time;

    int N_S2 = max(floor((double)L / (double)T_i),0);

    int W_S2 = N_S2 * C_i + min(max(L- N_S2 * T_i,0),min(C_i,D_i - P_i));

return W_S2;
}

int s3_calculation_OPA(int L, int Task_i_index, Task* Task_Set_need_to_test){
    int D_i = Task_Set_need_to_test[Task_i_index].deadline;
    int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
    int T_i = Task_Set_need_to_test[Task_i_index].period;
    int P_i = Task_Set_need_to_test[Task_i_index].priority_promotion_time;

    int N_S3 = max(floor((double)L + (double)D_i - (double)P_i/ (double)T_i) - 1,0);

    int Carry_value = min(max(L - N_S3 * T_i - T_i + D_i - P_i,0), min(2*C_i, 2*D_i - 2*P_i));

    int W_S3 = N_S3 * C_i + Carry_value;

return W_S3;


}

int W_r_p_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){
    int D_i = Task_Set_need_to_test[Task_i_index].deadline;
    int C_i = Task_Set_need_to_test[Task_i_index].execution_time;
    int T_i = Task_Set_need_to_test[Task_i_index].period;
    int P_i = Task_Set_need_to_test[Task_i_index].priority_promotion_time;
    int P_k = Task_Set_need_to_test[Task_k_index].priority_promotion_time;
    int W_r_p = 0;
if (Task_i_index < Task_k_index)
{
    int N_full_execution = floor((double)P_k +(double)D_i-(double)C_i/(double)T_i);  
    W_r_p = N_full_execution * C_i + min(C_i, max(P_k + D_i - C_i - N_full_execution * T_i,0));

}else{
    W_r_p = max(s1_calculation_OPA(P_k, Task_i_index, Task_Set_need_to_test), max(s2_calculation_OPA(P_k, Task_i_index, Task_Set_need_to_test),s3_calculation_OPA(P_k, Task_i_index, Task_Set_need_to_test)));
    W_r_p = min(W_r_p,P_k);
    // W_r_p = min(s1_calculation(P_k, Task_i_index), min(s2_calculation(P_k, Task_i_index),s3_calculation(P_k, Task_i_index)));
}

return W_r_p;
}

int W_p_d_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){
    int W_p_d;
    int D_P_k = Task_Set_need_to_test[Task_k_index].deadline - Task_Set_need_to_test[Task_k_index].priority_promotion_time;
    if (Task_i_index < Task_k_index)
    {
        W_p_d = max(s1_calculation_OPA(D_P_k, Task_i_index, Task_Set_need_to_test), max(s2_calculation_OPA(D_P_k, Task_i_index, Task_Set_need_to_test),s3_calculation_OPA(D_P_k, Task_i_index, Task_Set_need_to_test)));
        W_p_d = min(W_p_d,D_P_k);
        // W_p_d = min(s1_calculation(D_P_k, Task_i_index), min(s2_calculation(D_P_k, Task_i_index),s3_calculation(D_P_k, Task_i_index)));
    }else{
        W_p_d = 0;
    }
    
    return W_p_d;

}


int W_i_Dk_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){

   return W_r_p_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test) + W_p_d_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test);
    
}




int I_R_K_calculation_with_W_OPA(int R_K_pre, int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test){
    int I_R_K;

    int W_i_Dk = max(W_i_Dk_calculation_OPA(Task_i_index, Task_k_index, Task_Set_need_to_test),0);
    int M_R_k = M_R_k_calculation_OPA(Task_i_index, R_K_pre, Task_Set_need_to_test);
    int Supper_UB = Supper_UB_calculation_OPA(Task_k_index, R_K_pre, Task_Set_need_to_test);

        I_R_K = min(W_i_Dk,min(M_R_k,Supper_UB));
    // I_R_K = min(W_i_Dk_calculation_OPA(Task_i_index, Task_k_index),min(M_R_k_calculation_OPA(Task_i_index, R_K_pre),Supper_UB_calculation_OPA(Task_k_index, R_K_pre)));

    // I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));
    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}








int total_IRK_calculation_OPA(int R_pre, int Task_k_index, int Number_of_remain_Task, Task* Task_Set_need_to_test){

    int I_R_K = 0;
for (int Task_i_index = 0; Task_i_index < Number_of_remain_Task; Task_i_index++)
{
    if (Task_i_index < Task_k_index)
    {
        I_R_K = I_R_K + I_R_K_calculation_with_W_OPA(R_pre, Task_i_index, Task_k_index, Task_Set_need_to_test);
    }else{
        if(Task_i_index != Task_k_index){
        I_R_K = I_R_K + I_R_K_calculation_with_W_OPA(R_pre, Task_i_index, Task_k_index, Task_Set_need_to_test);
        }
    }


    // if (Task_i_index < Task_k_index)
    // {
    //     I_R_K = I_R_K + I_R_K_calculation_without_w(R_pre, Task_i_index, Task_k_index);
    // }


    
}

return I_R_K;

}


int check_RTA_DP_OPA_for_one_task_k(Task* Task_Set_need_to_test, int Task_k_index, int Number_of_remain_Task) {
    int R_initial = Task_Set_need_to_test[Task_k_index].execution_time;
    int R_k = R_initial + floor((double)total_IRK_calculation_OPA(R_initial, Task_k_index, Number_of_remain_Task, Task_Set_need_to_test)/(double)NUMBER_PROCESSORS);

    int R_pre = R_initial;
    int Test_result;
    int tmep_count = 0;
while (R_k != R_pre && tmep_count < 1000)
{
    R_pre = R_k;
    int interf_temp = floor((double)total_IRK_calculation_OPA(R_pre, Task_k_index, Number_of_remain_Task, Task_Set_need_to_test)/(double)NUMBER_PROCESSORS);
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




int check_RTA_DP_OPA(Task* Task_Set_remain, int Number_of_remain_Task){

// for (int i = 0; i < Number_of_remain_Task; i++)
// {
//     printf(" id = %d  period = %d  priority = %d  ppp = %d\n",Task_Set_remain[i].id, Task_Set_remain[i].period, Task_Set_remain[i].priority, Task_Set_remain[i].priority_promotion_time);
// }
// Print_Task_Set();

Task *Task_set_temp = (Task *)calloc(Number_of_remain_Task, sizeof(Task));

    int priority_assign_flag = 0;
    int RTA_DP_OPA_TEST_flag = 1;


// qsort(Task_Set_remain, priority_test, sizeof(Task), compare_task_priority_RTA_DP_OPA);


for (int priority_test = Number_of_remain_Task; priority_test >= 1; priority_test--)
{
    
    for (int Test_index = 0; Test_index < Number_of_remain_Task; Test_index++)
    {
        if (Task_Set_remain[Test_index].priority == 0){

            for (int i = 0; i < Number_of_remain_Task; i++)
                {
                    Task_set_temp[i] = Task_Set_remain[i];
                }


            if (priority_test == 1)
            {
                Task_Set_remain[Test_index].priority = priority_test + NUMBER_TASK;
                Task_Set_remain[Test_index].priority_promotion = priority_test;
                priority_assign_flag = 1;
            }else{

                Task_set_temp[Test_index].priority = priority_test + NUMBER_TASK;
                qsort(Task_set_temp, Number_of_remain_Task, sizeof(Task), compare_task_priority_RTA_DP_OPA);


                if (check_RTA_DP_OPA_for_one_task_k(Task_set_temp, priority_test, Number_of_remain_Task))
                {
                            Task_Set_remain[Test_index].priority = priority_test + NUMBER_TASK;
                            Task_Set_remain[Test_index].priority_promotion = priority_test;
                            priority_assign_flag = 1;
                            break;
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

return RTA_DP_OPA_TEST_flag;
}