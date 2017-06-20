//****************************************************************************
// @Module        Project Settings
// @Filename      MAIN.h
// @Project       can_test.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2267-96F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the MAIN module.
//
//----------------------------------------------------------------------------
// @Date          26.09.2012 12:39:22
//
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,1)

// USER CODE END



#ifndef _MAIN_H_
#define _MAIN_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************
#define TASKING_VIPER

#define far  __far
#define _inline  inline
#define _nop  __nop
#define _interrupt  __interrupt


// USER CODE BEGIN (MAIN_Header,4)
#define NUM_OF_CELLS 5
#define BMC_ID 3
// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,7)
extern unsigned char test_data_msg[8];

extern unsigned char ucaCellVolts14[8];
extern unsigned char ucaCellVolts58[8];

extern unsigned char ucaBMCStatus[8];
/*
extern unsigned char ucaCellVolts_BMC1_14[8];
extern unsigned char ucaCellVolts_BMC1_58[8];
extern unsigned char ucaCellVolts_BMC2_14[8];
extern unsigned char ucaCellVolts_BMC2_58[8];
extern unsigned char ucaCellVolts_BMC3_14[8];
extern unsigned char ucaCellVolts_BMC3_58[8];*/
extern unsigned char ucSPIWaitCnt;
extern unsigned char ucLTCEnableADC;
extern unsigned char ucBalanceCell[8];
// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************


void MAIN_vUnlockProtecReg(void);

void MAIN_vLockProtecReg(void);

void MAIN_vChangeFreq(void);

// USER CODE BEGIN (MAIN_Header,8)

// USER CODE END


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

// USER CODE BEGIN (MAIN_Header,9)

// USER CODE END


//****************************************************************************
// @Project Includes
//****************************************************************************


#include  "XC22xxREGS.h"
#include  "SCS.h"

#include  "IO.h"
#include  "GPT1.h"
#include  "GPT2.h"
#include  "CAN.h"
#include  "INT.h"

// USER CODE BEGIN (MAIN_Header,10)
#include "LTC_Comm.h"
extern stCAN_SWObj canBMCCommands;
// USER CODE END


#endif  // ifndef _MAIN_H_
