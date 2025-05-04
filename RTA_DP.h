#include "main.h"

int check_RTA_DP(int Task_k_index);

int M_R_k_calculation(int Task_i_index, int R_k);
int Supper_UB_calculation(int Task_k_index, int R_k);
int s1_calculation(int L, int Task_i_index);
int s2_calculation(int L, int Task_i_index);
int s3_calculation(int L, int Task_i_index);
int W_r_p_calculation(int Task_i_index, int Task_k_index);
int W_p_d_calculation(int Task_i_index, int Task_k_index);
int W_i_Dk_calculation(int Task_i_index, int Task_k_index);
int I_R_K_calculation(int R_K_pre, int Task_i_index, int Task_k_index);
int total_IRK_calculation(int R_pre, int Task_k_index);
int check_RTA_DP(int Task_k_index);