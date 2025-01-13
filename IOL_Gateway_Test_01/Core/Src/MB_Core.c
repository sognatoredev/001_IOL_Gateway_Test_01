#include "MB_Core.h"


static uint16_t MB_CrcCalc(uint8_t *packet, uint8_t size);


void MBC_ProcessWriteReq(uint16_t data){
    uint16_t cmd;
    cmd = MB_SPD_CMD;
    cmd = (cmd<<12) | data;
    MBC_WriteReq(MB_GRIP_CMD,cmd);
}

void MBC_ProcessReadReq(void){    
    MBC_ReadReq(MB_ACTUAL_POS_P);
}

void MBC_WriteReq(uint16_t addr,uint16_t data){
    uint8_t packet[10];
    uint16_t crc;

    packet[0] = MB_ID;
    packet[1] = MB_WRITE_SINGLE_REGISTER;
    packet[2] = (addr>>8)&0xFF;   // address (high byte)
    packet[3] = (addr)&0xFF;      // address (low byte)
    packet[4] = (data>>8)&0xFF;   // data (high byte)
    packet[5] = (data)&0xFF;      // data (low byte)

    crc = MB_CrcCalc(packet,6);
    packet[6] = (crc>>8)&0xFF;
    packet[7] = (crc)&0xFF;      

    MBI_UartTx(packet,8);      
}

void MBC_ReadReq(uint16_t addr){
    uint8_t packet[10];
    uint16_t crc;

    packet[0] = MB_ID;
    packet[1] = MB_READ_HOLDING_REGISTER;
    packet[2] = (addr>>8)&0xFF;   // address (high byte)
    packet[3] = (addr)&0xFF;      // address (low byte)
    packet[4] = 0;                // num of regs (high byte)
    packet[5] = 1;                // num of regs (low byte)

    crc = MB_CrcCalc(packet,6);
    packet[6] = (crc>>8)&0xFF;
    packet[7] = (crc)&0xFF;  

    MBI_UartTx(packet,8);      
}

bool MBC_ParsePacket(uint8_t *packet, uint8_t size, uint16_t *data){
    bool ret = false;
    uint16_t crc_packet, crc_calc;

    *data = ((packet[3]&0xFF)<<8) | packet[4];
    crc_packet = ((packet[5]&0xFF)<<8) | packet[6];
    crc_calc = MB_CrcCalc(packet,5);

    if(crc_packet == crc_calc)
        ret = true;
    
    return ret;
}


static uint16_t MB_CrcCalc(uint8_t *packet, uint8_t size){        
	uint16_t crc_val = (*packet) ^ 0xffff;
	int i, j = 0;
    for (i=0;i<size;i++) {
		if(i==0)	crc_val = (*packet) ^ 0xffff;
		else {
			packet++;
			crc_val = crc_val ^ (*packet);
		}
		for (j=0;j<8;j++) {
			if (crc_val & 0x0001) {
				crc_val = (crc_val>>1) ^ 0xA001;;
			}
			else {
				crc_val = crc_val >> 1;
			}
		}
	}
    return crc_val;
}