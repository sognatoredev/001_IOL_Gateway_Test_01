
#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdint.h> 
#include <stdbool.h>

// #include "user.h"

#define _upperlimit(amt,high)           (((amt)>(high))?(high):(amt))

typedef enum{
    // DEBUG = 0,
    HOST = 1,
    MONITOR = 2
} Q_type_t;

typedef struct{
    uint16_t    size;
    int16_t     idx_write;
    int16_t     idx_read;
    uint16_t    count_full;
    uint16_t    count_empty;
    uint8_t     *data;    
} Q_queue_t;


void Q_Init(Q_queue_t *queue, uint8_t *data, uint16_t len);
void Q_Clear(Q_queue_t *queue);
bool Q_Write(Q_queue_t *queue, uint8_t *data, uint16_t len);
bool Q_Read(Q_queue_t *queue, uint8_t *data, uint16_t len);

bool Q_WriteWithLen(Q_queue_t *queue, uint8_t *data, uint16_t len);
bool Q_ReadByte(Q_queue_t *queue, uint8_t *data);

uint16_t    Q_NumContents(Q_queue_t *queue);
uint16_t    Q_NumEmptyBytes(Q_queue_t *queue);
uint16_t    Q_GetFullCnt(Q_queue_t *queue);
uint16_t    Q_GetEmptyCnt(Q_queue_t *queue);
uint16_t    Q_GetFrameSizeMovePtr(Q_queue_t *queue);
uint16_t    Q_GetFrameSizeOnlyRead(Q_queue_t *queue);



#endif 
