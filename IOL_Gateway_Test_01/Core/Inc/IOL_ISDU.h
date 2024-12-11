/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    IOL_ISDU.h
  * @brief   This file contains all the function prototypes for
  *          the IOL_ISDU.c file
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IOL_ISDU_H__
#define __IOL_ISDU_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define IOL_OP_ISDU_PD_LENGTH                               4
#define IOL_OP_ISDU_OD_LENGTH                               2
#define IOL_OP_ISDU_CKS_LENGTH                              1
#define IOL_OP_ISDU_PRODUCTNAME_LENGTH                      13
#define IOL_OP_ISDU_SERIALNUMBER_LENGTH                     5
#define IOL_OP_ISDU_APPLICATIONSPECIFICTAG_LENGTH           18
#define IOL_OP_ISDU_PRODUCTID_LENGTH                        6
#define IOL_OP_ISDU_EXAMPLEPARAMETER_LENGTH                 2
#define IOL_OP_ISDU_DEVICESTATUS_LENGTH                     2
#define IOL_OP_ISDU_DETAILEDDEVICESTATUS_LENGTH             3
#define IOL_OP_ISDU_VENDORNAME_LENGTH                       34
#define IOL_OP_ISDU_HWREVISION_LENGTH                       34
#define IOL_OP_ISDU_FWREVISION_LENGTH                       34

#define IOL_OP_ISDU_IN_PROCESSDATALENGTH          (IOL_OP_ISDU_OD_LENGTH + IOL_OP_ISDU_PD_LENGTH + IOL_OP_ISDU_CKS_LENGTH)
#define IOL_OP_ISDU_OUT_PROCESSDATALENGTH         (IOL_OP_ISDU_PD_LENGTH + IOL_OP_ISDU_CKS_LENGTH)


#define IOL_ISDU_INDEX_DIRECTPARAMETER_PAGE1        0x0000
#define IOL_ISDU_INDEX_DIRECTPARAMETER_PAGE2        0x0001
#define IOL_ISDU_INDEX_SYSTEMCOMMAND                0x0002
#define IOL_ISDU_INDEX_DATASTORAGEINDEX             0x0003
#define IOL_ISDU_INDEX_DEVICE                       0x0004
#define IOL_ISDU_INDEX_PRODUCTNAME                  0x0005
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_PRODUCTNAME                
#define IOL_ISDU_INDEX_SERIALNUMBER             

#define IOL_ISDU_MASTERCOMMAND_PDOO             0x98
#define IOL_ISDU_MASTERCOMMAND_DO               0x99
#define IOL_ISDU_MASTERCOMMAND_DP               0x9A

typedef struct IOL_ISDUPacket
{
    uint8_t iservice;
    uint8_t length;
    uint8_t index;
    uint8_t subindex;
    uint8_t chkpdu;

    uint8_t *isdu_textid;
    
    uint8_t isdu_od[50];
    uint8_t isdu_od_cnt;
    uint8_t isdu_od_rxcplt;
    uint8_t isdu_od_writereq_flag;
    uint8_t isdu_od_writeReq8bit_flag;
    
} IOL_ISDUPacket_t;

// Index Look Up Table
typedef enum 
{
    IOL_Index_directParameter_page1 = 0,
    IOL_Index_directParameter_page2,    // 0x01
    IOL_Index_Systemcommand,            // 0x02
    IOL_Index_Datastorageindex,         // 0x03
    
    IOL_Index_DeviceAccessLocks = 12,   // 0x0C
    IOL_Index_ProfileCharacteristic,    // 0x0D
    IOL_Index_PDInputDescriptor,        // 0x0E
    IOL_Index_PDOutputDescriptor,       // 0x0F
    IOL_Index_VendorName,               // 0x10
    IOL_Index_VendorText,               // 0x11
    IOL_Index_ProductName,              // 0x12
    IOL_Index_ProductID,                // 0x13
    IOL_Index_ProductText,              // 0x14
    IOL_Index_SerialNumber,             // 0x15
    IOL_Index_HardwareRevision,         // 0x16
    IOL_Index_FirmwareRevision,         // 0x17
    IOL_Index_ApplicationSpecificTag,   // 0x18
    IOL_Index_FunctionTag,              // 0x19
    IOL_Index_LocationTag,              // 0x1A
    IOL_Index_ProductURI,               // 0x1B
    
    IOL_Index_ErrorCount = 32,          // 0x20
    
    IOL_Index_DeviceStatus = 36,        // 0x24
    IOL_Index_DetailedDeviceStatus,     // 0x25
    
    IOL_Index_ProcessDataInput = 40,    // 0x28
    IOL_Index_ProcessDataOutput,         // 0x29

    IOL_Index_PreferredIndex = 64       //0x40
} IOL_IndexTable;

typedef enum
{
    IOL_SC_ParamUploadStart = 1,
    IOL_SC_ParamUploadEnd,
    IOL_SC_ParamDownloadStart,
    IOL_SC_ParamDownloadEnd,
    IOL_SC_ParamDownloadStore,
    IOL_SC_ParamBreak,

    IOL_SC_DeviceReset = 128,
    IOL_SC_ApplicationRest,
    IOL_SC_RestoreFactorySettings,
    IOL_SC_BackToBox,

    IOL_VendorSpecific = 160
} IOL_SystemCommandTable;

typedef enum
{
    IOL_Iservice_M_Wreq8bit = 0x01,
    IOL_Iservice_M_Wreq8bitsubindex,
    IOL_Iservice_M_Wreq16bitsubindex,
    IOL_Iservice_D_WresM,
    IOl_Iservice_D_WresP,

    IOL_Iservice_M_Rreq8bit = 0x09,
    IOL_Iservice_M_Rreq8bitsubindex,
    IOL_Iservice_M_Rreq16bitsubindex,
    IOL_Iservice_D_RresM,
    IOL_Iservice_D_RresP
} IOL_IserviceTable;


extern IOL_IndexTable IOL_indextable;
extern uint8_t IOL_Get_ISDU_WR_ODArr (uint8_t * pData);
extern void IOL_Clear_PDBuffer (void);
/* USER CODE BEGIN Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ IOL_ISDU_H__ */

