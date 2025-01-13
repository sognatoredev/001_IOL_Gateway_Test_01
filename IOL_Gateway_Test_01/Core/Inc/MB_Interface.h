#ifndef _MB_INTERFACE_H
#define _MB_INTERFACE_H

#include "MB_Common.h"
#include "MB_Master.h"

typedef struct{
    uint16_t *PD_M2D_1;
    uint16_t *PD_M2D_2;
    uint16_t *PD_D2M_1;
    uint16_t *PD_D2M_2;
    uint16_t *ISDU_1;
    uint16_t *ISDU_2;
    uint16_t *Event;
} MBI_Data_t;

extern MBI_Data_t MBI_Data;

extern void MBI_UartRx(uint8_t *packet, uint8_t size);	    // IDLE time으로 구분된 패킷을 받으면 호출해야하는 콜백
extern void MBI_Timer1ms(void); 				            // 1ms Timer 인터럽트에서 호출해야하는 콜백

extern void MBI_UartTx(uint8_t *packet, uint8_t size);      // Uart로 송신할때 불려지는 루틴 

extern void MBI_Init(void); 						        // Uart와 Timer가 준비되면 호출해야하는 함수 
extern void MBI_IsduTxReq(uint16_t addr, uint16_t data);	// ISDU로 쓰기 명령이 수신되면 호출해야하는 함수
extern void MBI_IsduRxReq(uint16_t addr);			        // ISDU로 읽기 명령이 수신되면 호출해야하는 함수
extern void MBI_ErrorClr(uint16_t error_code);			    // IOLink 마스터로 오류 해제 명령을 받으면 호출해야하는 함수

extern void MBI_IsduTxCfm(uint16_t res);			        // ISDU 쓰기 처리 결과를 알려주는 함수, 
                                                            // IsduRxReq 이후 100ms 이내에 반드시 불려 짐. 
                                                            // 그 결과를 마스터에게 전달되어야 함.
extern void MBI_IsduRxRes(uint16_t res, uint16_t data);	    // ISDU 읽기 처리 결과를 알려주는 함수, 
                                                            // IsduRxReq 이후 100ms 이내에 반드시 불려 짐. 
                                                            // 그 결과를 마스터에게 전달되어야 함.
extern void MBI_Error(uint16_t error_code);                 // 모드 버스에서 오류를 감지하면 불려지는 함수, 
                                                            // IOLink에서 오류를 마스터로 보내야함.


#endif
