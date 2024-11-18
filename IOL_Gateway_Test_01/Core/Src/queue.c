#include "queue.h"

void Q_Init(Q_queue_t *queue, uint8_t *data, uint16_t len){
    queue->idx_read = 0;
    queue->idx_write = 0;
    queue->count_full = 0;
    queue->count_empty = 0;
    queue->size = len;
    queue->data = data;
}

void Q_Clear(Q_queue_t *queue){
    queue->idx_write = queue->idx_read;
}

bool Q_Write(Q_queue_t *queue, uint8_t *data, uint16_t len){
    int empty_bytes;
    uint16_t next_idx;
    uint16_t num_first;
    bool ret = false;
    
    empty_bytes = Q_NumEmptyBytes(queue);  
    if(empty_bytes >= len){
        next_idx = queue->idx_write + len;
        if(next_idx < queue->size){
            memcpy(&(queue->data[queue->idx_write]),data,len);  //memcpy(dest,source,num)
        }
        else{
            num_first = queue->size-queue->idx_write;
            memcpy(&(queue->data[queue->idx_write]),data,num_first);
            memcpy(&(queue->data[0]),&(data[num_first]),len-num_first);
        }        
        queue->idx_write = next_idx % queue->size;
        ret = true;        
    }
    else    queue->count_full = _upperlimit(queue->count_full+1,0xFFFF);      
    return ret;    
}


bool Q_WriteWithLen(Q_queue_t *queue, uint8_t *data, uint16_t len){
    int empty_bytes;
    uint16_t next_idx;
    uint16_t num_first;
    bool ret = false;
    
    empty_bytes = Q_NumEmptyBytes(queue);  
    if(empty_bytes >= len+1){        
        next_idx = queue->idx_write + len + 1;
        if(next_idx < queue->size){
            queue->data[queue->idx_write] = len;  
            memcpy(&(queue->data[queue->idx_write+1]),data,len);  //memcpy(dest,source,num)
        }
        else{
            num_first = queue->size-queue->idx_write;
            if(num_first == 1){
                queue->data[queue->idx_write] = len;
                memcpy(&(queue->data[0]),data,len);    
            }
            else{
                queue->data[queue->idx_write] = len;
                memcpy(&(queue->data[queue->idx_write+1]),data,num_first-1);
                memcpy(&(queue->data[0]),&(data[num_first-1]),len-num_first+1);
            }
        }        
        queue->idx_write = next_idx % queue->size;
        ret = true;                
    }
    else    queue->count_full = _upperlimit(queue->count_full+1,0xFFFF);
    return ret;    
}

bool Q_Read(Q_queue_t *queue, uint8_t *data, uint16_t len){
    bool ret;
    uint16_t next_idx;
    uint16_t num_first;

    if(Q_NumContents(queue) >= len){
        next_idx = queue->idx_read + len;
        if(next_idx < queue->size){
            memcpy(data,&(queue->data[queue->idx_read]),len);  //memcpy(dest,source,num)
        }
        else{
            num_first = queue->size-queue->idx_read;
            memcpy(data,&(queue->data[queue->idx_read]),num_first);  //memcpy(dest,source,num)
            memcpy(&(data[num_first]),&(queue->data[0]),len-num_first);
        }        
        queue->idx_read = (queue->idx_read+len)%queue->size;
        ret = true;
    }
    else{
        queue->count_empty = _upperlimit(queue->count_empty+1,0xFFFF);
        ret = false;
    }
    return ret;
}

bool Q_ReadByte(Q_queue_t *queue, uint8_t *data){
    bool ret = false;
    
    if(Q_NumContents(queue) >= 1){        
        *data = queue->data[queue->idx_read];
        queue->idx_read = (queue->idx_read+1)%queue->size;        
        ret = true;
    }
    else    queue->count_empty = _upperlimit(queue->count_empty+1,0xFFFF);
    return ret;
}


uint16_t Q_NumContents(Q_queue_t *queue){
    return (uint16_t)((queue->size + queue->idx_write - queue->idx_read)%(queue->size));    
}

uint16_t Q_NumEmptyBytes(Q_queue_t *queue){
    return queue->size - (int)Q_NumContents(queue) - 1;
}

uint16_t Q_GetFullCnt(Q_queue_t *queue){     
    return queue->count_full;
}

uint16_t Q_GetEmptyCnt(Q_queue_t *queue){         
    return queue->count_empty;
}

uint16_t Q_GetFrameSizeMovePtr(Q_queue_t *queue){
    uint16_t size=0;    
    if(Q_NumContents(queue) > 0){
        size = queue->data[queue->idx_read];
        if(size>0)
            queue->idx_read = (queue->idx_read+1)%queue->size;
    }
    return size;
}

uint16_t Q_GetFrameSizeOnlyRead(Q_queue_t *queue){
    return queue->data[queue->idx_read];
}

