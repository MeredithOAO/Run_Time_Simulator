#include "main.h"
#include "Simulator_1.h"
#include "Task_generate.h"
#include "RTA_DP_OPA.h"

int compare_descending_OPA(const void *a, const void *b);

int get_I_i_D_Dk_Ck_OPA(int Task_k_index, int HP_index);
int get_total_inter_of_task_k_OPA(int Task_k_index);



int get_I_i_NC_Dk_Ck_OPA(int Task_k_index, int HP_index);
int get_total_iNC_of_task_k_OPA(int Task_k_index);

int get_i_Diff_OPA(int Task_k_index, int HP_index);
int get_total_i_Diff_OPA(int Task_k_index);


void Print_Task_Set_OPA();
int check_DA_LC_OPA(Task* Task_Set, int priority_test);
int OPA_Assign_Priority(int DA_LC_state);


int OPA_Combined_DP(int DP_Check);

int check_DA_LC_OPA_combined_DP(Task* Task_Set, int priority_test);