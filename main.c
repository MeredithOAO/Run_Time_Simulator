#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define NUMBER_TASK 4
#define NUMBER_PROCESSORS  2

// 任务结构体定义
typedef struct {
    int id;
    int priority;
    int period;       // 周期 T
    int execution_time;         // 最坏执行时间 C
    int remaining_time;
    int deadline;     // 截止时间 D（默认等于周期）
    int release_time; // 释放时间（默认0）
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
    { .id = 0, .priority = 1, .period = 4, .execution_time = 1, .remaining_time = 0, .deadline = 3, .release_time = 0 , .next_release_time = 0 },
    { .id = 1, .priority = 2, .period = 8, .execution_time = 5, .remaining_time = 0, .deadline = 6, .release_time = 0 , .next_release_time = 0 },
    { .id = 2, .priority = 3, .period = 16, .execution_time = 7, .remaining_time = 0, .deadline = 9, .release_time = 0 , .next_release_time = 0 },
    { .id = 3, .priority = 4, .period = 32,  .execution_time = 17, .remaining_time = 0, .deadline = 31, .release_time = 0 , .next_release_time = 0 }
};

Task Ready_Queue[NUMBER_TASK];

/** generate_task_set
 * 生成符合UUniFast算法的任务集合
 * @param n 任务数量
 * @param U 总利用率
 * @param min_period 最小周期
 * @param max_period 最大周期
 * @param max_retries 最大重试次数
 * @return 生成的任务数组指针，失败返回NULL
 *     // 生成任务集
    // Task* tasks = generate_task_set(n, U, min_period, max_period, max_retries);
    // if (!tasks) {
    //     printf("生成失败，请检查输入参数！\n");
    //     return 1;
    // }

    // 打印结果
    // print_tasks(tasks, n);

    // free(tasks); // 释放内存
 
// // Task* generate_task_set(int n, double U, 
// //                        double min_period, double max_period,
// //                        int max_retries) {
//     // 输入参数校验
//     if (n <= 0 || U < 0 || U > n || 
//         min_period <= 0 || max_period <= min_period) {
//         return NULL;
//     }
//     Task* tasks = (Task*)malloc(n * sizeof(Task));
//     if (!tasks) return NULL;
//     int valid;
//     for (int retry = 0; retry < max_retries; retry++) {
//         valid = 1;
//         double remaining_U = U;
//         // 生成前n-1个任务
//         for (int i = 0; i < n-1; i++) {
//             // 生成随机数s ∈ [0,1)
//             double s = (double)rand() / RAND_MAX;
//
//             // 计算利用率 u_i = U_remaining * (1 - s^(1/(n-i)))
//             double exponent = 1.0 / (n - i - 1);
//             double u_i = remaining_U * (1 - pow(s, exponent));
//
//             // 合法性检查
//             if (u_i < 0 || u_i > 1.0) {
//                 valid = 0;
//                 break;
//             }
//
//             // 生成周期 T_i ∈ [min_period, max_period]
//             double T_i = min_period + 
//                         ((double)rand() / RAND_MAX) * (max_period - min_period);
//          
//             // 计算最坏执行时间 C_i = u_i * T_i
//             double C_i = u_i * T_i;
//
//             // 写入任务结构体
//             tasks[i].period = T_i;
//             tasks[i].wcet = C_i;
//             tasks[i].deadline = T_i;    // 截止时间等于周期
//             tasks[i].release_time = 0.0; // 释放时间默认为0
//
//             remaining_U -= u_i;
//         }
//
//         // 处理最后一个任务
//         if (valid) {
//             double u_last = remaining_U;
//             if (u_last < 0 || u_last > 1.0) {
//                 valid = 0;
//             } else {
//                 // 生成最后一个任务的参数
//                 double T_last = min_period + 
//                                ((double)rand() / RAND_MAX) * (max_period - min_period);
//                 double C_last = u_last * T_last;
//
//                 tasks[n-1].period = T_last;
//                 tasks[n-1].wcet = C_last;
//                 tasks[n-1].deadline = T_last;
//                 tasks[n-1].release_time = 0.0;
//             }
//         }
//
//         if (valid) {
//             return tasks; // 生成成功
//         }
//     }
//
//     free(tasks); // 超过最大重试次数
//     return NULL;
// }

// 打印任务集详细信息
void print_tasks(Task* tasks, int n) {
    double total_utilization = 0;
    printf("ID\tPeriod\tWCET\tDeadline\tUtilization\n");
    for (int i = 0; i < n; i++) {
        double utilization = tasks[i].wcet / tasks[i].period;
        total_utilization += utilization;
        printf("%d\t%d\t%d\t%d\t\t%.4f\n", 
              i+1, 
              tasks[i].period,
              tasks[i].wcet,
              tasks[i].deadline,
              utilization);
    }
    printf("Total Utilization: %.4f / %.4f\n", total_utilization, total_utilization);
}
*/
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

// void Set_task(Task* tasks){
//     tasks[0].id = 1;
//     tasks[0].priority = 1;
//     tasks[0].period = 4;
//     tasks[0].wcet = 1;
//     tasks[0].deadline = 3;
//     tasks[0].release_time = 0;
//     tasks[1].id = 2;
//     tasks[1].priority = 2;
//     tasks[1].period = 8;
//     tasks[1].wcet = 5;
//     tasks[1].deadline = 6;
//     tasks[1].release_time = 0;
//     tasks[2].id = 3;
//     tasks[2].priority = 3;
//     tasks[2].period = 16;
//     tasks[2].wcet = 7;
//     tasks[2].deadline = 9;
//     tasks[2].release_time = 0;
//     tasks[3].id = 3;
//     tasks[3].priority = 4;
//     tasks[3].period = 32;
//     tasks[3].wcet = 17;
//     tasks[3].deadline = 31;
//     tasks[3].release_time = 0;
// }

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

        // 1) 检查是否有任务在时刻 t 激活 (next_release_time == t)
        for(int i = 0; i < NUMBER_TASK; i++){
            // 如果当前时刻到达某任务的释放时刻，就绪并重置其剩余执行时间
            if(Global_Tasks[i].next_release_time == current_time) {
                Global_Tasks[i].remaining_time = Global_Tasks[i].execution_time;
            }
        }

}

int compare_task_priority(const void* a, const void* b) {
    Task* ta = (Task*)a;
    Task* tb = (Task*)b;
    // 数值越小优先级越高，所以按 priority 升序排序
    return (ta->priority - tb->priority);
}

int Add_Task_to_RQ(Task* Global_Tasks){

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


    printf("Time %d: \n", current_time);
        // 对被选中的任务执行 1 个时间单位
            for(int i = 0; i < Num_Task_need_run; i++){
                
                int running_id = Ready_Queue[i].id;

                // 在原始任务数组中找到对应任务，剩余执行时间减 1
                Global_Tasks[running_id].remaining_time--;

                // 如果任务执行完成，则设置它的下一次释放时刻
                if(Global_Tasks[running_id].remaining_time == 0) {
                    Global_Tasks[running_id].next_release_time = current_time + Global_Tasks[running_id].period;
                }

                printf("Running tasks id %d \n", Global_Tasks[running_id].id);

            }

}


void main() {

    srand(time(NULL)); // random seed

    int current_time = 0;   //   Time 

    // Set_task(tasks);

    int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);

    for(current_time = 0; current_time < Simulation_time; current_time++) {

        check_realse(current_time);

        int Ready_Task_Count = Add_Task_to_RQ(Global_Tasks);

        qsort(Ready_Queue, Ready_Task_Count, sizeof(Task), compare_task_priority);

        int idle_processor_count = check_processor_idle_count(processor);
        

        if (Ready_Task_Count < NUMBER_PROCESSORS)// check RQ
        {
            //run all task in RQ
            Run_Task_one_step(Global_Tasks, Ready_Queue, current_time, Ready_Task_Count);
        }

        if (Ready_Task_Count > NUMBER_PROCESSORS)
        {
            Run_Task_one_step(Global_Tasks, Ready_Queue, current_time, NUMBER_PROCESSORS);
        }
        
        // printf("Time %d: Running tasks [ ", current_time);
        // for(int i = 0; i < tasks_to_run; i++){
        //     printf("%d ", ready_tasks[i].task_id);
        // }
        // printf("]\n");

    }

    

    

    

    

    // int tasks_to_run = (ready_count < NUM_PROCESSORS) ? ready_count : NUM_PROCESSORS;

    // for (int i = 0; i < NUMBER_TASK; i++)
    // {
    //     printf("Ready Queue %d is Task: %d\n", i , Ready_Queue[i].id);
    // }
    
    // printf("LCM of Tasks' period: %d\n", Current_LCM);

    // printf("LCM of Tasks' period: %d\n", Current_LCM);


    /** 

    // while (t < Current_LCM)
    // {

    add task to ready queue

    //sort it based on priority
    qsort(Ready_Queue, Ready_Task_Count, sizeof(Task), compare_task_priority);


    if processor is idle{
    pick one from rq
    }
        
    // }
        
    
    */
 




    // return 0;
}