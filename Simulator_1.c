#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "Simulator_1.h"


int LCM_two_numbers(int num_1, int num_2){

    int a = num_1;
    int b = num_2;

     while (b != 0) {
             int LCM_temp = b;
             b = a % b;
             a = LCM_temp;
         }
         int GCD = a;

         int LCM_num_1_2 = (num_1 / GCD) * num_2;

         return LCM_num_1_2;

}

int Calculate_LCM(Task* tasks, int task_number){

    // int Period_arrary[3];

    // for ( int index_task = 1; index_task < task_number; index_task++)
    // {
    //     Period_arrary[index_task - 1] = tasks[1].period;
    // }
    
        if (task_number == 0) 
        {return 0; }

        int result = tasks[0].period;

        for (int i = 1; i < task_number; i++) {
            result = LCM_two_numbers(result, tasks[i].period);
        }
        
        return result;

    // printf("LCM of given numbers: %d\n", lcm_multiple(numbers, n));

}

void check_realse(int current_time){

    // 
    for(int i = 0; i < NUMBER_TASK; i++){
        //
        if(Global_Tasks[i].next_release_time == current_time) {
            Global_Tasks[i].remaining_time = Global_Tasks[i].execution_time; // set remaining_time
            Global_Tasks[i].next_release_time = current_time + Global_Tasks[i].period; //update next_release_time
        }
    }

}

void check_deadline(int current_time){

    
    for(int i = 0; i < NUMBER_TASK; i++){
        
        if(Global_Tasks[i].next_deadline == current_time) {

            if (Global_Tasks[i].remaining_time > 0)
            {
                printf("Tasks:%d miss the deadline at time %d \n", Global_Tasks[i].id, current_time);
            }
            Global_Tasks[i].next_deadline = current_time + Global_Tasks[i].next_release_time;


        }
    }

}

int compare_task_priority(const void* a, const void* b) {
    Task* ta = (Task*)a;
    Task* tb = (Task*)b;
    // 数值越小优先级越高，所以按 priority 升序排序
    return (ta->priority - tb->priority);
}

int Add_Task_to_RQ(Task* Global_Tasks, Task* Ready_Queue){

    int Ready_Task_Count = 0;

    for(int i = 0; i < NUMBER_TASK; i++){
        if(Global_Tasks[i].remaining_time > 0) {
            Ready_Queue[Ready_Task_Count++] = Global_Tasks[i];
        }
    }

    return Ready_Task_Count;
}

int check_processor_idle_count(Processor* processor){
 
    int idle_processor_count = 0;

    for (int i = 0; i < NUMBER_PROCESSORS; i++)
    {
        if(processor[i].Busy_State == false) {
            idle_processor_count++;
        }
        return idle_processor_count;
    }
    
}

void Run_Task_one_step(Task* Global_Tasks, Task* Ready_Queue, int current_time, int Num_Task_need_run){

            for(int i = 0; i < Num_Task_need_run; i++){
                
                int running_id = Ready_Queue[i].id;

                // running for 1 time
                Global_Tasks[running_id].remaining_time--;

                printf("Running tasks id %d \n", Global_Tasks[running_id].id);

            }

}

void reset_tasks_queue(Task* tasks_queue, int num_tasks) {
    for (int i = 0; i < num_tasks; i++) {
        tasks_queue[i].id = -1;
        tasks_queue[i].priority = 9999;
        tasks_queue[i].period = -1;
        tasks_queue[i].execution_time = -1;
        tasks_queue[i].remaining_time = -1;
        tasks_queue[i].deadline = -1;
        tasks_queue[i].next_deadline = -1;
        tasks_queue[i].release_time = -1;
        tasks_queue[i].next_release_time = -1;
    }
}