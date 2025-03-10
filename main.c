#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "main.h"
#include "Simulator_1.h" 
#include "Task_generate.h"
#include "DA_LC_Test.h"
#include "DA_LC_OPA.h"
#include "G_MP.h"


Processor processor[NUMBER_PROCESSORS];

Task Global_Tasks[NUMBER_TASK];

int main() {

    srand(time(NULL)); // random seed

    int current_time = 0;   //   Time 
    int missDeadline_flag = 0;
    int DA_LC_flag = 1;
    int DA_LC_OPA_flag = 1;
    
if (Self_Test)
{   
    // generate_task_set(Global_Tasks);
    // while (Calculate_LCM(Global_Tasks,NUMBER_TASK) > 2000000 || Calculate_LCM(Global_Tasks,NUMBER_TASK) < 0 || Calculate_Real_U() > (float)NUMBER_PROCESSORS)
    // {
    //     generate_task_set(Global_Tasks);
    // }
    // Print_Task_Set();
    // // print_result_of_DA();
    // // print_result_of_DA_LC();
    // OPA_Assign_Priority();

    // Reset_Gloabl_Task();
    // int Simulation_time_test = Calculate_LCM(Global_Tasks,NUMBER_TASK);
    // // Set_PPP();
    // printf("\nPriority Promotion Time Set As Fllow: \n\n");
    // Print_Task_Set();
    // printf("\nStart Running G_DP Scheduling: \n\n");
    // G_DP_Scheduling(Simulation_time_test);
    int run_times=0;
    int meet_condition_1_times=0;
    int meet_condition_2_times=0;
    int meet_condition_3_times=0;
while (run_times<100)
{
    generate_task_set(Global_Tasks);
            while (Calculate_LCM(Global_Tasks,NUMBER_TASK) > 2000000 || Calculate_LCM(Global_Tasks,NUMBER_TASK) < 0 || Calculate_Real_U() > (float)NUMBER_PROCESSORS) //Calculate_LCM(Global_Tasks,NUMBER_TASK) > 2000000 ||
        {
            generate_task_set(Global_Tasks);
        }
        // Print_Task_Set();
    run_times++;
    DA_LC_flag = print_result_of_DA_LC();
    DA_LC_OPA_flag = OPA_Assign_Priority(DA_LC_flag);

    // if (DA_LC_flag == 1)
    // {
    //     meet_condition_1_times++;
    // }
    
    if (DA_LC_OPA_flag == 0)
    {
        meet_condition_2_times++;
        Set_PPP();
        G_DP_Scheduling(Calculate_LCM(Global_Tasks,NUMBER_TASK));
        
    }
    

    // if (DA_LC_flag == 0 &&  DA_LC_OPA_flag == 1) //DA_LC_flag == 0 &&  DA_LC_OPA_flag == 1
    // {
    //     meet_condition_3_times++;
    // }

    

}
// float DA_LC_OPA_pass_rate = (float)meet_condition_2_times/10.0;
// printf("DA-LC-DM Pass Times = %d\nDA-LC-OPA Pass Times = %d\nOnly DA-LC-OPA Pass Times = %d\n",meet_condition_1_times,meet_condition_2_times,meet_condition_3_times);
// printf("DA-LC-OPA Pass Rate = %f%c\n",DA_LC_OPA_pass_rate,'%');
printf("DA-LC-OPA not Pass Times = %d",meet_condition_2_times);
}

    if (Generate_test_on)
    {
       
    //generate_task_set 
    generate_task_set(Global_Tasks);

        // while (Calculate_LCM(Global_Tasks,NUMBER_TASK) > 2000000 || Calculate_LCM(Global_Tasks,NUMBER_TASK) < 0 || Calculate_Real_U() > (float)NUMBER_PROCESSORS)
        // {
        //     generate_task_set(Global_Tasks);
        // }
        

    Print_Task_Set();
    DA_LC_flag = print_result_of_DA_LC();
    DA_LC_OPA_flag = OPA_Assign_Priority(DA_LC_flag);

    if (DA_LC_OPA_flag)
    {
        printf("\nPass DA-LC-OPA Test. No need to run G-FP simulation \n");
    }else{
    
        printf("\nFail DA-LC_OPA Test. Need to run G-FP simulation \n");
    }
    

    
    if (Simulation_print)
{
    int Simulation_time = Calculate_LCM(Global_Tasks,NUMBER_TASK);

    printf("\nSimulation Start, Simulation time(LCM) = %d M = %d \n",Simulation_time,NUMBER_PROCESSORS);



    for(current_time = 0; current_time < Simulation_time; current_time++) {
        
        // printf("Time %d: \n", current_time);

        Task Ready_Queue[NUMBER_TASK];
        if (check_deadline(current_time))
        {
            missDeadline_flag = 1;
        }
        
        
        check_realse(current_time);
        

        int Ready_Task_Count = Add_Task_to_RQ(Global_Tasks, Ready_Queue);

        qsort(Ready_Queue, NUMBER_TASK, sizeof(Task), compare_task_priority);

        // int idle_processor_count = check_processor_idle_count(processor);
    

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



    if (!missDeadline_flag)
    {
        printf("\nSimulation Result: All Task Meet Deadline \n\n");
    }else {

        printf("\nThese Tasks Miss Deadline. Need to try G-DP Scheduling. \n\n");
        Reset_Gloabl_Task();
        Set_PPP();
        printf("\nPriority Promotion Time Set As Fllow: \n\n");
        Print_Task_Set();
        printf("\nStart Running G_DP Scheduling: \n\n");
        G_DP_Scheduling(Simulation_time);

    }
    
    
}

}

return 0;
}

//./main.exe | tee output.txt