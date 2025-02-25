#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_TASK 4
#define NUMBER_PROCESSORS  2

#define Generate_test_on 0
#define Generate_and_run_test_on 0
#define TOTAL_UTILIZATION 2.8
#define MIN_PERIOD 2
#define MAX_PERIOD 40

typedef struct {
    int id;
    int priority;
    int period;       
    int execution_time;         
    int remaining_time;
    int deadline;
    int next_deadline;     
    int release_time; 
    int next_release_time;
} Task;

typedef struct {
    int processor_id;
    int runnin_task_id;
    bool Busy_State;
} Processor;

Processor processor[NUMBER_PROCESSORS] = {
    {.processor_id = 0, .runnin_task_id = -1, .Busy_State = false},
    {.processor_id = 1, .runnin_task_id = -1, .Busy_State = false}
};

Task Global_Tasks[NUMBER_TASK] = {
    { .id = 0, .priority = 1, .period = 4,   .execution_time = 1,  .remaining_time = 0, .deadline = 3,  .next_deadline = 3,  .release_time = 0, .next_release_time = 0 },
    { .id = 1, .priority = 2, .period = 8,   .execution_time = 5,  .remaining_time = 0, .deadline = 6,  .next_deadline = 6,  .release_time = 0, .next_release_time = 0 },
    { .id = 2, .priority = 3, .period = 16,  .execution_time = 7,  .remaining_time = 0, .deadline = 9,  .next_deadline = 9,  .release_time = 0, .next_release_time = 0 },
    { .id = 3, .priority = 4, .period = 32,  .execution_time = 17, .remaining_time = 0, .deadline = 31, .next_deadline = 31, .release_time = 0, .next_release_time = 0 }
};

void generate_task_set(Task *tasks, int num_tasks, double total_util, int min_period, int max_period)
{
//
double *util = (double *)malloc(sizeof(double) * num_tasks);
double sumU = total_util;
int i;

// UUnifast allocate the Utilization
for (i = 0; i < num_tasks - 1; i++) {
double rand_val = (double)rand() / RAND_MAX;  // rand [0,1) 
// ui = sumU * (1 - x^(1/(n-i)))
double tmp = pow(rand_val, 1.0 / (num_tasks - i - 1));
util[i] = sumU * (1 - tmp);
sumU = sumU * tmp;
}
// last task
util[num_tasks - 1] = sumU;

// period and c
for (i = 0; i < num_tasks; i++) {
// [min_period, max_period]
int period = (rand() % (max_period - min_period + 1)) + min_period;

// c = ui * period
// 向上取整，以保证任务 C_i 不为 0
int execution_time = (int)ceil(util[i] * period);
if (execution_time < 1) {
execution_time = 1;  // c >= 1
}

// 
tasks[i].id              = i;
tasks[i].priority        = i + 1; 
tasks[i].period          = period;
tasks[i].execution_time  = execution_time;
tasks[i].remaining_time  = 0;

// deadline = period 
tasks[i].deadline        = period;
tasks[i].next_deadline   = period; 
tasks[i].release_time    = 0;
tasks[i].next_release_time = 0;
}

free(util);
}


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


void main() {

    srand(time(NULL)); // random seed

    int current_time = 0;   //   Time 

    if (Generate_test_on)
    {

    //generate_task_set 
    generate_task_set(Global_Tasks, NUMBER_TASK, TOTAL_UTILIZATION, MIN_PERIOD, MAX_PERIOD);
    float Real_utilization = 0;
    for (int i = 0; i < NUMBER_TASK; i++)
    {
        printf("Task %d State: priority:%d period:%d  execution_time:%d remaining_time:%d ", Global_Tasks[i].id, Global_Tasks[i].priority, Global_Tasks[i].period, Global_Tasks[i].execution_time, Global_Tasks[i].remaining_time);
        printf("deadline:%d next_deadline:%d release_time:%d next_release_time:%d \n", Global_Tasks[i].deadline, Global_Tasks[i].next_deadline, Global_Tasks[i].release_time, Global_Tasks[i].next_release_time);
        Real_utilization = Real_utilization + (float)Global_Tasks[i].execution_time / Global_Tasks[i].period;
    }



    printf("Set Utilization: %f Real Utilization: %f \n",TOTAL_UTILIZATION,Real_utilization);

    int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);
    printf("LCM = %d \n",Simulation_time);

    }
    

    
    if (!Generate_test_on || (Generate_and_run_test_on && Generate_test_on))
{
    int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);

    printf("LCM = %d \n",Simulation_time);




   
    


    for(current_time = 0; current_time < Simulation_time; current_time++) {
        
        printf("Time %d: \n", current_time);

        Task Ready_Queue[NUMBER_TASK];

        check_realse(current_time);
        check_deadline(current_time);

        int Ready_Task_Count = Add_Task_to_RQ(Global_Tasks, Ready_Queue);

        qsort(Ready_Queue, Ready_Task_Count, sizeof(Task), compare_task_priority);

        int idle_processor_count = check_processor_idle_count(processor);
    

        if (Ready_Task_Count >= NUMBER_PROCESSORS)
        {
            Run_Task_one_step(Global_Tasks, Ready_Queue, current_time, NUMBER_PROCESSORS);
        }
        
        if (Ready_Task_Count < NUMBER_PROCESSORS)
        {
            Run_Task_one_step(Global_Tasks, Ready_Queue, current_time, Ready_Task_Count);
        }
        
        
 

        // for (int i = 0; i < NUMBER_TASK; i++)
        // {
        //     printf("Task %d State: priority:%d remaining_time:%d next_release_time:%d \n", Global_Tasks[i].id, Global_Tasks[i].priority, Global_Tasks[i].remaining_time, Global_Tasks[i].next_release_time);
        // }


        reset_tasks_queue(Ready_Queue,NUMBER_TASK);
    }
}
}

//./main.exe | tee output.txt