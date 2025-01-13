#include "MB_Master.h"
#include "MB_Core.h"

MBM_Data_t MBM_Data;

static bool MBM_ProtocolTimeout(void);

void MBM_Start(void){
    MBM_Data.pending_ptx.pending = false;
    MBM_Data.pending_stx.pending = false;
    MBM_Data.pending_srx.pending = false;
    MBM_Data.t_protocol_ms = 0;             // 프로토콜 타임 아웃 처리용도 
    MBM_Data.t_monitor_ms = 0;              // 주기적인 모니터링 용도
    MBM_Data.cmd_past = *(MBI_Data.PD_M2D_1);
    MBM_Data.err_code = 0;
    MBM_Data.state = READY;
}

void MBM_Handler1ms(void){
    uint16_t cmd;

    cmd = *(MBI_Data.PD_M2D_1); 

    if(MBM_Data.state >= READY){
    // HYY: interrupt disable

        // 프로토콜 타이머 종결 처리 
        if(MBM_Data.t_protocol_ms >= MBM_TIMEOUT_PROTOCOL){
            MBM_Data.t_protocol_ms = MBM_TIMEOUT_PROTOCOL;
            if(MBM_ProtocolTimeout()){
                MBM_Data.t_protocol_ms = 0;            
                MBM_Data.state = READY;                
            }
        }
        else if(MBM_Data.t_protocol_ms > 0){    // 처리중인 프로토콜이 없을 경우 타임아웃이 발생되지 않음
            MBM_Data.t_protocol_ms = MBM_Data.t_protocol_ms + 1;
        }
        
        // 팬딩된 프로토콜 처리
        if(MBM_Data.state == READY){
            if(MBM_Data.pending_ptx.pending){
                MBC_ProcessWriteReq(MBM_Data.pending_ptx.data);
                MBM_Data.t_protocol_ms = 1;
                MBM_Data.state = PROCESS_TX;
                MBM_Data.pending_ptx.pending = false;
            }
            else if(MBM_Data.pending_stx.pending){
                MBC_WriteReq(MBM_Data.pending_stx.addr, MBM_Data.pending_stx.data);
                MBM_Data.t_protocol_ms = 1;
                MBM_Data.state = SERVICE_TX;
                MBM_Data.pending_stx.pending = false;
            }
            else if(MBM_Data.pending_srx.pending){
                MBC_ReadReq(MBM_Data.pending_srx.addr);
                MBM_Data.t_protocol_ms = 1;
                MBM_Data.state = SERVICE_RX;
                MBM_Data.pending_srx.pending = false;
            }
        }

        // 그리퍼 동작명령 처리
        if((cmd != 0) && (MBM_Data.cmd_past == 0)){     // 그리퍼 동작명령 받음 
            // 오류 테스트를 위한 코드 HYY
            if(cmd == 4444){
                MBM_Data.state = MB_ERROR;
            }
            else{
                if(MBM_Data.state == READY){            // 그리퍼 동작명령 가능 상태
                    MBC_ProcessWriteReq(*(MBI_Data.PD_M2D_2));
                    MBM_Data.t_protocol_ms = 1;
                    MBM_Data.state = PROCESS_TX;
                }
                else{                                   // 그리퍼 동작명령 불가 상태 -> 팬딩
                    MBM_Data.pending_ptx.addr       = *(MBI_Data.PD_M2D_1);
                    MBM_Data.pending_ptx.data       = *(MBI_Data.PD_M2D_2);
                    MBM_Data.pending_ptx.pending    = true;
                }
            }

        }
        
        // 그리퍼 모니터링 처리        
        if(MBM_Data.t_monitor_ms >= MBM_TIMEOUT_MONITOR)
            MBM_Data.t_monitor_ms = MBM_TIMEOUT_MONITOR;
        else
            MBM_Data.t_monitor_ms = MBM_Data.t_monitor_ms + 1;

        if(MBM_Data.t_monitor_ms == MBM_TIMEOUT_MONITOR){
            if(MBM_Data.state == READY){
                MBC_ProcessReadReq();
                MBM_Data.t_protocol_ms = 1;
                MBM_Data.state = PROCESS_RX;
                MBM_Data.t_monitor_ms = 0;
            }
        }
            
    // HYY: interrupt enable        
    }
    *(MBI_Data.PD_M2D_1) = MBM_Data.state;
    MBM_Data.cmd_past = cmd;
}

void MBM_Receive(uint8_t *packet, uint8_t size){
    bool res;
    uint16_t data=0;    
    
    res = MBC_ParsePacket(packet, size, &data);    
    if(MBM_Data.state == PROCESS_RX){
        if(res){
            *(MBI_Data.PD_M2D_2) = data;
        }
    }  
    else if(MBM_Data.state == SERVICE_TX){
        if(res)     MBI_IsduTxCfm(1);
        else        MBI_IsduTxCfm(0);
    }  
    else if(MBM_Data.state == SERVICE_RX){
        if(res)     MBI_IsduRxRes(1,data);
        else        MBI_IsduRxRes(0,0);
    }  
    
    MBM_Data.t_protocol_ms = 0;
    MBM_Data.state = READY;
}

void MBM_ServiceTxReq(uint16_t addr, uint16_t data){
    if(MBM_Data.state >= READY){
    // HYY: interrupt disable
        if(MBM_Data.state == READY){        // 서비스 송신 가능 상태
            MBC_WriteReq(addr, data);
            MBM_Data.t_protocol_ms = 1;
            MBM_Data.state = SERVICE_TX;
        }
        else{                               // 서비스 송신 불가 상태
            MBM_Data.pending_stx.addr    = addr;
            MBM_Data.pending_stx.data    = data;
            MBM_Data.pending_stx.pending = true;
        }
    // HYY: interrupt enable    
    }
}

void MBM_ServiceRxReq(uint16_t addr){
    if(MBM_Data.state >= READY){
    // HYY: interrupt disable
        if(MBM_Data.state == READY){        // 서비스 수신 가능 상태
            MBC_ReadReq(addr);
            MBM_Data.t_protocol_ms = 1;
            MBM_Data.state = SERVICE_RX;
        }
        else{                               // 서비스 수신 불가 상태
            MBM_Data.pending_srx.addr    = addr;
            MBM_Data.pending_srx.pending = true;
        }
    // HYY: interrupt enable    
    }
}

void MBM_ErrorClr(uint16_t error_code){
    // HYY: interrupt disable
    if(MBM_Data.state == MB_ERROR){
        MBM_Start();
    }
    else{
        printf("Error Clear Exception. Not in Error State\r\n");    
    }
    // HYY: interrupt enable        
}


static bool MBM_ProtocolTimeout(void){
    if(MBM_Data.state == PROCESS_TX){
        // HYY: 프로세스 송신 오류 처리 
        return true;
    }
    else if(MBM_Data.state == PROCESS_RX){
        // HYY: 프로세스 수신 오류 처리
        return true;        
    }  
    else if(MBM_Data.state == SERVICE_TX){
        MBI_IsduTxCfm(0);
        return true;
    }  
    else if(MBM_Data.state == SERVICE_RX){
        MBI_IsduRxRes(0,0);        
        return true;
    }

    printf("Protocol TimeOut Exception. Not to be happend when normal cases\r\n");
    return false;
}
