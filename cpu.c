#include <stdio.h>
#include "oslabs.h"

//The NULLPCB is defined as [PID:0, AT:0, TBT:0, EST:0, EET:0, RBT:0, Priority:0]
/* Format for the structure
struct PCB {
        int process_id;
        int arrival_timestamp;
        int total_bursttime;
        int execution_starttime;
        int execution_endtime;
        int remaining_bursttime;
        int process_priority;
    }*/

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
    /* If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the 
    newly-arriving process, indicating that it is the process to execute next. In this case, the PCB of the new process is modified 
    so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp 
    and the total burst time and the remaining burst time is set to the total burst time. */
    if((current_process.process_id == 0) && (current_process.arrival_timestamp == 0) && (current_process.total_bursttime == 0) 
    && (current_process.execution_starttime == 0) && (current_process.execution_endtime == 0)&& (current_process.remaining_bursttime == 0) 
    && (current_process.process_priority == 0)){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    /*If there is a currently-running process, the method compares the priority of the newly-arriving process with the currently-running
    process. If the new process has equal or lower priority (smaller integers for the priority field in the PCB indicate higher priority),
    then its PCB is simply added to the ready queue and the return value is the PCB of the currently-running process. As the newly-arriving
    process is added to the ready queue, its execution start time and execution end time are set to 0, and the remaining burst time is the 
    same as its total burst time.*/
    else if(new_process.process_priority >= current_process.process_priority){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;
        return current_process;
    }
    /*If the new process has a higher priority, then the PCB of the currently-running process is added to the ready queue and the return 
    value is the PCB of the new process. In this case, the PCB of the new process is modified so that the execution start time is set to 
    the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining 
    burst time is set to the total burst time. Also, the PCB of the currently-running process is added to the ready queue after marking 
    its execution end time as 0, and adjusting its remaining burst time.    */
    else{//higher means a lower number so <=
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt = *queue_cnt +1;
        return new_process;
    } 
}
/*A sample execution input and output:
input/output   parameter        value
input          ready_queue      EMPTY
input          queue_cnt        0
input          current_process  [PID:1, AT:1, TBT:4, EST:1, EET:5, RBT:4, Priority:8]
input          new_process      [PID:2, AT:2, TBT:3, EST:0, EET:0, RBT:3, Priority:6]
input          time_stamp       2
output         ready_queue      [PID:1, AT:1, TBT:4, EST:1, EET:0, RBT:3, Priority:8]
output         queue_cnt        1
output          PCB             [PID:2, AT:2, TBT:3, EST:2, EET:5, RBT:3, Priority:6]*/
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    /*If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute. Otherwise, the method finds 
    the PCB of the process in the ready queue with the highest priority (smaller integers for the priority field in the PCB mean higher priorities), 
    removes this PCB from the ready queue and returns it. Before returning the PCB of the next process to execute, it is modified to set the execution 
    start time as the current timestamp and the execution end time as the sum of the current timestamp and the remaining burst time. */
    if(*queue_cnt > 0){
        struct PCB next_process;
        int priority_scan_index = 0;
        int highest_priority = ready_queue[0].process_priority;
        for(int i = 1; i < *queue_cnt; i++){//Search for highest priority before queue_cnt update
            if(highest_priority > ready_queue[i].process_priority){
                highest_priority = ready_queue[i].process_priority;
                priority_scan_index = i;
                }
        }
        next_process = ready_queue[priority_scan_index];
        for(int i = priority_scan_index; i < *queue_cnt - 1; i++){
            ready_queue[i] = ready_queue[i + 1];
        }
        *queue_cnt = *queue_cnt - 1;
        next_process.execution_starttime = timestamp;
        next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
        return next_process;
    }
    else{
        struct PCB NULLPCB = { 0, 0, 0, 0, 0, 0, 0};
        return NULLPCB;
    }
}
/*input/output    parameter       value
  input           ready_queue     [PID:1, AT:1, TBT:4, EST:0, EET:0, RBT:4, Priority:23], 
                                  [PID:2, AT:1, TBT:4, EST:0, EET:0, RBT:4, Priority:22], 
                                  [PID:3, AT:1, TBT:4, EST:0, EET:0, RBT:4, Priority:24]
  input           queue_cnt       3
  input           time_stamp      2
  output          ready_queue     [PID:1, AT:1, TBT:4, EST:0, EET:0, RBT:4, Priority:23], 
                                  [PID:3, AT:1, TBT:4, EST:0, EET:0, RBT:4, Priority:24]
  output          queue_cnt       2
 output          PCB             [PID:2, AT:1, TBT:4, EST:2, EET:6, RBT:4, Priority:22]*/

//2 SRTP
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt,struct PCB current_process, struct PCB new_process, int time_stamp){
    /*If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process,
    indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to 
    the current timestamp, the execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is 
    set to the total burst time.*/
    if((current_process.process_id == 0) && (current_process.arrival_timestamp == 0) && (current_process.total_bursttime == 0) 
    && (current_process.execution_starttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) 
    && (current_process.process_priority == 0)){
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    /*If there is a currently-running process, the method compares the remaining burst time of the currently-running process and the total burst time 
    of the newly-arriving process. If the new process does not have a shorter burst time, then its PCB is simply added to the ready queue and the return 
    value is the PCB of the currently running process. As the newly-arriving process is added to the ready queue, its execution start time and execution 
    end time are set to 0, and the remaining burst time is set to the total burst time. */
    else if(current_process.remaining_bursttime <= new_process.total_bursttime){
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;
        return current_process;
    }
    /*If the new process has a shorter burst time, then the PCB of the currently-running process is added to the ready queue and the return value is the 
    PCB of the new process. In this case, the PCB of the new process is modified so that the execution start time is set to the current timestamp, the 
    execution end time is set to the sum of the current timestamp and the total burst time and the remaining burst time is set to the total burst time. 
    Also, the PCB of the currently-running process is added to the ready queue, after marking its execution start time and execution end time as 0, and 
    adjusting its remaining burst time. */
    else{//has shorter burst time
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt = *queue_cnt + 1;
        return new_process;
    }
}
/*A sample execution input and output:
input/output        parameter           value
input               ready_queue         EMPTY
input               queue_cnt           0
input               current_process     [PID:1, AT:1, TBT:8, EST:1, EET:9, RBT:8, Priority:0]
input               new_process         [PID:2, AT:2, TBT:6, EST:0, EET:0, RBT:6, Priority:0]
input               time_stamp          2
output              ready_queue         [PID:1, AT:1, TBT:8, EST:0, EET:0, RBT:7, Priority:0]
output              queue_cnt           1
output              PCB                 [PID:2, AT:2, TBT:6, EST:2, EET:8, RBT:6, Priority:0]*/

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp){
    /*If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next. Otherwise, the method finds the 
    PCB of the process in the ready queue with the smallest remaining burst time, removes this PCB from the ready queue and returns it. Before returning the 
    PCB of the next process to execute, it is modified to set the execution start time as the current timestamp and the execution end time as the sum of the 
    current timestamp and the remaining burst time.*/ 
    if(*queue_cnt > 0){
        struct PCB next_process;
        int smallest_rbt_index = 0;
        int smallest_rbt = ready_queue[0].remaining_bursttime;
        for(int i = 1; i < *queue_cnt; i++){
            if(smallest_rbt > ready_queue[i].remaining_bursttime){
                smallest_rbt = ready_queue[1].remaining_bursttime;
                smallest_rbt_index = i;
            }
        }
        next_process = ready_queue[smallest_rbt_index];
        for(int i = smallest_rbt_index; i < *queue_cnt - 1; i++){
            ready_queue[i] = ready_queue[i + 1];
        }
        *queue_cnt = *queue_cnt - 1;
        next_process.execution_starttime = timestamp;
        next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
        return next_process;
    }
    else{
        struct PCB NULLPCB = { 0, 0, 0, 0, 0, 0, 0};
        return NULLPCB;
    }
}
/*A sample execution input and output:
input/output    parameter       value
input           ready_queue     [PID:1, AT:1, TBT:23, EST:0, EET:0, RBT:23, Priority:0], 
                                [PID:2, AT:1, TBT:22, EST:0, EET:0, RBT:22, Priority:0], 
                                [PID:3, AT:1, TBT:24, EST:0, EET:0, RBT:24, Priority:0]
input           queue_cnt       3
input           time_stamp      2
output          ready_queue     [PID:1, AT:1, TBT:23, EST:0, EET:0, RBT:23, Priority:0], 
                                [PID:3, AT:1, TBT:24, EST:0, EET:0, RBT:24, Priority:0]

output          queue_cnt       2
output          PCB             [PID:2, AT:1, TBT:22, EST:2, EET:24, RBT:22, Priority:0]*/

//3 RR
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum){
    /*If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the newly-arriving process,
    indicating that it is the process to execute next. In this case, the PCB of the new process is modified so that the execution start time is set to 
    the current timestamp, the execution end time is set to the sum of the current timestamp and the smaller of the time quantum and the total burst time. 
    The remaining burst time is set to the total burst time. */
    if((current_process.process_id == 0) && (current_process.arrival_timestamp == 0)  && (current_process.total_bursttime == 0)  
    && (current_process.execution_starttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) 
    && (current_process.process_priority == 0)){
        new_process.execution_starttime = timestamp;
        if(time_quantum <= new_process.total_bursttime){
            new_process.execution_endtime = timestamp + time_quantum;
        }
        else{
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
        }
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    /*If there is a currently-running process, the method simply adds the PCB of the newly-arriving process to the ready queue and the return value is the 
    PCB of the currently running process. As the newly-arriving process is added to the ready queue, its execution start time and execution end time are 
    set to 0, and the remaining burst time is set to the total burst time.*/
    else{
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt = *queue_cnt + 1;
        return current_process;
    }
}
/*
input/output    parameter           value
input           ready_queue         EMPTY
input           queue_cnt           0
input           current_process     [PID:1,AT:1,BT:8,EST:1,EET:9,RBT:8,Priority:0 ]
input           new_process         [PID:2, AT:2, TBT:8, EST:0, EET:0, RBT:8, Priority:0]
input           time_stamp          2
input           time_quantum        6
output          ready_queue         [PID:2,AT:2,TBT:8,EST:0,EET:0,RBT:8,Priority:0 ]
output          queue_cnt           1
output          PCB                 [PID:1,AT:1,TBT:8,EST:1,EET:9,RBT:8,Priority:0 ]*/

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum){
    /*If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next. Otherwise, the method finds the 
    PCB of the process in the ready queue with the earliest arrival time, removes this PCB from the ready queue and returns it. Before returning this 
    PCB, it is modified to set the execution start time as the current timestamp and the execution end time as the sum of the current timestamp and the 
    smaller of the time quantum and the remaining burst time. */    
    if(*queue_cnt > 0){
        struct PCB next_process;
        int earliest_at_index = 0;
        int earliest_at = ready_queue[0].arrival_timestamp;
        for(int i = 1; i < *queue_cnt; i++){
            if(earliest_at > ready_queue[i].arrival_timestamp){
                earliest_at = ready_queue[1].arrival_timestamp;
                earliest_at_index = i;
            }
        }
        next_process = ready_queue[earliest_at_index];
        for(int i = earliest_at_index; i < *queue_cnt - 1; i++){
            ready_queue[i] = ready_queue[i + 1];
        }
        *queue_cnt = *queue_cnt - 1;
        next_process.execution_starttime = timestamp;
        next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
        return next_process;
    }
    else{
        struct PCB NULLPCB = { 0, 0, 0, 0, 0, 0, 0};
        return NULLPCB;
    }
}
/*
input/output    parameter       value
input           ready_queue     [PID:1, AT:22, TBT:8, EST:0, EET:0, RBT:8, Priority:0], 
                                [PID:2, AT:21, TBT:8, EST:0, EET:0, RBT:8, Priority:0], 
                                [PID:3, AT:23, TBT:8, EST:0, EET:0, RBT:8, Priority:0]
input           queue_cnt       3
input           time_stamp      24
input           time_quantum    10
output          ready_queue     [PID:1, AT:22, TBT:8, EST:0, EET:0, RBT:8, Priority:0], 
                                [PID:3, AT:23, TBT:8, EST:0, EET:0, RBT:8, Priority:0]
output          queue_cnt       2
output          PCB             [PID:2, AT:21, TBT:8, EST:24, EET:32, RBT:8, Priority:0]*/