//****************************************************************************
// @Module        General Purpose Timer Unit (GPT1)
// @Filename      GPT1.c
// @Project       can_test.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2267-96F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.2
//
// @Description   This file contains functions that use the GPT1 module.
//
//----------------------------------------------------------------------------
// @Date          26.09.2012 12:39:23
//
//****************************************************************************

// USER CODE BEGIN (GPT1_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.h"

// USER CODE BEGIN (GPT1_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT1_General,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (GPT1_General,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT1_General,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT1_General,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT1_General,7)

// USER CODE END


//****************************************************************************
// @External Prototypes
//****************************************************************************

// USER CODE BEGIN (GPT1_General,8)

// USER CODE END


//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

// USER CODE BEGIN (GPT1_General,9)

// USER CODE END


//****************************************************************************
// @Function      void GPT1_vInit(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the initialization function of the GPT1 function 
//                library. It is assumed that the SFRs used by this library 
//                are in reset state. 
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          26.09.2012
//
//****************************************************************************

// USER CODE BEGIN (Init,1)

// USER CODE END

void GPT1_vInit(void)
{
  // USER CODE BEGIN (Init,2)

  // USER CODE END
  ///  -----------------------------------------------------------------------
  ///  Configuration of Timer Block Prescaler 1:
  ///  -----------------------------------------------------------------------
  GPT12E_KSCCFG  =  0x0003;      // Module Enable

  _nop();  // one cycle delay 

  _nop();  // one cycle delay 



  ///  -----------------------------------------------------------------------
  ///  Configuration of Timer Block Prescaler 1:
  ///  -----------------------------------------------------------------------
  ///  - prescaler for timer block 1 is 8


  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Core Timer 3:
  ///  -----------------------------------------------------------------------
  ///  - timer 3 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is reset
  ///  - alternate output function T3OUT (P7.0) is disabled
  ///  - timer 3 output toggle latch (T3OTL) is set to 0
  ///  - timer 3 run bit is reset

  GPT12E_T3CON   =  0x0000;      // load timer 3 control register
  GPT12E_T3      =  0x0000;      // load timer 3 register
  ///  - prescaler for timer block 1 is 8

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Auxiliary Timer 2:
  ///  -----------------------------------------------------------------------
  ///  - timer 2 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 128
  ///  - up/down control bit is reset

  GPT12E_T2CON   =  0x0004;      // load timer 2 control register
  GPT12E_T2      =  0x0BDC;      // load timer 2 register
  ///  - prescaler for timer block 1 is 8

  ///  -----------------------------------------------------------------------
  ///  Configuration of the GPT1 Auxiliary Timer 4:
  ///  -----------------------------------------------------------------------
  ///  - timer 4 works in timer mode
  ///  - external up/down control is disabled
  ///  - prescaler factor is 8
  ///  - up/down control bit is reset
  ///  - timer 4 run bit is reset

  GPT12E_T4CON   =  0x0000;      // load timer 4 control register
  GPT12E_T4      =  0x0000;      // load timer 4 register
  ///  - prescaler for timer block 1 is 8

  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT1 Port Pins:
  ///  -----------------------------------------------------------------------


  ///  -----------------------------------------------------------------------
  ///  Configuration of the used GPT1 Interrupts:
  ///  -----------------------------------------------------------------------
  ///  timer 2 service request node configuration:
  ///  - timer 2 interrupt priority level (ILVL) = 15
  ///  - timer 2 interrupt group level (GLVL) = 0
  ///  - timer 2 group priority extension (GPX) = 0

  GPT12E_T2IC    =  0x007C;     

  ///  Use PEC channel 4 for GPT1 T2 INT:
  ///  - normal interrupt
  ///  - pointers are not modified
  ///  - transfer a word
  ///  - service End of PEC interrrupt by a EOP interrupt node is disabled
  ///  - channel link mode is disabled

  PECC4          =  0x0000;      // load PECC4 control register



  // USER CODE BEGIN (GPT1_Function,3)

  // USER CODE END

  GPT12E_T2CON_T2R  =  1;        // set timer 2 run bit

} //  End of function GPT1_viTmr4


//****************************************************************************
// @Function      void GPT1_viTmr2(void) 
//
//----------------------------------------------------------------------------
// @Description   This is the interrupt service routine for the GPT1 timer 2. 
//                It is called up in the case of over or underflow of the 
//                timer 2 register.
//                If the incremental interface mode is selected and the 
//                interrupt for this mode is not disabled it is called up if 
//                count edge or count direction was detected.
//                
//                Please note that you have to add application specific code 
//                to this function.
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    None
//
//----------------------------------------------------------------------------
// @Date          26.09.2012
//
//****************************************************************************

// USER CODE BEGIN (Tmr2,1)

// USER CODE END

_interrupt(T2INT)  void GPT1_viTmr2(void)
{
  // USER CODE BEGIN (Tmr2,2)

  // USER CODE END


  // USER CODE BEGIN (Tmr2,5)
#if BMC_ID == 1
	CAN_vLoadData(0, ucaCellVolts14);
	CAN_vLoadData(1, ucaCellVolts58);
	CAN_vTransmit(0);
	CAN_vTransmit(1);
	CAN_vLoadData(6, ucaBMCStatus);
	CAN_vTransmit(6);
#endif
#if BMC_ID == 2
	CAN_vLoadData(2, ucaCellVolts14);
	CAN_vLoadData(3, ucaCellVolts58);
	CAN_vTransmit(2);
	CAN_vTransmit(3);
	CAN_vLoadData(7, ucaBMCStatus);
	CAN_vTransmit(7);
#endif
#if BMC_ID == 3
	CAN_vLoadData(4, ucaCellVolts14);
	CAN_vLoadData(5, ucaCellVolts58);
	CAN_vTransmit(4);
	CAN_vTransmit(5);
	CAN_vLoadData(8, ucaBMCStatus);
	CAN_vTransmit(8);
#endif
	ucLTCEnableADC = 0x01;
  // USER CODE END

} //  End of function GPT1_viTmr2




// USER CODE BEGIN (GPT1_General,10)

// USER CODE END

