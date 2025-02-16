#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 宏定义任务数和总利用率（允许总利用率大于 1）
#define TASK_COUNT         10
#define TOTAL_UTILIZATION  1.5

// 定义周期的取值范围（例如：50 到 1000 毫秒）
#define MIN_PERIOD 50.0
#define MAX_PERIOD 1000.0

// 任务结构体，包含周期、释放时间、截止日期和最坏执行时间
typedef struct {
    double period;   // 周期
    double release;  // 释放时间
    double deadline; // 截止日期
    double wcet;     // 最坏执行时间
} Task;

/*
 * 利用 UUnifast 算法生成任务利用率，并随机生成任务参数
 *
 * 参数:
 *   tasks   - 任务数组
 *   n       - 任务数
 *   U_total - 总利用率（可大于 1）
 *
 * 对于每个任务：
 *   随机生成一个周期 period ∈ [MIN_PERIOD, MAX_PERIOD]
 *   计算 wcet = u × period，其中 u 为 UUnifast 算法生成的利用率
 *   释放时间设为 0，截止日期设为周期（隐式截止）
 */
void generate_task_set(Task tasks[], int n, double U_total) {
    double sumU = U_total;
    double u;

    for (int i = 0; i < n - 1; i++) {
        // 生成 (0,1) 内的随机数
        double rand_val = ((double)rand() / (RAND_MAX + 1.0));
        // UUnifast 核心公式：u = sumU - nextSumU，其中 nextSumU = sumU * rand_val^(1/(n-i))
        double nextSumU = sumU * pow(rand_val, 1.0 / (n - i));
        u = sumU - nextSumU;
        sumU = nextSumU;

        // 随机生成周期
        double period = MIN_PERIOD + ((double)rand() / RAND_MAX) * (MAX_PERIOD - MIN_PERIOD);
        tasks[i].period = period;
        tasks[i].release = 0.0;       // 释放时间设为 0
        tasks[i].deadline = period;     // 隐式截止：截止日期等于周期
        tasks[i].wcet = u * period;     // wcet = 利用率 × 周期
    }

    // 为最后一个任务分配剩余利用率
    double period = MIN_PERIOD + ((double)rand() / RAND_MAX) * (MAX_PERIOD - MIN_PERIOD);
    tasks[n - 1].period = period;
    tasks[n - 1].release = 0.0;
    tasks[n - 1].deadline = period;
    tasks[n - 1].wcet = sumU * period;
}

int main() {
    // 使用宏定义的任务数
    Task tasks[TASK_COUNT];

    // 初始化随机数种子
    srand((unsigned)time(NULL));

    // 生成任务集
    generate_task_set(tasks, TASK_COUNT, TOTAL_UTILIZATION);

    // 输出生成的任务参数
    printf("Task Set:\n");
    printf("Task Number\tPeriod\t\tRelease Time\tDDL\tWCET\n");
    for (int i = 0; i < TASK_COUNT; i++) {
        printf("%d\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", 
               i + 1, tasks[i].period, tasks[i].release, tasks[i].deadline, tasks[i].wcet);
    }
    getchar();
    return 0;
}
