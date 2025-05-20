#include <stdio.h>
#include <stdbool.h>
#include "RTA_DP.h"


// sort task set no need

// rta calculation

int M_R_k_calculation(int Task_i_index, int R_k){

int D_i = Global_Tasks[Task_i_index].deadline;
int C_i = Global_Tasks[Task_i_index].execution_time;
int T_i = Global_Tasks[Task_i_index].period;

    int N_I_L = floor(((double)R_k + (double)D_i - (double)C_i)/(double)T_i);
    int M_R_K = N_I_L * C_i + min(C_i,max(R_k + D_i - C_i - N_I_L * T_i,0));
return M_R_K;

}

int Supper_UB_calculation(int Task_k_index, int R_k){
    int Supper_UB;
    Supper_UB = R_k - Global_Tasks[Task_k_index].execution_time + 1;
return Supper_UB;
}

int s1_calculation(int L, int Task_i_index){
    int D_i = Global_Tasks[Task_i_index].deadline;
    int C_i = Global_Tasks[Task_i_index].execution_time;
    int T_i = Global_Tasks[Task_i_index].period;
    int P_i = Global_Tasks[Task_i_index].priority_promotion_time;

    int N_S1 = floor((double)L - (double)D_i + (double)P_i / (double)T_i) + 1;

    int W_S1 = N_S1 * max(D_i - P_i,C_i);

return W_S1;
}


int s2_calculation(int L, int Task_i_index){
    int D_i = Global_Tasks[Task_i_index].deadline;
    int C_i = Global_Tasks[Task_i_index].execution_time;
    int T_i = Global_Tasks[Task_i_index].period;
    int P_i = Global_Tasks[Task_i_index].priority_promotion_time;

    int N_S2 = max(floor((double)L / (double)T_i),0);

    int W_S2 = N_S2 * C_i + min(max(L- N_S2 * T_i,0),min(C_i,D_i - P_i));

return W_S2;
}

int s3_calculation(int L, int Task_i_index){
    int D_i = Global_Tasks[Task_i_index].deadline;
    int C_i = Global_Tasks[Task_i_index].execution_time;
    int T_i = Global_Tasks[Task_i_index].period;
    int P_i = Global_Tasks[Task_i_index].priority_promotion_time;

    int N_S3 = max(floor((double)L + (double)D_i - (double)P_i/ (double)T_i) - 1,0);

    int Carry_value = min(max(L - N_S3 * T_i - T_i + D_i - P_i,0), min(2*C_i, 2*D_i - 2*P_i));

    int W_S3 = N_S3 * C_i + Carry_value;

return W_S3;


}

int W_r_p_calculation(int Task_i_index, int Task_k_index){
    int D_i = Global_Tasks[Task_i_index].deadline;
    int C_i = Global_Tasks[Task_i_index].execution_time;
    int T_i = Global_Tasks[Task_i_index].period;
    int P_i = Global_Tasks[Task_i_index].priority_promotion_time;
    int P_k = Global_Tasks[Task_k_index].priority_promotion_time;
    int W_r_p = 0;
if (Task_i_index < Task_k_index)
{
    int N_full_execution = floor((double)P_k +(double)D_i-(double)C_i/(double)T_i);  
    W_r_p = N_full_execution * C_i + min(C_i, max(P_k + D_i - C_i - N_full_execution * T_i,0));

}else{
    W_r_p = max(s1_calculation(P_k, Task_i_index), max(s2_calculation(P_k, Task_i_index),s3_calculation(P_k, Task_i_index)));
    W_r_p = min(W_r_p,P_k);
    // W_r_p = min(s1_calculation(P_k, Task_i_index), min(s2_calculation(P_k, Task_i_index),s3_calculation(P_k, Task_i_index)));
}

return W_r_p;
}

int W_p_d_calculation(int Task_i_index, int Task_k_index){
    int W_p_d;
    int D_P_k = Global_Tasks[Task_k_index].deadline - Global_Tasks[Task_k_index].priority_promotion_time;
    if (Task_i_index < Task_k_index)
    {
        W_p_d = max(s1_calculation(D_P_k, Task_i_index), max(s2_calculation(D_P_k, Task_i_index),s3_calculation(D_P_k, Task_i_index)));
        W_p_d = min(W_p_d,D_P_k);
        // W_p_d = min(s1_calculation(D_P_k, Task_i_index), min(s2_calculation(D_P_k, Task_i_index),s3_calculation(D_P_k, Task_i_index)));
    }else{
        W_p_d = 0;
    }
    
    return W_p_d;

}


int W_i_Dk_calculation(int Task_i_index, int Task_k_index){

   return W_r_p_calculation(Task_i_index, Task_k_index) + W_p_d_calculation(Task_i_index, Task_k_index);
    
}


int I_R_K_calculation(int R_K_pre, int Task_i_index, int Task_k_index){
    int I_R_K;

    int W_i_Dk = max(W_i_Dk_calculation(Task_i_index, Task_k_index),0);
    int M_R_k = M_R_k_calculation(Task_i_index, R_K_pre);
    int Supper_UB = Supper_UB_calculation(Task_k_index, R_K_pre);

    // I_R_K = min(W_i_Dk_calculation(Task_i_index, Task_k_index),min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre)));
    I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));
    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}

int I_R_K_calculation_without_w(int R_K_pre, int Task_i_index, int Task_k_index){
    int I_R_K;

    int W_i_Dk = max(W_i_Dk_calculation(Task_i_index, Task_k_index),0);
    int M_R_k = M_R_k_calculation(Task_i_index, R_K_pre);
    int Supper_UB = Supper_UB_calculation(Task_k_index, R_K_pre);

    // I_R_K = min(W_i_Dk_calculation(Task_i_index, Task_k_index),min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre)));
    I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));
    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}


int I_R_K_calculation_with_W(int R_K_pre, int Task_i_index, int Task_k_index){
    int I_R_K;

    int W_i_Dk = max(W_i_Dk_calculation(Task_i_index, Task_k_index),0);
    int M_R_k = M_R_k_calculation(Task_i_index, R_K_pre);
    int Supper_UB = Supper_UB_calculation(Task_k_index, R_K_pre);

    I_R_K = min(W_i_Dk_calculation(Task_i_index, Task_k_index),min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre)));
    // I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));
    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}


int I_R_K_calculation_without_M(int R_K_pre, int Task_i_index, int Task_k_index){
    int I_R_K;

    int W_i_Dk = max(W_i_Dk_calculation(Task_i_index, Task_k_index),0);
    int M_R_k = M_R_k_calculation(Task_i_index, R_K_pre);
    int Supper_UB = Supper_UB_calculation(Task_k_index, R_K_pre);

    I_R_K = min(W_i_Dk_calculation(Task_i_index, Task_k_index),Supper_UB_calculation(Task_k_index, R_K_pre));
    // I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));
    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}


int I_R_K_calculation_only_W(int R_K_pre, int Task_i_index, int Task_k_index){
    int I_R_K;

    int W_i_Dk = max(W_i_Dk_calculation(Task_i_index, Task_k_index),0);
    int M_R_k = M_R_k_calculation(Task_i_index, R_K_pre);
    int Supper_UB = Supper_UB_calculation(Task_k_index, R_K_pre);

    I_R_K = W_i_Dk;
    // I_R_K = min(M_R_k_calculation(Task_i_index, R_K_pre),Supper_UB_calculation(Task_k_index, R_K_pre));
    // printf("W_i_Dk is %d M_R_k is %d Supper_UB is %d\n",W_i_Dk,M_R_k,Supper_UB);

    return I_R_K;
}


int total_IRK_calculation(int R_pre, int Task_k_index){

    int I_R_K = 0;
for (int Task_i_index = 0; Task_i_index < NUMBER_TASK; Task_i_index++)
{
    if (Task_i_index < Task_k_index)
    {
        I_R_K = I_R_K + I_R_K_calculation_with_W(R_pre, Task_i_index, Task_k_index);
    }else{
        if(Task_i_index != Task_k_index){
        I_R_K = I_R_K + I_R_K_calculation_with_W(R_pre, Task_i_index, Task_k_index);
        }
    }


    // if (Task_i_index < Task_k_index)
    // {
    //     I_R_K = I_R_K + I_R_K_calculation_without_w(R_pre, Task_i_index, Task_k_index);
    // }


    
}

return I_R_K;

}


int check_RTA_DP(int Task_k_index) {
    int R_initial = Global_Tasks[Task_k_index].execution_time;
    int R_k = R_initial + floor((double)total_IRK_calculation(R_initial, Task_k_index)/(double)NUMBER_PROCESSORS);

    int R_pre = R_initial;
    int Test_result;
    int tmep_count = 0;
while (R_k != R_pre && tmep_count < 1000)
{
    R_pre = R_k;
    int interf_temp = floor((double)total_IRK_calculation(R_pre, Task_k_index)/(double)NUMBER_PROCESSORS);
    R_k = R_initial + interf_temp;
    // printf("Iteration %d : Rk is %d  \n",tmep_count,R_k);
    tmep_count++;
}




if (R_k < Global_Tasks[Task_k_index].deadline)
{
    Test_result = 1;
}else{
    Test_result = 0;
}

    return Test_result;
}


int RTA_DP_Result(){
    int RTA_DP_flag = 1;
    for (int Task_k_index = 1; Task_k_index < NUMBER_TASK; Task_k_index++)
    {
        if (check_RTA_DP(Task_k_index)){
            // printf("Task id %d Pass The RTA_DP Test \n",Task_k_index);
        }else{
            // printf("Task id %d fail The RTA_DP Test \n",Task_k_index);
            RTA_DP_flag = 0;
        }
    }
    return RTA_DP_flag;
}





int I_R_K_calculation_for_FP(int R_K_pre, int Task_i_index, int Task_k_index){
    int I_R_K;

    // int W_i_Dk = max(W_i_Dk_calculation(Task_i_index, Task_k_index),0);
    int M_R_k = M_R_k_calculation(Task_i_index, R_K_pre);
    int Supper_UB = Supper_UB_calculation(Task_k_index, R_K_pre);

    I_R_K = min(M_R_k,Supper_UB);

    return I_R_K;
}


int total_I_FP_calculation(int R_pre, int Task_k_index){

        int I_R_K = 0;
for (int Task_i_index = 0; Task_i_index < NUMBER_TASK; Task_i_index++)
{

    if (Task_i_index < Task_k_index)
    {
        I_R_K = I_R_K + I_R_K_calculation_for_FP(R_pre, Task_i_index, Task_k_index);
    }


    
}

return I_R_K;

}




int check_RTA_FP(int Task_k_index){
    int R_initial = Global_Tasks[Task_k_index].execution_time;
    int R_k = R_initial + floor((double)total_I_FP_calculation(R_initial, Task_k_index)/(double)NUMBER_PROCESSORS);

    int R_pre = R_initial;
    int Test_result;
    int tmep_count = 0;
while (R_k != R_pre && tmep_count < 1000)
{
    R_pre = R_k;
    int interf_temp = floor((double)total_I_FP_calculation(R_pre, Task_k_index)/(double)NUMBER_PROCESSORS);
    R_k = R_initial + interf_temp;
    // printf("Iteration %d : Rk is %d  \n",tmep_count,R_k);
    tmep_count++;
}




if (R_k < Global_Tasks[Task_k_index].deadline)
{
    Test_result = 1;
}else{
    Test_result = 0;
}

    return Test_result;
}


int RTA_FP_result(){

        int RTA_FP_flag = 1;
    for (int Task_k_index = 1; Task_k_index < NUMBER_TASK; Task_k_index++)
    {
        if (check_RTA_FP(Task_k_index)){
            // printf("Task id %d Pass The RTA_DP Test \n",Task_k_index);
        }else{
            // printf("Task id %d fail The RTA_DP Test \n",Task_k_index);
            RTA_FP_flag = 0;
        }
    }
    return RTA_FP_flag;

}