#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "oslabs.h"
const 
/* struct PTE {
        int is_valid;
        int frame_number;
        int arrival_timestamp;
        int last_access_timestamp;
        int reference_count;
    }*/

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    /*The function determines the memory frame number for the logical page and returns this number. 
    First the function checks if the page being referenced is already in memory (i.e., the page-table entry has the valid bit true). If so, it returns the frame number, 
    after modifying the last_access_timestamp and the reference_count fields of the page-table entry. 

    If the page being referenced is not in memory, the function checks if there are any free frames (i.e., the process frame pool is not empty). If so, a frame is 
    removed from the process frame pool and the frame number is inserted into the page-table entry corresponding to the logical page number. In addition, the other 
    fields of the page-table entry are set appropriately. The function returns the frame number associated with the page-table entry. 

    If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced. The function selects among all the pages 
    of the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest arrival_timestamp. It marks that page_table entry 
    as invalid, along with setting the frame_number, arrival_timestamp, last_access_timestamp and reference_count to -1. It then sets the frame_number of the page-table 
    entry of the newly-referenced page to the newly freed frame. It also sets the arrival_timestamp, the last_access_timestamp and the reference_count fields of the 
    page-table entry appropriately. Finally, the function returns this frame number.*/
    if(page_table[page_number].is_valid == 1){//check if page being ref exist in mem
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = page_table[page_number].reference_count + 1;
        return page_table[page_number].frame_number;
    }
    else if((page_table[page_number].is_valid == 0) && (*frame_cnt > 0)){
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return page_table[page_number].frame_number;
    }
    else{
        bool flag = false;
        int temp_index = 0;
        int temp_frame = 0;
        int temp_arrival_stamp = 0;
        for(int i = 0; i < *table_cnt; i++){
            if((page_table[i].is_valid == 1) && (flag)){
                temp_frame = page_table[i].frame_number;
                temp_arrival_stamp = page_table[i].arrival_timestamp;
                flag = true;
                temp_index = i;
            }
            else if((page_table[i].is_valid == 1) && (temp_arrival_stamp > page_table[i].arrival_timestamp) && (!flag)){
                temp_frame = page_table[i].frame_number;
                temp_arrival_stamp = page_table[i].arrival_timestamp;
                temp_index = i;
            }
        }
        if(!flag){
            page_table[temp_index].is_valid = 0;
            page_table[temp_index].frame_number = -1;
            page_table[temp_index].arrival_timestamp = 0;
            page_table[temp_index].last_access_timestamp = 0;
            page_table[temp_index].reference_count = 0;
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = temp_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return temp_frame;
    }
}
/*A sample execution input and output:
input/output    parameter           value
input           page_table          [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:10,ATS:3,LATS:3,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:20,ATS:2,LATS:4,RC:2]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:30,ATS:1,LATS:1,RC:1]
input           table_cnt           8
input           page_number         0
input           frame_pool          EMPTY
input           frame_cnt           0
input           current_timestamp   12
output          page_table          [IV:true,FN:30,ATS:12,LATS:12,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:10,ATS:3,LATS:3,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:20,ATS:2,LATS:4,RC:2]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:0,LATS:0,RC:0]
output          table_cnt           8
output          frame_pool          EMPTY
output          frame_cnt           0
output          int                 30*/

int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    /*The function returns the estimated number of page faults for the reference string, with respect to the pool of frames allocated to the process. For each logical 
    page number (in the sequence), the function simulates the processing of the page access in the FIFO system. It keeps track of the number of page faults that occur 
    in the system as it simulates the processing of the entire sequence of logical page numbers. 

    When simulating the processing of a page access, the function first checks if the page being referenced is already in memory (i.e., the page-table entry has the 
    valid bit true). If so, it modifies the last_access_timestamp and the reference_count fields of the page-table entry. It then moves on to the processing of the 
    next entry in the sequence of logical page numbers. 

    In order to simulate timestamps, the function starts with a timestamp of 1 and increments it whenever the processing of a new page access is begun. 

    If the page being referenced is not in memory, the function checks if there are any free frames (i.e., the process frame pool is not empty). If so, a frame is 
    removed from the process frame pool and the frame number is inserted into the page-table entry corresponding to the logical page number. In addition, the other 
    fields of the page-table entry are set appropriately. The function counts this page access as a page fault. 

    If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced. The function selects among all the pages 
    of the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest arrival_timestamp. It marks that page-table entry 
    as invalid, along with setting the arrival_timestamp, last_access_timestamp and reference_count to 0. It then sets the frame_number of the page-table entry of the 
    newly-referenced page to the newly freed frame. It also sets the arrival_timestamp, the last_access_timestamp and the reference_count fields of the page-table entry 
    appropriately. The function counts this page access as a page fault. 

    The function returns the total number of page faults encountered in the simulation*/
    int timestamp = 1;
    int page_faults = 0;
    for(int i = 0; i < reference_cnt; i++){
        if(page_table[refrence_string[i]].is_valid == 1){//check if page being ref exist in mem
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = page_table[refrence_string[i]].reference_count + 1;
        }
        else if((page_table[refrence_string[i]].is_valid == 0) && (frame_cnt > 0)){
            frame_cnt = frame_cnt - 1;
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = 1;
            page_faults = page_faults + 1;
        }
        else if((page_table[refrence_string[i]].is_valid == 0) && (frame_cnt == 0)){
            bool flag = false;
            int temp_index = 0;
            int temp_frame = 0;
            int temp_arrival_stamp = 0;
            page_faults = page_faults + 1;
            for(int i = 0; i < table_cnt; i++){
                if((page_table[i].is_valid == 1) && (flag)){
                    temp_frame = page_table[i].frame_number;
                    temp_arrival_stamp = page_table[i].arrival_timestamp;
                    flag = true;
                    temp_index = i;
                }
                else if((page_table[i].is_valid == 1) && (temp_arrival_stamp > page_table[i].arrival_timestamp) && (!flag)){
                    temp_frame = page_table[i].frame_number;
                    temp_arrival_stamp = page_table[i].arrival_timestamp;
                    temp_index = i;
                }
            }
            if(!flag){
                page_table[temp_index].is_valid = 0;
                page_table[temp_index].frame_number = -1;
                page_table[temp_index].arrival_timestamp = 0;
                page_table[temp_index].last_access_timestamp = 0;
                page_table[temp_index].reference_count = 0;
                page_table[refrence_string[i]].is_valid = 1;
                page_table[refrence_string[i]].frame_number = temp_frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 1;
            }
        }
        timestamp = timestamp + 1;
    }
    return page_faults;
}
/*A sample execution input and output:
input/output    parameter       value
input           page_table      [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
input           table_cnt       8
input           reference_string[0, 3, 2, 6, 3, 4, 5, 2, 4, 5, 6]
input           reference_cnt   11
input           frame_pool      [0, 1, 2]
input           frame_cnt       3
output          faults          8*/

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    /*The function determines the memory frame number for the logical page and returns this number. 

    First the function checks if the page being referenced is already in memory (i.e., the page-table entry has the valid bit true). If so, it returns the frame number, 
    after modifying the last_access_timestamp and the reference_count fields of the page-table entry. 

    If the page being referenced is not in memory, the function checks if there are any free frames (i.e., the process frame pool is not empty). If so, a frame is 
    removed from the process frame pool and the frame number is inserted into the page-table entry corresponding to the logical page number. In addition, the other 
    fields of the page-table entry are set appropriately. The function returns the frame number associated with the page-table entry. 

    If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced. The function selects among all the pages 
    of the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest last_access_timestamp. It marks that page-table 
    entry as invalifd, along with setting the frame_number, arrival_timestamp, last_access_timestamp and reference_count to -1. It then sets the frame_number of the 
    page-table entry of the newly-referenced page to the newly freed frame. It also sets the arrival_timestamp, the last_access_timestamp and the reference_count 
    fields of the page-table entry appropriately. Finally, the function returns this frame number.*/
    if(page_table[page_number].is_valid == 1){
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = page_table[page_number].reference_count + 1;
        return page_table[page_number].frame_number;
    }
    else if((page_table[page_number].is_valid == 0) && (*frame_cnt > 0)){
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return page_table[page_number].frame_number;
    }
    else{
        bool flag = false;
        int temp_index = 0;
        int temp_frame = 0;
        int temp_last_access_stamp = 0;
        for(int i = 0; i < *table_cnt; i++){
            if((page_table[i].is_valid == 1) && (flag)){
                temp_frame = page_table[i].frame_number;
                temp_last_access_stamp = page_table[i].last_access_timestamp;
                flag = true;
                temp_index = i;
            }
            else if((page_table[i].is_valid == 1) && (temp_last_access_stamp > page_table[i].last_access_timestamp) && (!flag)){
                temp_frame = page_table[i].frame_number;
                temp_last_access_stamp = page_table[i].last_access_timestamp;
                temp_index = i;
            }
        }
        if(!flag){
            page_table[temp_index].is_valid = 0;
            page_table[temp_index].frame_number = -1;
            page_table[temp_index].arrival_timestamp = 0;
            page_table[temp_index].last_access_timestamp = 0;
            page_table[temp_index].reference_count = 0;
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = temp_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return temp_frame;
    }
}
/*
input/output    parameter           value
input           page_table          [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:10,ATS:3,LATS:3,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:20,ATS:2,LATS:4,RC:2]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:30,ATS:1,LATS:1,RC:1]
inpu            table_cnt           8
input           page_number         0
input           frame_pool          EMPTY
input           frame_cnt           0
input           current_timestamp   12
output          page_table          [IV:true,FN:30,ATS:12,LATS:12,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:10,ATS:3,LATS:3,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:20,ATS:2,LATS:4,RC:2]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:0,LATS:0,RC:0]
output          table_cnt           8
output          int                 30*/

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    /*The function returns the estimated number of page faults for the reference string, with respect to the pool of frames allocated to the process. For each 
    logical page number (in the sequence), the function simulates the processing of the page access in the LRU system. It keeps track of the number of page faults 
    that occur in the system as it simulates the processing of the entire sequence of logical page numbers. 

    When simulating the processing of a page access, the function first checks if the page being referenced is already in memory (i.e., the page-table entry has 
    the valid bit true). If so, it modifies the last_access_timestamp and the reference_count fields of the page-table entry. It then moves on to the processing 
    of the next entry in the sequence of logical page numbers. 

    In order to simulate timestamps, the function starts with a timestamp of 1 and increments it whenever the processing of a new page access is begun. 

    If the page being referenced is not in memory, the function checks if there are any free frames (i.e., the process frame pool is not empty). If so, a frame is 
    removed from the process frame pool and the frame number is inserted into the page-table entry corresponding to the logical page number. In addition, the other 
    fields of the page-table entry are set appropriately. The function counts this page access as a page fault. 

    If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced. The function selects among all the pages 
    of the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest last_access_timestamp. It marks that page-table 
    entry as invalid, along with setting the arrival_timestamp, last_access_timestamp and reference_count to 0. It then sets the frame_number of the page-table entry 
    of the newly-referenced page to the newly freed frame. It also sets the arrival_timestamp, the last_access_timestamp and the reference_count fields of the page-table 
    entry appropriately. The function counts this page access as a page fault. 

    The function returns the total number of page faults encountered in the simulation. */
    int timestamp = 1;
    int page_faults = 0;
    for(int i = 0; i< reference_cnt; i++){
        if(page_table[refrence_string[i]].is_valid == 1){//check if page being ref exist in mem
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = page_table[refrence_string[i]].reference_count + 1;
        }
        else if((page_table[refrence_string[i]].is_valid == 0) && (frame_cnt > 0)){
            frame_cnt = frame_cnt - 1;
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = 1;
            page_faults = page_faults + 1;
        }
        else if((page_table[refrence_string[i]].is_valid == 0) && (frame_cnt == 0)){
            bool flag = false;
            int temp_index = 0;
            int temp_frame = 0;
            int temp_arrival_stamp = 0;
            page_faults = page_faults + 1;
            for(int i = 0; i < table_cnt; i++){
                if((page_table[i].is_valid == 1) && (flag)){
                    temp_frame = page_table[i].frame_number;
                    temp_arrival_stamp = page_table[i].arrival_timestamp;
                    flag = true;
                    temp_index = i;
                }
                else if((page_table[i].is_valid == 1) && (temp_arrival_stamp > page_table[i].last_access_timestamp) && (!flag)){
                    temp_frame = page_table[i].frame_number;
                    temp_arrival_stamp = page_table[i].last_access_timestamp;
                    temp_index = i;
                }
            }
            if(!flag){
                page_table[temp_index].is_valid = 0;
                page_table[temp_index].frame_number = -1;
                page_table[temp_index].arrival_timestamp = 0;
                page_table[temp_index].last_access_timestamp = 0;
                page_table[temp_index].reference_count = 0;
                page_table[refrence_string[i]].is_valid = 1;
                page_table[refrence_string[i]].frame_number = temp_frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 1;
            }
        }
        timestamp = timestamp + 1;
    }
    return page_faults;
}
/*A sample execution input and output:
input/output    parameter           value
input           page_table          [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
input           table_cnt           8
input           reference_string    [0, 3, 2, 6, 3, 4, 5, 2, 4, 6, 5]
input           reference_cnt       11
input           frame_pool          [0, 1, 2]
input           frame_cnt           3
output          faults              9*/

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    /*The function determines the memory frame number for the logical page and returns this number. 

    First the function checks if the page being referenced is already in memory (i.e., the page-table entry has the valid bit true). If so, it returns the frame number, 
    after modifying the last_access_timestamp and the reference_count fields of the page-table entry. 

    If the page being referenced is not in memory, the function checks if there are any free frames (i.e., the process frame pool is not empty). If so, a frame is removed 
    from the process frame pool and the frame number is inserted into the page-table entry corresponding to the logical page number. In addition, the other fields of the 
    page-table entry are set appropriately. The function returns the frame number associated with the page-table entry. 

    If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced. The function selects among all the pages of 
    the process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest reference_count. If multiple pages have the smallest 
    reference_count, the one with the smallest arrival_timestamp among these is selected. After selecting the page for replacement, the function marks that page-table 
    entry as invalid, and sets the frame_number, arrival_timestamp, last_access_timestamp and reference_count to -1. It then sets the frame_number of the page-table 
    entry of the newly-referenced page to the newly freed frame. It also sets the arrival_timestamp, the last_access_timestamp and the reference_count fields of the 
    page-table entry appropriately. Finally, the function returns this frame number. */
    if(page_table[page_number].is_valid == 1){//check if page being ref exist in mem
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = page_table[page_number].reference_count + 1;
        return page_table[page_number].frame_number;
    }
    else if((page_table[page_number].is_valid == 0) && (*frame_cnt > 0)){
        *frame_cnt = *frame_cnt - 1;
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return page_table[page_number].frame_number;
    }
    else{
        bool flag = false;
        int temp_index = 0;
        int temp_frame = 0;
        int temp_reference_count = 0;
        int temp_arrival_stamp = 0;
        for(int i = 0; i < *table_cnt; i++){
            if((page_table[i].is_valid == 1) && (flag)){
                temp_frame = page_table[i].frame_number;
                temp_arrival_stamp = page_table[i].arrival_timestamp;
                temp_reference_count = page_table[i].reference_count;
                flag = true;
                temp_index = i;
            }
            else if((page_table[i].is_valid == 1) && (temp_reference_count > page_table[i].reference_count) && (!flag)){
                if((temp_reference_count == page_table[i].reference_count) && (temp_arrival_stamp > page_table[i].arrival_timestamp)){
                    temp_frame = page_table[i].frame_number;
                    temp_reference_count = page_table[i].reference_count;
                    temp_arrival_stamp = page_table[i].arrival_timestamp;
                    temp_index = i;
                }
                else if( temp_reference_count > page_table[i].reference_count){
                    temp_frame = page_table[i].frame_number;
                    temp_reference_count = page_table[i].reference_count;
                    temp_arrival_stamp = page_table[i].arrival_timestamp;
                    temp_index = i;
                }
            }
        }
        if(!flag){
            page_table[temp_index].is_valid = 0;
            page_table[temp_index].frame_number = -1;
            page_table[temp_index].arrival_timestamp = 0;
            page_table[temp_index].last_access_timestamp = 0;
            page_table[temp_index].reference_count = 0;
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = temp_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
    return temp_frame;
    } 
}
/*A sample execution input and output:
input/output    parameter           value
input           page_table          [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:10,ATS:3,LATS:3,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:20,ATS:2,LATS:4,RC:2]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:30,ATS:1,LATS:1,RC:1]
input           table_cnt           8
input           page_number         0
input           frame_pool          EMPTY
input           frame_cnt           0
input           current_timestamp   12
output          page_table          [IV:true,FN:30,ATS:12,LATS:12,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:10,ATS:3,LATS:3,RC:1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:true,FN:20,ATS:2,LATS:4,RC:2]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:0,LATS:0,RC:0]
output          table_cnt           8
output          int                 30 */

int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    /*The function returns the estimated number of page faults for the reference string, with respect to the pool of frames allocated to the process. For each logical page 
    number (in the sequence), the function simulates the processing of the page access in the LFU system. It keeps track of the number of page faults that occur in the system 
    as it simulates the processing of the entire sequence of logical page numbers. 

    When simulating the processing of a page access, the function first checks if the page being referenced is already in memory (i.e., the page-table entry has the valid 
    bit true). If so, it modifies the last_access_timestamp and the reference_count fields of the page-table entry. It then moves on to the processing of the next entry in 
    the sequence of logical page numbers. 

    In order to simulate timestamps, the function starts with a timestamp of 1 and increments it whenever the processing of a new page access is begun. 

    If the page being referenced is not in memory, the function checks if there are any free frames (i.e., the process frame pool is not empty). If so, a frame is removed 
    from the process frame pool and the frame number is inserted into the page-table entry corresponding to the logical page number. In addition, the other fields of the 
    page-table entry are set appropriately. The function counts this page access as a page fault. 

    If the page being referenced is not in memory and there are no free frames for the process, a page needs to be replaced. The function selects among all the pages of the 
    process that are currently in memory (i.e., they have valid bits as true) the page that has the smallest reference_count. If multiple pages exist with the smallest 
    reference_count, the one with the smallest arrival_timestamp is chosen. The function then marks that page-table entry as invalid, along with setting the arrival_timestamp, 
    last_access_timestamp and reference_count to 0. It then sets the frame_number of the page-table entry of the newly-referenced page to the newly freed frame. It also sets 
    the arrival_timestamp, the last_access_timestamp and the reference_count fields of the page-table entry appropriately. The function counts this page access as a page fault. 

    The function returns the total number of page faults encountered in the simulation.*/
    int timestamp = 1;
    int page_faults = 0;
    for(int i = 0; i< reference_cnt; i++){
        if(page_table[refrence_string[i]].is_valid == 1){//check if page being ref exist in mem
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = page_table[refrence_string[i]].reference_count + 1;
        }
        else if((page_table[refrence_string[i]].is_valid == 0) && (frame_cnt > 0)){
            frame_cnt = frame_cnt - 1;
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].is_valid = true;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = 1;
            page_faults = page_faults + 1;
        }
        else if((page_table[refrence_string[i]].is_valid == 0) && (frame_cnt == 0)){
            bool flag = false;
            int temp_index = 0;
            int temp_frame = 0;
            int temp_reference_count = 0;
            int temp_arrival_stamp = 0;
            for(int i = 0; i < table_cnt; i++){
                if((page_table[i].is_valid == 1) && (flag)){
                    temp_frame = page_table[i].frame_number;
                    temp_arrival_stamp = page_table[i].arrival_timestamp;
                    temp_reference_count = page_table[i].reference_count;
                    flag = true;
                    temp_index = i;
                }
                else if((page_table[i].is_valid == 1) && (temp_reference_count > page_table[i].reference_count) && (!flag)){
                    if((temp_reference_count == page_table[i].reference_count) && (temp_arrival_stamp > page_table[i].arrival_timestamp)){
                        temp_frame = page_table[i].frame_number;
                        temp_reference_count = page_table[i].reference_count;
                        temp_arrival_stamp = page_table[i].arrival_timestamp;
                        temp_index = i;
                    }
                    else if(temp_reference_count > page_table[i].reference_count){
                        temp_frame = page_table[i].frame_number;
                        temp_reference_count = page_table[i].reference_count;
                        temp_arrival_stamp = page_table[i].arrival_timestamp;
                        temp_index = i;
                    }

                }
                if(!flag){
                    page_table[temp_index].is_valid = 0;
                    page_table[temp_index].frame_number = -1;
                    page_table[temp_index].arrival_timestamp = 0;
                    page_table[temp_index].last_access_timestamp = 0;
                    page_table[temp_index].reference_count = 0;
                    page_table[refrence_string[i]].is_valid = 1;
                    page_table[refrence_string[i]].frame_number = temp_frame;
                    page_table[refrence_string[i]].arrival_timestamp = timestamp;
                    page_table[refrence_string[i]].last_access_timestamp = timestamp;
                    page_table[refrence_string[i]].reference_count = 1;
                }
            }
            timestamp = timestamp + 1;
        }
    }
    return page_faults;
}
/*A sample execution input and output:
input/output    parameter           value
input           page_table          [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
                                    [IV:false,FN:-1,ATS:-1,LATS:-1,RC:-1]
input           table_cnt           8
input           reference_string    [0, 3, 2, 6, 3, 4, 5, 2, 6, 4, 5]
input           reference_cnt       11
input           frame_pool          [0, 1, 2]
input           frame_cnt           3
output          faults              10 */