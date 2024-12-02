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

#define IOL_OP_ISDU_PD_LENGTH                   4
#define IOL_OP_ISDU_OD_LENGTH                   2
#define IOL_OP_ISDU_CKS_LENGTH                  1
#define IOL_OP_ISDU_PRODUCTNAME_LENGTH          23
#define IOL_OP_ISDU_SERIALNUMBER_LENGTH         10

#define IOL_OP_ISDU_IN_PROCESSDATALENGTH          (IOL_OP_ISDU_OD_LENGTH + IOL_OP_ISDU_PD_LENGTH + IOL_OP_ISDU_CKS_LENGTH)
#define IOL_OP_ISDU_OUT_PROCESSDATALENGTH         (IOL_OP_ISDU_PD_LENGTH + IOL_OP_ISDU_CKS_LENGTH)


#define IOL_ISDU_INDEX_DIRECTPARAMETER_PAGE1         
#define IOL_ISDU_INDEX_DIRECTPARAMETER_PAGE2                
#define IOL_ISDU_INDEX_SYSTEMCOMMAND      
#define IOL_ISDU_INDEX_DATASTORAGEINDEX                
#define IOL_ISDU_INDEX_DEVICE                
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
    
    uint8_t isdu_od[50];
    uint8_t isdu_od_cnt;
    uint8_t isdu_od_cplt;
} IOL_ISDUPacket_t;

typedef enum 
{
    IOL_Index_directParameter_page1 = 0,
    IOL_Index_directParameter_page2,
    IOL_Index_Systemcommand,
    IOL_Index_Datastorageindex,
    
    IOL_Index_DeviceAccessLocks = 12,
    IOL_Index_ProfileCharacteristic,
    IOL_Index_PDInputDescriptor,
    IOL_Index_PDOutputDescriptor,
    IOL_Index_VendorName,
    IOL_Index_VendorText,
    IOL_Index_ProductName,
    IOL_Index_ProductID,
    IOL_Index_ProductText,
    IOL_Index_SerialNumber,
    IOL_Index_HardwareRevision,
    IOL_Index_FirmwareRevision,
    IOL_Index_ApplicationSpecificTag,
    IOL_Index_FunctionTag,
    IOL_Index_LocationTag,
    IOL_Index_ProductURI,
    
    IOL_Index_ErrorCount = 32,
    
    IOL_Index_DeviceStatus = 36,
    IOL_Index_DetailedDeviceStatus,
    
    IOL_Index_ProcessDataInput = 40,
    IOL_Index_ProcessDataOutput
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
    IOL_Iservice_M_Wreq8bit = 0x10,
    IOL_Iservice_M_Wreq8bitsubindex,
    IOL_Iservice_M_Wreq16bitsubindex,
    IOL_Iservice_D_WresM,
    IOl_Iservice_D_WresP,

    IOL_Iservice_M_Rreq8bit = 0x90,
    IOL_Iservice_M_Rreq8bitsubindex,
    IOL_Iservice_M_Rreq16bitsubindex,
    IOL_Iservice_D_RresM,
    IOL_Iservice_D_RresP
} IOL_IserviceTable;

/* USER CODE BEGIN Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ IOL_ISDU_H__ */

