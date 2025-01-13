#ifndef _MB_CORE_H
#define _MB_CORE_H

#include "MB_Interface.h"

#define MB_POS_CMD 2
#define MB_SPD_CMD 4

#define MB_LIMIT_SPD_P  8
#define MB_GRIP_CMD     9
#define MB_ACTUAL_POS_P 10

#define MB_ID 1
#define MB_WRITE_SINGLE_REGISTER 0x06
#define MB_READ_HOLDING_REGISTER 0x03

extern void MBC_ProcessWriteReq(uint16_t data);
extern void MBC_ProcessReadReq(void);
extern void MBC_WriteReq(uint16_t addr,uint16_t data);
extern void MBC_ReadReq(uint16_t addr);
extern bool MBC_ParsePacket(uint8_t *packet, uint8_t size, uint16_t *data);

#endif
