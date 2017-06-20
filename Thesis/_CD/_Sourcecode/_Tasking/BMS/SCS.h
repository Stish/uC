//****************************************************************************
// @Module        Project Settings
// @Filename      SCS.h
// @Project       can_test.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2267-96F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.2
//
// @Description   This file is the header for the SCS driver
//             
//                Note:
//                This file should not be changed by 
//                the user
//
//----------------------------------------------------------------------------
// @Date          26.09.2012 12:39:22
//
//****************************************************************************

// USER CODE BEGIN (SCS_Header,1)

// USER CODE END



#ifndef _SCS_H_
#define _SCS_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (SCS_Header,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

// USER CODE BEGIN (SCS_Header,3)

// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// Notes: All frequencies are specified in [Hz].
// Crystal frequency at XTAL1/XTAL2 or direct input frequency at XTAL1
// given by hardware, 4000000...25000000
#define SCS_F_XTAL 8000000
// Target PLL frequency fPLL (<= 80000000)
// (Actual frequency: SCS_F_PLL, can be used e.g. for baud rate calculations)
#define SCS_F_PLL_TARGET 80000000
// Selection of wakeup oscillator frequency fWU (0...3), different from SCU spec!!!
// 0 = 560000, 1 = 200000, 2 = 300000, 3 = 150000
#define SCS_FREQSEL 3
// RAM code start/end address in flash for driver/trap code to be executed from PSRAM
// range: valid address in flash
// should be consistent with compiler options
#define SCS_RAMCODE_START_FLASH 0xC00200
#define SCS_RAMCODE_END_FLASH   0xC002FF
// RAM code start address in RAM for driver/trap code to be executed from PSRAM
// range: valid address in PSRAM
// should be consistent with compiler options
#define SCS_RAMCODE_START_RAM   0xE00200
// Vector segment number for flash for vectors that must be active before/after power down
// range: valid segment in flash
// should be consistent with compiler options
#define SCS_VECSEG_FLASH 0xC0
// Maximum PLL frequency step
#define SCS_F_MAXSTEP 20000000
// Maximum frequency change factor at CLKSEL multiplexer
#define SCS_FACTOR_CLKSEL_MAX 5
// K2 divider for PLL osc./VCO after boot SW execution
#define SCS_K2_BOOT (8)
// VCO frequency fVCO after boot SW execution
#define SCS_F_VCO_BOOT (5000000 * 16)
// Vector segment number for RAM
#define SCS_VECSEG_RAM 0xE0
// PLL frequency fPLL after boot SW execution
#define SCS_F_PLL_BOOT (SCS_F_VCO_BOOT/SCS_K2_BOOT)
// If not user defined:
// Number of XTAL clock periods for stable state after start
// only relevant if timer is used for XTAL clock counting
#ifndef SCS_CLOCKS_XTAL_START
#define SCS_CLOCKS_XTAL_START 1000
#endif
// If not user defined:
// VCO range for normal XTAL/VCO operation
#ifndef SCS_VCOSEL
#define SCS_VCOSEL 1
#endif
// Compiler related values *****************************************************
#  define NOP __nop


// Values for calculation ******************************************************

// If not user defined:
// Target VCO frequency fVCO for normal XTAL/VCO operation
#ifndef SCS_F_VCO_TARGET
#define SCS_F_VCO_TARGET 160000000
#endif
// If not user defined:
// Target frequency after P divider fP for normal XTAL/VCO operation
#ifndef SCS_F_P_TARGET
#define SCS_F_P_TARGET 16000000
#endif
// If not user defined:
// Calculate P divider for normal XTAL/VCO operation, round up
#ifndef SCS_P
#define SCS_P ((SCS_F_XTAL + SCS_F_P_TARGET - 1)/SCS_F_P_TARGET)
#endif
// Calculate frequency after P divider fP for normal XTAL/VCO operation, round down
#define SCS_F_P (SCS_F_XTAL/SCS_P)
// If not user defined:
// Calculate final K2 divider for normal XTAL/VCO operation, round down to avoid SCS_F_VCO > SCS_F_VCO_TARGET
#ifndef SCS_K2
#define SCS_K2 (SCS_F_VCO_TARGET/SCS_F_PLL_TARGET)
#endif
// If not user defined:
// Calculate N divider for normal XTAL/VCO operation, round to nearest; avoid SCS_F_VCO > SCS_F_VCO_TARGET
#ifndef SCS_N
#define SCS_N_TEMP ((SCS_F_PLL_TARGET * SCS_K2 + SCS_F_P/2)/SCS_F_P)
#  if SCS_F_P * SCS_N_TEMP > SCS_F_VCO_TARGET
#     define SCS_N (SCS_N_TEMP - 1)
#  else
#     define SCS_N SCS_N_TEMP
#  endif
#endif
// Calculate actual VCO frequency fVCO for normal XTAL/VCO operation
#define SCS_F_VCO (SCS_F_P * SCS_N)
// Calculate actual PLL frequency fPLL for normal XTAL/VCO operation
#define SCS_F_PLL (SCS_F_VCO/SCS_K2)
// If not user defined and if necessary:
// Calculate additional K2 divider step for switch from XTAL/K1 to XTAL/VCO, round up
#ifndef SCS_K2_1
#  if SCS_F_PLL - SCS_F_XTAL > SCS_F_MAXSTEP
#     define SCS_K2_1 ((SCS_F_VCO + SCS_F_XTAL + SCS_F_MAXSTEP - 1)/(SCS_F_XTAL + SCS_F_MAXSTEP))
#  endif
#endif
// If not user defined:
// Calculate additional K2 divider step for switch from XTAL/K1 to XTAL/VCO, round up, avoid previous K2 step
#ifndef SCS_K2_2
#  ifdef SCS_K2_1
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_1 > SCS_F_MAXSTEP
#       define SCS_K2_2_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_1 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_1 + SCS_F_MAXSTEP))
#       if  SCS_K2_2_TEMP < SCS_K2_1
#           define SCS_K2_2 SCS_K2_2_TEMP
#       endif
#    endif
#  endif
#endif
#if 0  // Disabled due to problem with compiler Tasking 8.6r1p3 and earlier
// If not user defined:
// Calculate additional K2 divider step for switch from XTAL/K1 to XTAL/VCO, round up, avoid previous K2 step
# ifndef SCS_K2_3
#  ifdef SCS_K2_2
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_2 > SCS_F_MAXSTEP
#       define SCS_K2_3_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_2 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_2 + SCS_F_MAXSTEP))
#       if  SCS_K2_3_TEMP < SCS_K2_2
#           define SCS_K2_3 SCS_K2_3_TEMP
#       endif
#    endif
#  endif
# endif
#endif
#if 0  // Disabled due to problem with compiler Tasking 8.6r1p3 and earlier
// If not user defined:
// Calculate additional K2 divider step for switch from XTAL/K1 to XTAL/VCO, round up, avoid previous K2 step
# ifndef SCS_K2_4
#  ifdef SCS_K2_3
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_3 > SCS_F_MAXSTEP
#       define SCS_K2_4_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_3 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_3 + SCS_F_MAXSTEP))
#       if  SCS_K2_4_TEMP < SCS_K2_3
#           define SCS_K2_4 SCS_K2_4_TEMP
#       endif
#    endif
#  endif
# endif
#endif

// Calculate PLL values for XTAL clock waiting time ///////////////////////////
// If not user defined and if necessary:
// Calculate K2 divider for XTAL clock waiting time, round down, avoid value >= boot SW value
#ifndef SCS_K2_WAIT
#  define SCS_K2_WAIT_TEMP (SCS_F_VCO_BOOT/(SCS_F_XTAL * 2))
#  if SCS_K2_WAIT_TEMP < SCS_K2_BOOT
#     define SCS_K2_WAIT SCS_K2_WAIT_TEMP
#  endif
#endif
// If not user defined and if necessary:
// Calculate additional K2 divider step for XTAL clock waiting time, round up
#ifndef SCS_K2_WAIT_1
#  ifdef SCS_K2_WAIT
#    if SCS_F_VCO_BOOT/SCS_K2_WAIT - SCS_F_PLL_BOOT > SCS_F_MAXSTEP
#       define SCS_K2_WAIT_1 ((SCS_F_VCO_BOOT + SCS_F_PLL_BOOT + SCS_F_MAXSTEP - 1)/(SCS_F_PLL_BOOT + SCS_F_MAXSTEP))
#    endif
#  endif
#endif
// If not user defined and if necessary:
// Calculate additional K2 divider step for XTAL clock waiting time, round up, avoid previous K2 step
#ifndef SCS_K2_WAIT_2
#  ifdef SCS_K2_WAIT_1
#    if SCS_F_VCO_BOOT/SCS_K2_WAIT - SCS_F_VCO_BOOT/SCS_K2_WAIT_1 > SCS_F_MAXSTEP
#       define SCS_K2_WAIT_2_TEMP ((SCS_F_VCO_BOOT + SCS_F_VCO_BOOT/SCS_K2_WAIT_1 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO_BOOT/SCS_K2_WAIT_1 + SCS_F_MAXSTEP))
#       if SCS_K2_WAIT_2_TEMP < SCS_K2_WAIT_1
#          define SCS_K2_WAIT_2 SCS_K2_WAIT_2_TEMP
#       endif
#    endif
#  endif
#endif
// If not user defined and if necessary:
// Calculate PWSEL
#ifndef SCS_PWSEL
#  if SCS_FREQSEL == 0
#     define SCS_PWSEL 0
#  elif SCS_FREQSEL == 1
#       define SCS_PWSEL 0
#  elif SCS_FREQSEL == 2
#       define SCS_PWSEL 0
#  else
#       define SCS_PWSEL 0
#  endif
#endif
// Calculate wakeup oscillator frequency fWU, different to SCU spec!!!
#  if SCS_FREQSEL == 0
#     define SCS_F_WU 560000
#  elif SCS_FREQSEL == 1
#       define SCS_F_WU 300000
#  elif SCS_FREQSEL == 2
#       define SCS_F_WU 200000
#  elif SCS_FREQSEL == 3
#       define SCS_F_WU 150000
#  endif
// If not user defined:
// Calculate additional K2 divider step for switch from wakeup oscillator to XTAL/VCO, round up
// Calculate actual frequency for switch from wakeup to VCO
#ifndef  SCS_K2_WU_1
#  if SCS_F_PLL > SCS_F_WU * SCS_FACTOR_CLKSEL_MAX
#     define SCS_K2_WU_1 ((SCS_F_VCO + (SCS_F_WU * SCS_FACTOR_CLKSEL_MAX) - 1)/(SCS_F_WU * SCS_FACTOR_CLKSEL_MAX))
#  endif
#endif
// If not user defined and if necessary:
// Calculate additional K2 divider step for switch from wakeup oscillator to XTAL/VCO, round up, avoid previous K2 step
#ifndef SCS_K2_WU_2
#  ifdef SCS_K2_WU_1
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_WU_1 > SCS_F_MAXSTEP
#       define SCS_K2_WU_2_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_WU_1 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_WU_1 + SCS_F_MAXSTEP))
#       if SCS_K2_WU_2_TEMP < SCS_K2_WU_1
#          define SCS_K2_WU_2 SCS_K2_WU_2_TEMP
#       endif
#    endif
#  endif
#endif
// Disabled due to problem with compiler Tasking 8.6r1p3 and earlier
// If not user defined:
// Calculate additional K2 divider step for switch from wakeup oscillator to XTAL/VCO, round up, avoid previous K2 step
#if 0
# ifndef SCS_K2_WU_3
#  ifdef SCS_K2_WU_2
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_WU_2 > SCS_F_MAXSTEP
#       define SCS_K2_WU_3_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_WU_2 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_WU_2 + SCS_F_MAXSTEP))
#       if  SCS_K2_WU_3_TEMP < SCS_K2_WU_2
#           define SCS_K2_WU_3 SCS_K2_WU_3_TEMP
#       endif
#    endif
#  endif
# endif
#endif
#if 0  // Disabled due to problem with compiler Tasking 8.6r1p3 and earlier
// If not user defined:
// Calculate additional K2 divider step 4 for switch from wakeup oscillator to XTAL/VCO, round up, avoid previous K2 step
# ifndef SCS_K2_WU_4
#  ifdef SCS_K2_WU_3
#    if SCS_F_PLL - SCS_F_VCO/SCS_K2_WU_3 > SCS_F_MAXSTEP
#       define SCS_K2_WU_4_TEMP ((SCS_F_VCO + SCS_F_VCO/SCS_K2_WU_3 + SCS_F_MAXSTEP - 1)/(SCS_F_VCO/SCS_K2_WU_3 + SCS_F_MAXSTEP))
#       if  SCS_K2_WU_4_TEMP < SCS_K2_WU_3
#           define SCS_K2_WU_4 SCS_K2_WU_4_TEMP
#       endif
#    endif
#  endif
# endif
#endif

// Wakeup osc. clocks needed for switch to HP bandgap, round down
// = required time[us] * fWU[Hz]/1e6
#define SCS_CLOCKS_WU_HP ((30 * SCS_F_WU)/1000000)
// ESR channel specification
#define SCS_ESR_NONE (0U)
#define SCS_ESR_CH0  (1U  <<0U)
#define SCS_ESR_CH1  (1U  <<1U)
#define SCS_ESR_CH2  (1U  <<2U)
//****************************************************************************
// @Typedefs
//****************************************************************************

// Type for EXTCLK MUX setting

typedef enum
{
  SCS_EXTCLK_F_SYS    = 0, // system frequency
  SCS_EXTCLK_F_OUT    = 1, // programmable output frequency
  SCS_EXTCLK_F_PLL    = 2, // PLL output frequency
  SCS_EXTCLK_F_OSC    = 3, // HP oscillator frequency
  SCS_EXTCLK_F_WU     = 4, // wakeup oscillator frequency
  SCS_EXTCLK_F_TCK    = 5, // test clock at Pin 2.9
  SCS_EXTCLK_F_OSC_FL = 6, // internal flash oscillator frequency
  SCS_EXTCLK_F_RTC    = 8  // real-time clock
} SCS_EXTCLK_Type;

//****************************************************************************
// @Imported Global Variables
//****************************************************************************

//****************************************************************************
// @Global Variables
//****************************************************************************

//****************************************************************************
// @Prototypes Of Global Functions
//****************************************************************************


void SCS_CountClocksWithTimer(SCS_EXTCLK_Type Source, unsigned int Clocks);
void SCS_StartXtalOsc(unsigned int UseTimer);
void SCS_SwitchSystemClock(unsigned int UseTimer);
void SCS_SwitchToHighPrecBandgap(void);


#endif  // ifndef _SCS_H_
