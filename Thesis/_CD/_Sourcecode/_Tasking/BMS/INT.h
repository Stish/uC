//****************************************************************************
// @Module        Interrupt Controller (INT)
// @Filename      INT.h
// @Project       can_test.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2267-96F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the INT module.
//
//----------------------------------------------------------------------------
// @Date          26.09.2012 12:39:25
//
//****************************************************************************

// USER CODE BEGIN (INT_Header,1)

// USER CODE END



#ifndef _INT_H_
#define _INT_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (INT_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (INT_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

//   -------------------------------------------------------------------------
//   Defines for the parameter IntName
//   -------------------------------------------------------------------------

#define GPT1_T2_INT              GPT12E_T2IC_IE
#define GPT1_T3_INT              GPT12E_T3IC_IE
#define GPT1_T4_INT              GPT12E_T4IC_IE
#define GPT2_CAPREL_INT          GPT12E_CRIC_IE
#define GPT2_T5_INT              GPT12E_T5IC_IE
#define GPT2_T6_INT              GPT12E_T6IC_IE
#define U0C0_0IC_INT             U0C0_0IC_IE
#define U0C0_1IC_INT             U0C0_1IC_IE
#define U0C0_2IC_INT             U0C0_2IC_IE
#define U0C1_0IC_INT             U0C1_0IC_IE
#define U0C1_1IC_INT             U0C1_1IC_IE
#define U0C1_2IC_INT             U0C1_2IC_IE
#define U1C0_0IC_INT             U1C0_0IC_IE
#define U1C0_1IC_INT             U1C0_1IC_IE
#define U1C0_2IC_INT             U1C0_2IC_IE
#define U1C1_0IC_INT             U1C1_0IC_IE
#define U1C1_1IC_INT             U1C1_1IC_IE
#define U1C1_2IC_INT             U1C1_2IC_IE
#define U2C0_0IC_INT             U2C0_0IC_IE
#define U2C0_1IC_INT             U2C0_1IC_IE
#define U2C0_2IC_INT             U2C0_2IC_IE
#define U2C1_0IC_INT             U2C1_0IC_IE
#define U2C1_1IC_INT             U2C1_1IC_IE
#define U2C1_2IC_INT             U2C1_2IC_IE
#define EOP_INT                  EOPIC_EOPIE
#define RTC_INT                  RTC_IC_IE
#define ADC_INT_0                ADC_0IC_IE
#define ADC_INT_1                ADC_1IC_IE
#define ADC_INT_2                ADC_2IC_IE
#define ADC_INT_3                ADC_3IC_IE
#define ADC_INT_4                ADC_4IC_IE
#define ADC_INT_5                ADC_5IC_IE
#define ADC_INT_6                ADC_6IC_IE
#define ADC_INT_7                ADC_7IC_IE
#define CC2_T7_INT               CC2_T7IC_IE
#define CC2_T8_INT               CC2_T8IC_IE
#define CC2_CH16_INT             CC2_CC16IC_IE
#define CC2_CH17_INT             CC2_CC17IC_IE
#define CC2_CH18_INT             CC2_CC18IC_IE
#define CC2_CH19_INT             CC2_CC19IC_IE
#define CC2_CH20_INT             CC2_CC20IC_IE
#define CC2_CH21_INT             CC2_CC21IC_IE
#define CC2_CH22_INT             CC2_CC22IC_IE
#define CC2_CH23_INT             CC2_CC23IC_IE
#define CC2_CH24_INT             CC2_CC24IC_IE
#define CC2_CH25_INT             CC2_CC25IC_IE
#define CC2_CH26_INT             CC2_CC26IC_IE
#define CC2_CH27_INT             CC2_CC27IC_IE
#define CC2_CH28_INT             CC2_CC28IC_IE
#define CC2_CH29_INT             CC2_CC29IC_IE
#define CC2_CH30_INT             CC2_CC30IC_IE
#define CC2_CH31_INT             CC2_CC31IC_IE
#define CCU60_I0_INT             CCU60_0IC_IE
#define CCU60_I1_INT             CCU60_1IC_IE
#define CCU60_I2_INT             CCU60_2IC_IE
#define CCU60_I3_INT             CCU60_3IC_IE
#define CCU61_I0_INT             CCU61_0IC_IE
#define CCU61_I1_INT             CCU61_1IC_IE
#define CCU61_I2_INT             CCU61_2IC_IE
#define CCU61_I3_INT             CCU61_3IC_IE
#define CCU62_I0_INT             CCU62_0IC_IE
#define CCU62_I1_INT             CCU62_1IC_IE
#define CCU62_I2_INT             CCU62_2IC_IE
#define CCU62_I3_INT             CCU62_3IC_IE
#define CCU63_I0_INT             CCU63_0IC_IE
#define CCU63_I1_INT             CCU63_1IC_IE
#define CCU63_I2_INT             CCU63_2IC_IE
#define CCU63_I3_INT             CCU63_3IC_IE
#define WDT_INT0                 SCU_0IC_IE
#define WDT_INT1                 SCU_1IC_IE
#define CAN_INT_0                CAN_0IC_IE
#define CAN_INT_1                CAN_1IC_IE
#define CAN_INT_2                CAN_2IC_IE
#define CAN_INT_3                CAN_3IC_IE
#define CAN_INT_4                CAN_4IC_IE
#define CAN_INT_5                CAN_5IC_IE
#define CAN_INT_6                CAN_6IC_IE
#define CAN_INT_7                CAN_7IC_IE
#define CAN_INT_8                CAN_8IC_IE
#define CAN_INT_9                CAN_9IC_IE
#define CAN_INT_10               CAN_10IC_IE
#define CAN_INT_11               CAN_11IC_IE
#define CAN_INT_12               CAN_12IC_IE
#define CAN_INT_13               CAN_13IC_IE
#define CAN_INT_14               CAN_14IC_IE
#define CAN_INT_15               CAN_15IC_IE

// USER CODE BEGIN (INT_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (INT_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (INT_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (INT_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void INT_vInit(void);


// USER CODE BEGIN (INT_Header,8)

// USER CODE END


//****************************************************************************
// @Macro         INT_vEnableInterrupt(IntName) 
//
//----------------------------------------------------------------------------
// @Description   This macro enables the selected interrupt source.
//                Note:
//                Enable only those interrupt sources for which a service 
//                routine is installed.
//                Available definitions for IntName see in the header file
//
//----------------------------------------------------------------------------
// @Returnvalue   None
//
//----------------------------------------------------------------------------
// @Parameters    IntName: 
//                Name of Interrupt (see @Defines in the header file)
//
//----------------------------------------------------------------------------
// @Date          26.09.2012
//
//****************************************************************************

#define INT_vEnableInterrupt(IntName) IntName = 1


//****************************************************************************
// @Interrupt Vectors
//****************************************************************************



// USER CODE BEGIN (INT_Header,9)

// USER CODE END


#endif  // ifndef _INT_H_
