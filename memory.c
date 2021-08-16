#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "oslabs.h"

//NULLBLOCK as [start_address: 0, end_address: 0, segment_size: 0, process_id: 0]
/*struct MEMORY_BLOCK {
        int start_address;
        int end_address;
        int segment_size;
        int process_id; //0 indicates a free block
    }*/

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    /*This method allocates memory according to the Best Fit scheme. The method is given the process id of the requesting process, size of the 
    memory being requested, and the memory map. It finds the candidate memory blocks that can be allocated and chooses the one whose size is 
    closest to the requested size. 
    1.If the free block found is exactly of the same size as the requested size, the method updates the process id 
    to allocate it and returns this memory block. 
    2.If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and 
    -the second piece becoming a free block in the memory map. Thus, the method may alter the memory map appropriately. 
    3.Note that if there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK. */
    struct MEMORY_BLOCK freeblock = {0, 0, 0, 0};
    struct MEMORY_BLOCK allocated = {0, 0, 0, 0};
    bool size_match = false;
    int memory_map_index = 0;
    int best_fit_segment = 0;
    for(int i = 0; i <= *map_cnt - 1; i++){
        if(request_size == memory_map[i].segment_size){//1
            best_fit_segment = request_size;
            size_match = true;
            memory_map_index = i;
            break;
        }
        else if((memory_map[i].segment_size > request_size) && (memory_map[i].process_id == 0)){//2
            if(size_match){
                best_fit_segment = memory_map[i].segment_size;
                size_match = true;
                memory_map_index = i;
            }
            else{
                if(best_fit_segment > memory_map[i].segment_size){
                    best_fit_segment = memory_map[i].segment_size;
                    memory_map_index = i;
                    }
            }
        }
    }
    if(!size_match){//1 + 2
        freeblock = memory_map[memory_map_index];// 0, 1023, 1024, 0
        allocated.start_address = freeblock.start_address;//0
        allocated.end_address = allocated.start_address + request_size - 1;//0 + 10 - 1 = 9
        allocated.segment_size = request_size;//10
        allocated.process_id = process_id;//32
        *map_cnt = *map_cnt + 1;//2
        for(int i = memory_map_index; i <= *map_cnt; i++){//move freeblock
            memory_map[memory_map_index + 1] = memory_map[i];
        }
        memory_map[memory_map_index + 1].start_address = allocated.end_address + 1;//10
        memory_map[memory_map_index + 1].end_address = freeblock.end_address;//1023
        memory_map[memory_map_index + 1].segment_size = freeblock.segment_size - request_size;//1024 - 10 = 1014
        memory_map[memory_map_index + 1].process_id = freeblock.process_id;//0
        memory_map[memory_map_index] = allocated;
        return allocated;
    }
    else{//3
        struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};
        return NULLBLOCK;
    }  
}
/*A sample execution input and output:
input/output        parameter       value
input               memory_map      [start_address: 0, end_address: 1023, segment_size: 1024, process_id: 0]
input               map_cnt         1
input               request_size    10
input               process_id      32
output              memory_map      [start_address: 0, end_address: 9, segment_size: 10, process_id: 32],
                                    [start_address: 10, end_address: 1023, segment_size: 1014, process_id: 0]
output              map_cnt         2
output              MEMORY_BLOCK    [start_address: 0, end_address: 9, segment_size: 10, process_id: 32]*/

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    /*This method allocates memory according to the First Fit scheme. The method is given the process id of the requesting process, size of the 
    memory being requested, and the memory map. It finds the first (lowest starting address) free memory block whose size is at least as large as 
    the requested size. 
    1.If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it 
    and returns this memory block. 
    2.If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and the 
    second piece becoming a free block in the memory map. Thus, the method may alter the memory map appropriately. 
    3.Note that if there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK. */
    struct MEMORY_BLOCK freeblock = {0, 0, 0, 0};
    struct MEMORY_BLOCK allocated = {0, 0, 0, 0};
    bool size_match = false;
    int memory_map_index = 0;
    int first_fit_segement = 0; 
    for(int i = 0; i <= *map_cnt - 1; i++){
        if(request_size == memory_map[i].segment_size){//1
            first_fit_segement = request_size;
            size_match = true;
            memory_map_index = i;
            break;
        }
        else if((request_size <= memory_map[i].segment_size) && (memory_map[i].process_id == 0)){//2
            if(size_match){
                first_fit_segement = memory_map[i].segment_size;
                size_match = true;
                memory_map_index = i;
            }
            else{
                if(first_fit_segement < memory_map[i].segment_size){
                    first_fit_segement = memory_map[i].segment_size;
                    memory_map_index = i;
                }
            }
        } 
    }
    if(!size_match){//1 + 2
        freeblock = memory_map[memory_map_index];// 0, 1023, 1024, 0
        allocated.start_address = freeblock.start_address;//0
        allocated.end_address = allocated.start_address + request_size - 1;//0 + 10 - 1=9
        allocated.segment_size = request_size;//10
        allocated.process_id = process_id;//32
        *map_cnt = *map_cnt + 1;//2
        for(int i = memory_map_index; i <= *map_cnt; i++){//move freeblock
            memory_map[memory_map_index + 1] = memory_map[i];
        }
        memory_map[memory_map_index + 1].start_address = allocated.end_address + 1;//10
        memory_map[memory_map_index + 1].end_address = freeblock.end_address;//1023
        memory_map[memory_map_index + 1].segment_size = first_fit_segement - request_size;//1024 - 10 = 1014
        memory_map[memory_map_index + 1].process_id = freeblock.process_id;
        memory_map[memory_map_index] = allocated;
        return allocated;
    }
    else{//3
        struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};
        return NULLBLOCK;
    }
}
/*A sample execution input and output:
input/output        parameter       value
input               memory_map      [start_address: 0, end_address: 1023, segment_size: 1024, process_id: 0]
input               map_cnt         1
input               request_size    10 
input               process_id      32
output              memory_map      [start_address: 0, end_address: 9, segment_size: 10, process_id: 32],
                                    [start_address: 10, end_address: 1023, segment_size: 1014, process_id: 0]
output              map_cnt         2
output              MEMORY_BLOCK    [start_address: 0, end_address: 9, segment_size: 10, process_id: 32]*/

struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id){
    /*This method allocates memory according to the Worst Fit scheme. The method is given the process id of the requesting process, size of the 
    memory being requested, and the memory map. It finds the candidate memory blocks that can be allocated and chooses the largest among these 
    blocks. 
    1.If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it and returns 
    this memory block. 
    2.If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and 
    the second piece becoming a free block in the memory map. Thus, the method may alter the memory map appropriately. 
    3.Note that if there is no free block of memory (in the memory map) that is at least as large as the requested size, the method returns the NULLBLOCK. */
    struct MEMORY_BLOCK freeblock = {0, 0, 0, 0};
    struct MEMORY_BLOCK allocated = {0, 0, 0, 0};
    bool size_match = false;
    int memory_map_index = 0;
    int worst_fit_segement = 0; 
    for(int i = 0; i <= *map_cnt; i++){
        if((request_size <= memory_map[i].segment_size) && (memory_map[i].process_id == 0)){
            if(size_match){
                worst_fit_segement = memory_map[i].segment_size;
                size_match = true;
                memory_map_index = i;
            }
            else{
                if(worst_fit_segement < memory_map[i].segment_size){
                    worst_fit_segement = memory_map[i].segment_size;
                    memory_map_index = i;
                }
            }
        }
    }
    if(!size_match){
        freeblock = memory_map[memory_map_index];// 0, 1023, 1024, 0
        allocated.start_address = freeblock.start_address;//0
        allocated.end_address = allocated.start_address + request_size - 1;//0 + 10 - 1=9
        allocated.segment_size = request_size;//10
        allocated.process_id = process_id;//32
        *map_cnt = *map_cnt + 1;//2
        for(int i = memory_map_index; i < *map_cnt; i++){//move freeblock
            memory_map[memory_map_index + 1] = memory_map[i];
        }
        memory_map[memory_map_index + 1].start_address = allocated.end_address + 1;//10
        memory_map[memory_map_index + 1].end_address = freeblock.end_address;//1023
        memory_map[memory_map_index + 1].segment_size = worst_fit_segement - request_size;//1024 - 10 = 1014
        memory_map[memory_map_index + 1].process_id = freeblock.process_id;
        memory_map[memory_map_index] = allocated;
        return allocated;
    }
    else{
        struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};
        return NULLBLOCK;
    }
}
/*A sample execution input and output:
input/output        parameter       value
input               memory_map      [start_address: 0, end_address: 1023, segment_size: 1024, process_id: 0]
input               map_cnt         1
input               request_size    10
input               process_id      32
output              memory_map      [start_address: 0, end_address: 9, segment_size: 10, process_id: 32],
                                    [start_address: 10, end_address: 1023, segment_size: 1014, process_id: 0]
output              map_cnt         2
output              MEMORY_BLOCK    [start_address: 0, end_address: 9, segment_size: 10, process_id: 32]*/

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK   memory_map[MAPMAX],int *map_cnt, int process_id, int last_address){
    /*This method allocates memory according to the Next Fit scheme. The method is given the process id of the requesting process, size of the memory 
    requested, the memory map, and the address of the last block allocated. 
    It finds the first (lowest starting address) free memory block, greater than or equal to the previously allocated block address, whose size is at least 
    as the requested size. 
    1.If the free block found is exactly of the same size as the requested size, the method updates the process id to allocate it and returns this memory block. 
    2.If the free block found is larger than the requested size, the block is split into two pieces - the first piece allocated and the second piece becoming a free block in the memory map. 
    Thus, the method may alter the memory map appropriately. 
    3.Note that if there is no free block of memory (in the memory map) that is at least as large 
    as the requested size, the method returns the NULLBLOCK. */
    struct MEMORY_BLOCK freeblock = {0, 0, 0, 0};
    struct MEMORY_BLOCK allocated = {0, 0, 0, 0};
    bool size_match = false;
    int memory_map_index = 0;
    int next_fit_segement = 0; 
    for(int i = 0; i <= *map_cnt - 1; i++){
        if((request_size == memory_map[i].segment_size) && (memory_map[i].process_id == 0)){
            next_fit_segement = request_size;
            size_match = true;
            memory_map_index = i;
        }
        else if((request_size < memory_map[i].segment_size) && (memory_map[i].process_id == 0)){
            if(size_match){
                next_fit_segement = memory_map[i].segment_size;
                size_match = true;
                memory_map_index;
            }
            else{
                if(next_fit_segement < memory_map[i].segment_size){
                    next_fit_segement = memory_map[i].segment_size;
                    memory_map_index = i;
                }
            }
        }
    }
    if(!size_match){
        freeblock = memory_map[memory_map_index];// 0, 1023, 1024, 0
        allocated.start_address = freeblock.start_address;//0
        allocated.end_address = allocated.start_address + request_size - 1;//0 + 10 - 1=9
        allocated.segment_size = request_size;//10
        allocated.process_id = process_id;//32
        *map_cnt = *map_cnt + 1;//2
        for(int i = memory_map_index; i <= *map_cnt; i++){//move freeblock
            memory_map[memory_map_index + 1] = memory_map[i];
        }
        memory_map[memory_map_index + 1].start_address = allocated.end_address + 1;//10
        memory_map[memory_map_index + 1].end_address = freeblock.end_address;//1023
        memory_map[memory_map_index + 1].segment_size = next_fit_segement - request_size;//1024 - 10 = 1014
        memory_map[memory_map_index + 1].process_id = freeblock.process_id;
        memory_map[memory_map_index] = allocated;
        return allocated;
    }
    else{
        struct MEMORY_BLOCK NULLBLOCK = {0,0,0,0};
        return NULLBLOCK;
    }
}
/*A sample execution input and output:
input/output        parameter       value
input               memory_map      [start_address: 0, end_address: 1023, segment_size: 1024, process_id: 0]
input               map_cnt         1
input               request_size    10
input               process_id      32
input               last_address    0
output              memory_map      [start_address: 0, end_address: 9, segment_size: 10, process_id: 32],
                                    [start_address: 10, end_address: 1023, segment_size: 1014, process_id: 0]
output              map_cnt         2
output              MEMORY_BLOCK    [start_address: 0, end_address: 9, segment_size: 10, process_id: 32]*/

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt){
    /*
    This method releases a memory block. Accordingly, it modifies the memory map passed in. Specifically, it marks the released block of memory as free 
    and then it merges that block with adjacent free blocks if any. That is, if the memory block adjacent to the newly released block is free, the memory 
    map is altered to reduce the number of memory blocks by one and the ending address (and the size) of the previous free block extended. Note that the 
    method does not have any explicit return value and instead modifies the memory map passed in  */
    bool free = false;
    //int release_memory_index = 0;
    if((*map_cnt == 1) && (memory_map[0].start_address == 0) && (memory_map[0].end_address == 0) && 
    (memory_map[0].segment_size == 0) && (memory_map[0].process_id == 0)){
        return;
    }
    else{
        for(int i = 1; i < *map_cnt - 1; i++){
            if((freed_block.start_address == memory_map[i].start_address) && (freed_block.end_address == memory_map[i].end_address) && 
            (freed_block.segment_size == memory_map[i].segment_size) && (freed_block.process_id == memory_map[i].process_id)){
                if(i > 0){
                    if(memory_map[i - 1].process_id == 0){
                        memory_map[i - 1].end_address = freed_block.end_address;
                        memory_map[i - 1].segment_size = memory_map[i - 1].segment_size + freed_block.segment_size;
                        for(int release_memory_index = i; release_memory_index <= *map_cnt; release_memory_index++){
                            memory_map[release_memory_index] = memory_map[release_memory_index + 1];
                        }
                        *map_cnt = *map_cnt - 1;
                        free = true;
                    }
                }
                else if(i < *map_cnt -1){
                    if(free){
                        i = i + 1;
                    }
                    else if(memory_map[i].process_id == 0){
                        memory_map[i].start_address = memory_map[i - 1].start_address;
                        memory_map[i].segment_size = memory_map[i].end_address - memory_map[i].start_address + 1;
                        for(int release_memory_index = i; release_memory_index <= *map_cnt; release_memory_index++){
                            memory_map[release_memory_index - 1] = memory_map[release_memory_index];
                        }
                        *map_cnt = *map_cnt - 1;
                    }
                }    
            break;
            }
        }
    }
}
/*A sample execution input and output:
input/output        parameter       value
input               memory_map      [start_address: 0, end_address: 7, segment_size: 8, process_id: 12],
                                    [start_address: 8, end_address: 15, segment_size: 8, process_id: 0],
                                    [start_address: 16, end_address: 23, segment_size: 8, process_id: 13],
                                    [start_address: 24, end_address: 27, segment_size: 4, process_id: 0],
                                    [start_address: 28, end_address: 29, segment_size: 2, process_id: 11]
input               map_cnt         5
input               freed_block     [start_address: 16, end_address: 23, segment_size: 8, process_id: 13]
output              memory_map      [start_address: 0, end_address: 7, segment_size: 8, process_id: 12],
                                    [start_address: 8, end_address: 27, segment_size: 20, process_id: 0],
                                    [start_address: 28, end_address: 29, segment_size: 2, process_id: 11]
output              map_cnt         3*/