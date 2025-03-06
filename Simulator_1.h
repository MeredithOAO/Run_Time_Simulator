#ifndef SIMULATOR_1
#define SIMULATOR_1

#include "main.h"



int LCM_two_numbers(int num_1, int num_2);

int Calculate_LCM(Task* tasks, int task_number);

void check_realse(int current_time);

int check_deadline(int current_time);
float Calculate_Real_U();
int compare_task_priority(const void* a, const void* b);

int Add_Task_to_RQ(Task* Global_Tasks, Task* Ready_Queue);

// int check_processor_idle_count(Processor* processor);

void Run_Task_one_step(Task* Global_Tasks, Task* Ready_Queue, int current_time, int Num_Task_need_run);

void reset_tasks_queue(Task* tasks_queue, int num_tasks);


#endif