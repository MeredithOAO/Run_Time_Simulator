#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_TASKS       4           // 任务数
#define NUM_PROCESSORS  2           // 处理器(或CPU核)个数
#define SIMULATION_TIME 50          // 模拟时间上限

// 任务结构体
typedef struct {
    int task_id;            // 任务ID
    int priority;           // 任务优先级(数值越小优先级越高)
    int period;             // 任务周期
    int execution_time;     // 任务每次激活需要执行的时间
    int remaining_time;     // 当前作业剩余执行时间
    int next_release_time;  // 下次激活的时刻
} Task;

// 全局任务数组（周期性任务）
Task g_tasks[NUM_TASKS] = {
    { .task_id = 0, .priority = 1, .period = 10, .execution_time = 3, .remaining_time = 0, .next_release_time = 0 },
    { .task_id = 1, .priority = 2, .period = 10, .execution_time = 2, .remaining_time = 0, .next_release_time = 0 },
    { .task_id = 2, .priority = 3, .period = 20, .execution_time = 4, .remaining_time = 0, .next_release_time = 0 },
    { .task_id = 3, .priority = 4, .period = 5,  .execution_time = 1, .remaining_time = 0, .next_release_time = 0 }
};

// 用于根据优先级对任务进行排序(从高到低优先级)
int compare_task_priority(const void* a, const void* b) {
    Task* ta = (Task*)a;
    Task* tb = (Task*)b;
    // 数值越小优先级越高，所以按 priority 升序排序
    return (ta->priority - tb->priority);
}

int main(void) {
    printf("Global Fixed Priority Scheduling Simulation\n");
    printf("Number of tasks: %d, Number of processors: %d\n", NUM_TASKS, NUM_PROCESSORS);
    printf("Simulating up to time %d...\n\n", SIMULATION_TIME);

    // 打印初始任务信息
    printf("Initial task set:\n");
    for(int i = 0; i < NUM_TASKS; i++){
        printf("Task %d: priority=%d, period=%d, execution_time=%d\n",
               g_tasks[i].task_id, g_tasks[i].priority, g_tasks[i].period, g_tasks[i].execution_time);
    }
    printf("\n");

    // 模拟时间从 0 到 SIMULATION_TIME-1
    for(int t = 0; t < SIMULATION_TIME; t++) {
        
        // 1) 检查是否有任务在时刻 t 激活 (next_release_time == t)
        for(int i = 0; i < NUM_TASKS; i++){
            // 如果当前时刻到达某任务的释放时刻，就绪并重置其剩余执行时间
            if(g_tasks[i].next_release_time == t) {
                g_tasks[i].remaining_time = g_tasks[i].execution_time;
            }
        }

        // 2) 将就绪(remaining_time > 0)的任务按照优先级排序
        //    先复制到一个临时数组进行排序
        Task ready_tasks[NUM_TASKS];
        int ready_count = 0;
        for(int i = 0; i < NUM_TASKS; i++){
            if(g_tasks[i].remaining_time > 0) {
                ready_tasks[ready_count++] = g_tasks[i];
            }
        }

        // 根据优先级从高到低（priority 值小到大）进行排序
        qsort(ready_tasks, ready_count, sizeof(Task), compare_task_priority);

        // 3) 从优先级最高的就绪任务开始，最多选择 NUM_PROCESSORS 个进行执行
        int tasks_to_run = (ready_count < NUM_PROCESSORS) ? ready_count : NUM_PROCESSORS;

        // 4) 对被选中的任务执行 1 个时间单位
        for(int i = 0; i < tasks_to_run; i++){
            int running_id = ready_tasks[i].task_id;
            // 在原始任务数组中找到对应任务，剩余执行时间减 1
            g_tasks[running_id].remaining_time--;
            // 如果任务执行完成，则设置它的下一次释放时刻
            if(g_tasks[running_id].remaining_time == 0) {
                g_tasks[running_id].next_release_time = t + g_tasks[running_id].period;
            }
        }

        // 5) 可在此打印或记录调度决策，这里简单打印当前时刻的调度情况
        // 打印正在执行的任务
        printf("Time %2d: Running tasks [ ", t);
        for(int i = 0; i < tasks_to_run; i++){
            printf("%d ", ready_tasks[i].task_id);
        }
        printf("]\n");
    }

    // 模拟结束，打印结果
    printf("\nSimulation finished.\n");
    // 可以查看每个任务最后一次的完成情况等信息
    for(int i = 0; i < NUM_TASKS; i++) {
        printf("Task %d => next_release_time = %d, remaining_time = %d\n",
               g_tasks[i].task_id,
               g_tasks[i].next_release_time,
               g_tasks[i].remaining_time);
    }

    return 0;
}
