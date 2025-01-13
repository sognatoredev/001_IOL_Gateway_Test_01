#ifndef _MB_MASTER_H
#define _MB_MASTER_H

#include "MB_Interface.h"

#define MBM_TIMEOUT_PROTOCOL    100    // ms
#define MBM_TIMEOUT_MONITOR     1000    // ms


typedef enum{
    DORMANT     = 0,    // 동작 불가
    MB_ERROR       = 1,    // 오류 감지
    READY       = 2,    // 동작 가능하지만 진행중인 사항 없음 
    PROCESS_TX  = 3,    // 그리퍼 제어 중
    PROCESS_RX  = 4,    // 주기적인 감시 중
    SERVICE_TX  = 5,    // 서비스 송신 처리 증
    SERVICE_RX  = 6,    // 서비스 수신 처리 중
} MBM_State_t;

typedef struct{
    bool pending;
    uint16_t addr;
    uint16_t data;
} MBM_Pending_t;

typedef struct{
    MBM_State_t state;
    MBM_Pending_t pending_ptx;
    MBM_Pending_t pending_stx;
    MBM_Pending_t pending_srx;
    
    uint16_t t_protocol_ms;     // 모드버스에 송신한 정보에 대한 응답을 기다리고 있는지 유무
                                // 0: 기다리지 않음, 1~MBM_TIMEOUT-1: 기다리는 중, MBM_TIMEOUT: 타임아웃 발생
    uint16_t t_monitor_ms;      // 주기적인 모니터링 주기
    uint16_t cmd_past;
    uint16_t err_code;
} MBM_Data_t;


extern void MBM_Start(void);
extern void MBM_Handler1ms(void);
extern void MBM_Receive(uint8_t *packet, uint8_t size);

extern void MBM_ServiceTxReq(uint16_t addr, uint16_t data);
extern void MBM_ServiceRxReq(uint16_t addr);
extern void MBM_ErrorClr(uint16_t error_code);

#endif
