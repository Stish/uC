//****************************************************************************
// @Module        General Purpose Timer Unit (GPT2)
// @Filename      GPT2.h
// @Project       can_test.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2267-96F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.2
//
// @Description   This file contains all function prototypes and macros for 
//                the GPT2 module.
//
//----------------------------------------------------------------------------
// @Date          26.09.2012 12:39:24
//
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,1)

// USER CODE END



#ifndef _GPT2_H_
#define _GPT2_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

//   This parameter identifies GPT2 timer 5
#define GPT2_TIMER_5 5

//   This parameter identifies GPT2 timer 6
#define GPT2_TIMER_6 6

// USER CODE BEGIN (GPT2_Header,4)

// USER CODE END


//****************************************************************************
// @Typedefs
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,5)

// USER CODE END


//****************************************************************************
// @Imported Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,6)

// USER CODE END


//****************************************************************************
// @Global Variables
//****************************************************************************

// USER CODE BEGIN (GPT2_Header,7)

// USER CODE END


//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************

void GPT2_vInit(void);

// USER CODE BEGIN (GPT2_Header,8)

// USER CODE END

//****************************************************************************
// @Interrupt Vectors
//****************************************************************************

#define T5INT 0x23

// USER CODE BEGIN (GPT2_Header,9)

// USER CODE END


#endif  // ifndef _GPT2_H_