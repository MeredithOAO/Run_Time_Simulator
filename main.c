#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
 * 使用 UUnifast 算法生成任务利用率
 * 参数:
 *   tasks - 任务数组
 *   n     - 任务数
 *   U_total - 总利用率（0~1）
 *
 * 对于每个任务：
 *   随机生成一个周期 period ∈ [MIN_PERIOD, MAX_PERIOD]
 *   计算 wcet = u * period，其中 u 为由 UUnifast 算法生成的利用率
 *   释放时间设为 0，截止日期设为周期（隐式截止）
 */
void generate_task_set(Task tasks[], int n, double U_total) {
    double sumU = U_total;
    double u;

    for (int i = 0; i < n - 1; i++) {
        // 生成 (0,1) 内的随机数
        double rand_val = ((double)rand() / (RAND_MAX + 1.0));
        // UUnifast 核心公式：u = sumU - nextSumU，其中 nextSumU = sumU * (rand_val)^(1/(n-i))
        double nextSumU = sumU * pow(rand_val, 1.0 / (n - i));
        u = sumU - nextSumU;
        sumU = nextSumU;

        // 随机生成周期
        double period = MIN_PERIOD + ((double)rand() / RAND_MAX) * (MAX_PERIOD - MIN_PERIOD);
        tasks[i].period = period;
        tasks[i].release = 0.0;       // 释放时间设为 0
        tasks[i].deadline = period;     // 隐式截止：截止时间等于周期
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
    int n;
    double U_total;

    // 提示用户输入任务数和总利用率
    printf("请输入任务数: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("任务数输入错误！\n");
        return -1;
    }

    printf("请输入总利用率 (0-1): ");
    if (scanf("%lf", &U_total) != 1 || U_total <= 0 || U_total > 1) {
        printf("总利用率输入错误！\n");
        return -1;
    }

    // 动态分配任务数组
    Task *tasks = (Task *)malloc(n * sizeof(Task));
    if (tasks == NULL) {
        printf("内存分配失败！\n");
        return -1;
    }

    // 初始化随机数种子
    srand((unsigned)time(NULL));

    // 生成任务集
    generate_task_set(tasks, n, U_total);

    // 输出生成的任务参数
    printf("\n生成的任务集:\n");
    printf("任务编号\t周期\t\t释放时间\t截止时间\tWCET\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%.2f\t\t%.2f\t\t%.2f\t\t%.2f\n", 
               i + 1, tasks[i].period, tasks[i].release, tasks[i].deadline, tasks[i].wcet);
    }

    // 释放动态分配的内存
    free(tasks);
    return 0;
}
