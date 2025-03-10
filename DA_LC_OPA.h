#include "main.h"
#include "Simulator_1.h"

int compare_descending(const void *a, const void *b);

int get_I_i_D_Dk_Ck(int Task_k_index, int HP_index);
int get_total_inter_of_task_k(int Task_k_index);
int check_DA(int Task_k_index);
void print_result_of_DA();


int get_I_i_NC_Dk_Ck(int Task_k_index, int HP_index);
int get_total_iNC_of_task_k(int Task_k_index);

int get_i_Diff(int Task_k_index, int HP_index);
int get_total_i_Diff(int Task_k_index);
int check_DA_LC(int Task_k_index);
int print_result_of_DA_LC();

void Print_Task_Set_OPA(Task* Task_set);
int print_result_of_DA_LC();
int check_DA_LC_OPA(Task* Task_Set, int priority_test);
int OPA_Assign_Priority();