#include "main.h"

int Improved_compare_task_priority_RTA_DP_OPA(const void* a, const void* b);
int Improved_M_R_k_calculation_OPA(int Task_i_index, int R_k, Task* Task_Set_need_to_test);
int Improved_Supper_UB_calculation_OPA(int Task_k_index, int R_k, Task* Task_Set_need_to_test);
int Improved_W_r_p_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test);

int Improved_W_p_d_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test);
int Improved_W_i_Dk_calculation_OPA(int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test);
int Improved_I_R_K_calculation_with_W_OPA(int R_K_pre, int Task_i_index, int Task_k_index, Task* Task_Set_need_to_test);
int Improved_total_IRK_calculation_OPA(int R_pre, int Task_k_index, int Number_of_remain_Task, Task* Task_Set_need_to_test);
int Improved_check_RTA_DP_OPA_for_one_task_k(Task* Task_Set_need_to_test, int Task_k_index, int Number_of_remain_Task);
int Improved_RTA_DP_OPA_Result(Task* Task_Set_need_to_test, int Number_of_remain_Task);
int Improved_check_RTA_DP_OPA(Task* Task_Set_remain, int Number_of_remain_Task);