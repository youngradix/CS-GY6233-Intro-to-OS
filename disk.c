#include <stdio.h>
#include "oslabs.h"

//The NULLRCB is defined as [RID:0, AT:0, CYL:0, ADDR:0, PID:0]
/*struct RCB {
        int request_id;
        int arrival_timestamp;
        int cylinder;
        int address;
        int process_id;
    }*/

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
/*The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being a NULLRCB), otherwise, 
it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue  */
}
/*A sample execution input and output:
input/output    parameter           value
input           request_queue       EMPTY
input           queue_cnt           0
input           current_request     [RID:51, AT:1, CYL:53, ADDR:53, PID:51]
input           new_request         [RID:52, AT:2, CYL:54, ADDR:54, PID:52]
input           timestamp           2
output          request_queue       [RID:52, AT:2, CYL:54, ADDR:54, PID:52]
output          queue_cnt           1
output          RCB                 [RID:51, AT:1, CYL:53, ADDR:53, PID:51]*/

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
/*If the request queue is empty, the method returns a NULLRCB, indicating that there is no request to service next. Otherwise, the method 
finds the RCB in the request queue that has the earliest arrival time. It then removes this RCB from the request queue and returns it. */
}
/*A sample execution input and output:
input/output    parameter       value
input           request_queue   [RID:1, AT:10, CYL:124323, ADDR:124323, PID:1]
input           queue_cnt       1
output          request_queue   EMPTY
output          queue_cnt       0
output          RCB             [RID:1, AT:10, CYL:124323, ADDR:124323, PID:1]*/

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
/*The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being NULLRCB), otherwise, 
it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue*/
}
/* A sample execution input and output:
input/output    parameter           value
input           request_queue       EMPTY
input           queue_cnt           0
input           current_request     [RID:51, AT:1, CYL:53, ADDR:53, PID:51]
input           new_request         [RID:52, AT:2, CYL:54, ADDR:54, PID:52]
input           timestamp           2
output          request_queue       [RID:52, AT:2, CYL:54, ADDR:54, PID:52]
output          queue_cnt           1
output          RCB                 [RID:51, AT:1, CYL:53, ADDR:53, PID:51]*/

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
    /*If the request queue is empty, the method returns NULLRCB, indicating that there is no request to service next. Otherwise, 
    the method finds the RCB in the request queue whose cylinder is closest to the current cylinder. If there are multiple requests 
    with the closest cylinder, then the method picks the request among these that has the earliest arrival_timestamp. The method then 
    removes the RCB of the selected request from the request queue and returns it.*/
}
/*A sample execution input and output:
input/output    parameter           value
input           request_queue       [RID:1, AT:72, CYL:45, ADDR:45, PID:1], 
                                    [RID:2, AT:71, CYL:47, ADDR:47, PID:2], 
                                    [RID:3, AT:73, CYL:43, ADDR:43, PID:3]
input           queue_cnt           3
input           current_cylinder    48
output          request_queue       [RID:1, AT:72, CYL:45, ADDR:45, PID:1], 
                                    [RID:3, AT:73, CYL:43, ADDR:43, PID:3]
output          queue_cnt           2
output          RCB                 [RID:2, AT:71, CYL:47, ADDR:47, PID:2]*/

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    /*The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being NULLRCB), 
    otherwise, it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue. */
}
/*A sample execution input and output:
input/output    parameter       value
input           request_queue   EMPTY
input           queue_cnt       0
input           current_request [RID:51, AT:1, CYL:53, ADDR:53, PID:51]
input           new_request     [RID:52, AT:2, CYL:54, ADDR:54, PID:52]
input           timestamp       2
output          request_queue   [RID:52, AT:2, CYL:54, ADDR:54, PID:52]
output          queue_cnt       1
output          RCB             [RID:51, AT:1, CYL:53, ADDR:53, PID:51]*/

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction){
    /*If the request queue is empty, the method returns NULLRCB, indicating that there is no request to service next. Otherwise, it 
    picks the next request to service from the request queue. 

    If there are requests in the queue with the same cylinder as the current cylinder, the method picks the one among these requests 
    with the earliest arrival time. Otherwise, if the scan direction is 1 and there are requests with cylinders larger than the current 
    cylinder, the method picks the one among these whose cylinder is closest to the current cylinder. Otherwise, if the scan direction 
    is 1 and there are no requests with cylinders larger than the current cylinder, the method picks the request whose cylinder is closest 
    to the current cylinder. Otherwise, if the scan direction is 0 and there are requests with cylinders smaller than the current cylinder, 
    the method picks the one among these whose cylinder is closest to the current cylinder. Otherwise, if the scan direction is 0 and there 
    are requests with cylinders larger than the current cylinder, the method picks the request whose cylinder is closest to the current cylinder.  
    
    After picking the RCB from the request queue, as described above, the method removes the RCB from the queue and returns it.*/
}
/*A sample execution input and output:
input/output    parameter           value
input           request_queue       [RID:1, AT:52, CYL:58, ADDR:58, PID:1], 
                                    [RID:2, AT:51, CYL:58, ADDR:58, PID:2], 
                                    [RID:3, AT:53, CYL:58, ADDR:58, PID:3]
input           queue_cnt           3
input           current_cylinder    58
input           scan_direction      1
output          request_queue       [RID:1, AT:52, CYL:58, ADDR:58, PID:1], 
                                    [RID:3, AT:53, CYL:58, ADDR:58, PID:3]
output          queue_cnt           2
output          RCB                 [RID:2, AT:51, CYL:58, ADDR:58, PID:2]*/