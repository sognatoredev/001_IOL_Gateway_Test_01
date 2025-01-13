#include "MB_Interface.h"
#include "main.h"

extern UART_HandleTypeDef huart2;
extern uint8_t device_ProcessDataIn_Arr[];
extern uint8_t device_ProcessDataOut_Arr[];
extern uint8_t M2D_PDOut_Arr[];
extern uint8_t IOL_EventFlag;

MBI_Data_t MBI_Data;

// Hardware Interface

void MBI_UartRx(uint8_t *packet, uint8_t size){
    // IF: MBM_Receive(packet,size);
    printf("MBI_UartRx\r\n"); 
};

void MBI_Timer1ms(void){
    // IF: MBM_Handler1ms();
    printf("MBI_Timer1ms\r\n"); 
}

void MBI_UartTx(uint8_t *packet, uint8_t size){

    uint8_t *pData;
    uint8_t packet_size = 0;

    pData = packet;
    packet_size = size;

    HAL_UART_Transmit_IT(&huart2, pData, packet_size);
    // CSH: Uart로 송신할때 불려지는 루틴 
    printf("MBI_UartTx\r\n");
}     

// Modbus Master Interface

void MBI_Init(void){
    // CSH: Modbus 메모리 포인터에 IOLink 메모리 주소를 설정  
    MBI_Data.PD_M2D_1    = (uint16_t *)(&M2D_PDOut_Arr[2]);
    MBI_Data.PD_M2D_2    = (uint16_t *)(&M2D_PDOut_Arr[4]);
    MBI_Data.PD_D2M_1    = (uint16_t *)(&device_ProcessDataIn_Arr[2]);
    MBI_Data.PD_D2M_2    = (uint16_t *)(&device_ProcessDataIn_Arr[4]);
    // MBI_Data.ISDU_1      = (uint16_t *)(&device_ProcessDataIn_Arr[0]);
    // MBI_Data.ISDU_2      = (uint16_t *)(&device_ProcessDataIn_Arr[1]);
    MBI_Data.Event       = (uint16_t *)(&IOL_EventFlag);
    
    // IF: MBM_Start();
    printf("MBI_Init\r\n"); 
}

void MBI_IsduTxReq(uint16_t addr, uint16_t data){
    // IF: MBM_ServiceTxReq(addr, data);  
    printf("MBI_IsduTxReq\r\n"); 
}

void MBI_IsduRxReq(uint16_t addr){
    // IF: MBM_ServiceRxReq(addr);   
    printf("MBI_IsduRxReq\r\n"); 
}

void MBI_ErrorClr(uint16_t error_code){
    // IF: MBM_ErrorClr(error_code);    
    printf("MBI_ErrorClr\r\n"); 
}

void MBI_IsduTxCfm(uint16_t res){
    // CSH: ISDU 쓰기 처리 결과를 알려주는 함수
    printf("MBI_IsduTxCfm\r\n"); 
}

void MBI_IsduRxRes(uint16_t res, uint16_t data){
	// CSH: ISDU 읽기 처리 결과를 알려주는 함수, 
    printf("MBI_IsduRxRes\r\n"); 
}

void MBI_Error(uint16_t error_code){
    // CSH:S 모드 버스에서 오류를 감지하면 불려지는 함수, 
    printf("MBI_Error\r\n"); 
}

