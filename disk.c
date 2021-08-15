#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "oslabs.h"
//The NULLRCB is defined as [RID:0, AT:0, CYL:0, ADDR:0, PID:0]
/*struct RCB {
        int request_id;
        int arrival_timestamp;
        int cylinder;
        int address;
        int process_id;
    }*/

/* 1 FCFS */
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    /*The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being a NULLRCB), otherwise, 
    it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue  */
    if((current_request.request_id == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) //Disk free
        && (current_request.address == 0) && (current_request.process_id == 0)){
            return new_request;
    }
    else{
        if(*queue_cnt < QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt = *queue_cnt + 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
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
    if(*queue_cnt > 0){
        struct RCB next_RCB;
        int earliest_at_index = 0;
        int earliest_at = request_queue[0].arrival_timestamp;
        for(int i = 1; i < *queue_cnt; i++){
            if(earliest_at > request_queue[i].arrival_timestamp){
                earliest_at = request_queue[i].arrival_timestamp;
                earliest_at_index = i;
            }
        }
        next_RCB = request_queue[earliest_at_index];
        for(int i = earliest_at_index; i < *queue_cnt - 1; i++){
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt = *queue_cnt -1;
        return next_RCB;
    }
    else{
        struct RCB NULLRCB = { 0, 0, 0, 0, 0};
        return NULLRCB;
    }
}
/*A sample execution input and output:
input/output    parameter       value
input           request_queue   [RID:1, AT:10, CYL:124323, ADDR:124323, PID:1]
input           queue_cnt       1
output          request_queue   EMPTY
output          queue_cnt       0
output          RCB             [RID:1, AT:10, CYL:124323, ADDR:124323, PID:1]*/

/* 2 SSTF */
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    /*The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being NULLRCB), otherwise, 
    it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue*/
    if((current_request.request_id == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) //Disk free
        && (current_request.address == 0) && (current_request.process_id == 0)){
            return new_request;
    }
    else{
        if(*queue_cnt < QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt = *queue_cnt + 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
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
    if(*queue_cnt > 0){
        struct RCB next_RCB;
        int request_index = 0;
        int closest_cylinder = abs(current_cylinder - request_queue[0].cylinder);
        int earliest_at = request_queue[0].arrival_timestamp;
        for(int i = 1; i < *queue_cnt; i++){
            if(closest_cylinder > request_queue[i].cylinder){
                closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                earliest_at = request_queue[i].arrival_timestamp;
                request_index = i;
            }
        }
        next_RCB = request_queue[request_index];
        for(int i = request_index; i < *queue_cnt - 1; i++){
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt = *queue_cnt -1;
        return next_RCB;
    }
    else{
        struct RCB NULLRCB = { 0, 0, 0, 0, 0};
        return NULLRCB;
    }
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

/* 3 LOOK */
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    /*The method returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being NULLRCB), 
    otherwise, it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue. */
    if((current_request.request_id == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) //Disk free
        && (current_request.address == 0) && (current_request.process_id == 0)){
            return new_request;
    }
    else{
        if(*queue_cnt < QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt = *queue_cnt + 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
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

    1.If there are requests in the queue with the same cylinder as the current cylinder, the method picks the one among these requests 
    with the earliest arrival time. 
    2.Otherwise, if the scan direction is 1 and there are requests with cylinders larger than the current 
    cylinder, the method picks the one among these whose cylinder is closest to the current cylinder. 
    2.Otherwise, if the scan direction is 1 and there are no requests with cylinders larger than the current cylinder, the method picks the request whose cylinder is closest 
    to the current cylinder. 
    3.Otherwise, if the scan direction is 0 and there are requests with cylinders smaller than the current cylinder, 
    the method picks the one among these whose cylinder is closest to the current cylinder. 
    3.Otherwise, if the scan direction is 0 and there 
    are requests with cylinders larger than the current cylinder, the method picks the request whose cylinder is closest to the current cylinder.  
   
    After picking the RCB from the request queue, as described above, the method removes the RCB from the queue and returns it.*/
    if(*queue_cnt > 0){
        struct RCB next_RCB;
        bool first_arrivaltime = false, closest_cylinder_same = false,  direction_first = false, opposite_direction_first = false, 
        larger_cylinders_present = false, smaller_cylinders_present = false;
        int request_index = 0;
        int closest_cylinder = abs(current_cylinder - request_queue[0].cylinder);
        int earliest_at = request_queue[0].arrival_timestamp;
        for(int i = 0; i < *queue_cnt; i++){
            if(current_cylinder == request_queue[i].cylinder){//if same cyl as curr
                if(first_arrivaltime){//first at store
                    earliest_at = request_queue[i].arrival_timestamp;
                    first_arrivaltime = true;
                    closest_cylinder_same = true;
                    request_index = i;
                }
                else if(earliest_at > request_queue[i].arrival_timestamp){//Earliest at
                    earliest_at = request_queue[i].arrival_timestamp;
                    request_index = i;
                }
            }
            else if((scan_direction == 1) && (closest_cylinder_same)){
                if(abs(current_cylinder - request_queue[i].cylinder > 0))
                    if(direction_first){
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        larger_cylinders_present = true;
                        direction_first = true;
                        request_index = i;
                    }
                    else if(closest_cylinder > abs(current_cylinder - readyqueue[i].cylinder)){
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        request_index = i;
                    }
                else if((larger_cylinders_present) && (abs(current_cylinder - request_queue[i].cylinder > 0))){
                    if(opposite_direction_first){
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        opposite_direction_first = true;
                        request_index = i;
                    }
                    else if(closest_cylinder > abs(current_cylinder - request_queue[i].cylinder)){
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        request_index = i;
                    }
                }
            }
            else if((scan_direction == 0) && (closest_cylinder_same)){//3 & Check for smaller cyl or lar
                if(abs(current_cylinder- request_queue[i].cylinder > 0)){
                    if(direction_first){//scan direction = 0 w/ smaller Cyl
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        smaller_cylinders_present = true;
                        direction_first = true;
                        request_index = i;
                    }
                    else if(closest_cylinder > abs(current_cylinder - request_queue[i].cylinder)){//scan direction = 0 w/ larger cyl
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        request_index = i;
                    }
                }
                else if((smaller_cylinders_present) &&  (abs(current_cylinder - request_queue[i].cylinder) > 0)){
                    if(opposite_direction_first){
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                        opposite_direction_first = true;
                        request_index = i;
                    }
                    else if(closest_cylinder > abs(current_cylinder - request_queue[i].cylinder)){
                        closest_cylinder = abs(current_cylinder - request_queue[i].cylinder;
                        request_index = i;
                    }
                        
                }   
            }
        }
        next_RCB = request_queue[request_index];
        for(int i = request_index; i < *queue_cnt - 1; i++){
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt = *queue_cnt - 1;
        return next_RCB;
    }
    else{
        struct RCB NULLRCB = { 0, 0, 0, 0, 0};
        return NULLRCB;
    }    
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