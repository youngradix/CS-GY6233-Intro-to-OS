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
    }
*/
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp);{
    /* If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the PCB of the 
    newly-arriving process, indicating that it is the process to execute next. In this case, the PCB of the new process is modified 
    so that the execution start time is set to the current timestamp, the execution end time is set to the sum of the current timestamp 
    and the total burst time and the remaining burst time is set to the total burst time. */
    if((current_process.process_id == 0) && (current_process.arrival_timestamp == 0) && (current_process.total_bursttime == 0) 
    && (current_process.execution_starttime == 0) && (current_process.execution_endtime == 0)&& (current_process.remaining_bursttime == 0) 
    && (current_process.process_priority == 0){
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp = new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    /*If there is a currently-running process, the method compares the priority of the newly-arriving process with the currently-running
    process. If the new process has equal or lower priority (smaller integers for the priority field in the PCB indicate higher priority),
    then its PCB is simply added to the ready queue and the return value is the PCB of the currently-running process. As the newly-arriving
    process is added to the ready queue, its execution start time and execution end time are set to 0, and the remaining burst time is the 
    same as its total burst time.*/
    if (new_process.process_priority >= current_process.process.priority){
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

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);{

    }

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt,struct PCB current_process, struct PCB new_process, int time_stamp);{
    if((current_process.process_id == 0) && (current_process.arrival_timestamp == 0) && (current_process.total_bursttime == 0) 
    && (current_process.execution_starttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) 
    && (current_process.process_priority == 0){

    
    }
    if

}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);{


}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum);{
    if((current_process.process_id == 0) && (current_process.arrival_timestamp == 0)  && (current_process.total_bursttime == 0)  
    && (current_process.execution_starttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) 
    && (current_process.process_priority == 0){

    
    
    
    }



}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum);{



}