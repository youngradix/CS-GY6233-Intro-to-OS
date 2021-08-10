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

    }

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);{

    }

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt,struct PCB current_process, struct PCB new_process, int time_stamp);{

    }

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp);{

    }

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum);{

    }

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int time_stamp, int time_quantum);{

    }