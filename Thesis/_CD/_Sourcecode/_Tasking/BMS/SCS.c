//****************************************************************************
// @Module        Project Settings
// @Filename      SCS.c
// @Project       can_test.dav
//----------------------------------------------------------------------------
// @Controller    Infineon XC2267-96F80
//
// @Compiler      Tasking Viper
//
// @Codegenerator 2.2
//
// @Description   This file contains the SCS driver.
//                      
//                Note:
//                This file should not be changed by 
//                the user.
//
//----------------------------------------------------------------------------
// @Date          26.09.2012 12:39:22
//
//****************************************************************************

// USER CODE BEGIN (SCS_General,1)

// USER CODE END



//****************************************************************************
// @Project Includes
//****************************************************************************

#include "MAIN.h"

// USER CODE BEGIN (SCS_General,2)

// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************


//****************************************************************************
// @Defines
//****************************************************************************

// USER CODE BEGIN (SCS_General,3)

// USER CODE END


// Check frequency related values /////////////////////////////////////////////
// Check fP
#if SCS_F_P < 4000000 || SCS_F_P > 16000000
#   error  "SCS_F_P out of range"
#endif
// Check fVCO
#if SCS_VCOSEL == 0
#  if SCS_F_VCO < 48000000 || SCS_F_VCO > 112000000
#   error  "SCS_F_VCO out of range for VCOSEL = 0"
#  endif
#else
#  if SCS_F_VCO < 96000000 || SCS_F_VCO > 160000000
#   error  "SCS_F_VCO out of range for VCOSEL = 1"
#  endif
#endif
// Check P divider
#if SCS_P < 1 || SCS_P > 16
#   error  "SCS_P out of range"
#endif
// Check N divider
#if SCS_VCOSEL == 0
#  if SCS_N < 8 || SCS_N > 28
#   error  "SCS_N out of range for VCOSEL = 0"
#  endif
#else
#  if SCS_N < 16 || SCS_N > 40
#   error  "SCS_N out of range for VCOSEL = 1"
#  endif
#endif
// Check K2 divider and additional K2 divider steps if defined
#if SCS_K2 < 1 || SCS_K2 > 512
#   error  "SCS_K2 out of range"
#endif
#ifdef SCS_K2_1
#  if SCS_K2_1 < 1 || SCS_K2_1 > 512
#     error  "SCS_K2_1 out of range"
#  endif
#endif
#ifdef SCS_K2_2
#  if SCS_K2_2 < 1 || SCS_K2_2 > 512
#     error  "SCS_K2_2 out of range"
#  endif
#endif
#ifdef SCS_K2_3
#  if SCS_K2_3 < 1 || SCS_K2_3 > 512
#     error  "SCS_K2_3 out of range"
#  endif
#endif
#ifdef SCS_K2_4
#  if SCS_K2_4 < 1 || SCS_K2_4 > 512
#     error  "SCS_K2_4 out of range"
#  endif
#endif
// Check K2 K2 divider and additional K2 divider steps for switch to/from WU if defined
#if SCS_K2_WU_1 < 1 || SCS_K2_WU_1 > 512
#   error  "SCS_K2_WU_1 out of range"
#endif
#ifdef SCS_K2_WU_2
#  if SCS_K2_WU_2 < 1 || SCS_K2_WU_2 > 512
#     error  "SCS_K2_WU_2 out of range"
#  endif
#endif
#ifdef SCS_K2_WU_3
#  if SCS_K2_WU_3 < 1 || SCS_K2_WU_3 > 512
#     error  "SCS_K2_WU_3 out of range"
#  endif
#endif
#ifdef SCS_K2_WU_4
#  if SCS_K2_WU_4 < 1 || SCS_K2_WU_4 > 512
#     error  "SCS_K2_WU_4 out of range"
#  endif
#endif
// Check start clocks for XTAL
#if SCS_CLOCKS_XTAL_START < 1 || SCS_CLOCKS_XTAL_START > 65535
#   error  "SCS_CLOCKS_XTAL_START out of range"
#endif
// Check FREQSEL
#if SCS_FREQSEL < 0 || SCS_FREQSEL > 3
#   error  "SCS_FREQSEL out of range"
#endif
// Check PWSEL
#if SCS_PWSEL < 0 || SCS_PWSEL > 3
#   error  "SCS_PWSEL out of range"
#endif


//****************************************************************************
// @Typedefs
//****************************************************************************

// Type for CLKSEL setting

typedef enum
{
  SCS_CLKSEL_F_WU   = 0, // wakeup oscillator frequency
  SCS_CLKSEL_F_OSC  = 1, // HP oscillator frequency
  SCS_CLKSEL_F_PLL  = 2  // PLL frequency
} SCS_CLKSEL_Type;



//****************************************************************************
// @Imported Global Variables
//****************************************************************************



//****************************************************************************
// @Global Variables
//****************************************************************************



//****************************************************************************
// @External Prototypes
//****************************************************************************



//****************************************************************************
// @Prototypes Of Local Functions
//****************************************************************************

_inline void SetK2(unsigned int K2);
_inline void EnableVcoBypass(void);
_inline void DisableVcoBypass(void);
_inline void WaitForTimer13Stop(void);


//****************************************************************************
// @Local Functions
//****************************************************************************

//****************************************************************************;
// @Function      _inline void SetK2(unsigned int K2)
//
//-----------------------------------------------------------------------------
// @Description   Sets K2 divider and waits until PLL is locked.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    K2: K2 divider
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SetK2,1)

// USER CODE END

_inline void SetK2(unsigned int K2)
{
   // set K2DIV and enable K2 acknowledge
    SCU_PLLCON3 =
     ((K2-1U)<<0U)| // K2DIV K2-Divider Value
                     // 0..511 : K2-Divider = K2DIV + 1
     (1U    <<15U);  // K2ACK K2-Divider Ready Acknowledge
                     // Provides acknowledge to K2RDY
     // restart VCO lock detection
     SCU_PLLCON1_RESLD = 1;


     // wait until VCO lock status is OK
     while (!(SCU_PLLSTAT &
       ((0U   <<0U) | // VCOBYST VCO Bypass Status Freerunning/Normal (rh)
       (0U    <<1U) | // PWDSTAT Power-saving Mode Status (rh)
       (0U    <<2U) | // OSCSELST Oscillator Input Selection Status OSC_PLL (rh)
       (1U    <<3U) | // OVCOLOCK PLL VCO Lock Status (rh)
       (0U    <<4U) | // BY Bypass Mode Status (rh)
       (0U    <<5U) | // PRDY P-Divider Ready Status (rh)
       (0U    <<6U) | // NRDY N-Divider Ready Status (rh)
       (0U    <<7U) | // K1RDY K1-Divider Ready Status (rh)
       (0U    <<8U) | // K2RDY K1-Divider Ready Status (rh)
       (0U    <<9U) | // FINDIS Input Clock Disconnect Select Status (rh)
       (0U    <<10U)| // VCOL0 VCO Lock Detection Lost Status (falling edge VCOLOCK) (rh)
       (0U    <<11U)| // VCOL1 VCO Lock Detection Reached Status (rh)
       (0U    <<12U))))// REGSTAT PLL Power Regulator Status powered (operation possible) (rh)
     {
         // wait
         // in case MISRA-C check is activated you need to install a timeout loop here.
     }
} // end of function SetK2

//****************************************************************************;
// @Function      _inline void EnableVcoBypass(void)
//
//-----------------------------------------------------------------------------
// @Description   Enables VCO bypass and waits until bypass status is OK.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (EnableVcoBypass,1)

// USER CODE END

_inline void EnableVcoBypass(void)
{
   // enable VCO bypass
    SCU_PLLCON0_VCOBY = 1;

   // wait until VCO bypass status is entered
   while (SCU_PLLSTAT &
     ((1U   <<0U) | // VCOBYST VCO Bypass Status Freerunning/Normal (rh)
     (0U    <<1U) | // PWDSTAT Power-saving Mode Status (rh)
     (0U    <<2U) | // OSCSELST Oscillator Input Selection Status OSC_PLL (rh)
     (0U    <<3U) | // OVCOLOCK PLL VCO Lock Status (rh)
     (0U    <<4U) | // BY Bypass Mode Status (rh)
     (0U    <<5U) | // PRDY P-Divider Ready Status (rh)
     (0U    <<6U) | // NRDY N-Divider Ready Status (rh)
     (0U    <<7U) | // K1RDY K1-Divider Ready Status (rh)
     (0U    <<8U) | // K2RDY K1-Divider Ready Status (rh)
     (0U    <<9U) | // FINDIS Input Clock Disconnect Select Status (rh)
     (0U    <<10U)| // VCOL0 VCO Lock Detection Lost Status (falling edge VCOLOCK) (rh)
     (0U    <<11U)| // VCOL1 VCO Lock Detection Reached Status (rh)
     (0U    <<12U)))// REGSTAT PLL Power Regulator Status powered (operation possible) (rh)
     {
         // wait
         // in case MISRA-C check is activated you need to install a timeout loop here.
     }
} // end of function EnableVcoBypass

//****************************************************************************;
// @Function      _inline void DisableVcoBypass(void)
//
//-----------------------------------------------------------------------------
// @Description   Disables VCO bypass and waits until bypass status is OK.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (DisableVcoBypass,1)

// USER CODE END

_inline void DisableVcoBypass(void)
{
   // disable VCO bypass, don't touch other bits
    SCU_PLLCON0_VCOBY = 0;

   // wait until VCO bypass status is left
   while (!(SCU_PLLSTAT &
     ((1U   <<0U) | // VCOBYST VCO Bypass Status Freerunning/Normal (rh)
     (0U    <<1U) | // PWDSTAT Power-saving Mode Status (rh)
     (0U    <<2U) | // OSCSELST Oscillator Input Selection Status OSC_PLL (rh)
     (0U    <<3U) | // OVCOLOCK PLL VCO Lock Status (rh)
     (0U    <<4U) | // BY Bypass Mode Status (rh)
     (0U    <<5U) | // PRDY P-Divider Ready Status (rh)
     (0U    <<6U) | // NRDY N-Divider Ready Status (rh)
     (0U    <<7U) | // K1RDY K1-Divider Ready Status (rh)
     (0U    <<8U) | // K2RDY K1-Divider Ready Status (rh)
     (0U    <<9U) | // FINDIS Input Clock Disconnect Select Status (rh)
     (0U    <<10U)| // VCOL0 VCO Lock Detection Lost Status (falling edge VCOLOCK) (rh)
     (0U    <<11U)| // VCOL1 VCO Lock Detection Reached Status (rh)
     (0U    <<12U))))// REGSTAT PLL Power Regulator Status powered (operation possible) (rh)
     {
         // wait
         // in case MISRA-C check is activated you need to install a timeout loop here.
     }
} // end of function DisableVcoBypass

//****************************************************************************;
// @Function      _inline void WaitForTimer13Stop(void)
//
//-----------------------------------------------------------------------------
// @Description   This function waits until CCU6 timer T13 has stopped.
//                Notes:
//                - Function StartT13 should have been called before.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (WaitForTimer13Stop,1)

// USER CODE END

_inline void WaitForTimer13Stop(void)
{
   // wait until Timer 13 is stopped 
   while (CCU60_TCTR0 &
     ((0U    <<4U) | // T12R T12 Run Bit (rh)
      (0U    <<5U) | // STE12 T12 Shadow Transfer Enable (rh)
      (0U    <<6U) | // CDIR T12 Count Direction (rh)
      (1U    <<12U)| // T13R T13 Run Bit (rh)
      (0U    <<13U)))// STE13 T13 Shadow Transfer Enable (rh)
     {
         // wait
         // in case MISRA-C check is activated you need to install a timeout loop here.
     }
} // end of function WaitForTimer13Stop

//****************************************************************************;
// @Function      void SCS_StartXtalOsc(unsigned int UseTimer)
//
//-----------------------------------------------------------------------------
// @Description   This function starts the crystal oscillator at XTAL1 and
//                XTAL2 or enables the external oscillator input at XTAL1.
//                If specified, the function enables and starts CCU6 timer T13
//                and sets EXTCLK for counting XTAL clocks; if required, PLL 
//                frequency will be increased for synchronization purposes.
//                
//                Notes:
//                - If UseTimer is set, the function will enable CCU6 and will
//                  overwrite previous settings for T13; additionally, it will
//                  overwrite previous EXTCLK clock multiplexer settings.
//                - The user is responsible for disabling the register
//                protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    UseTimer: 0 = no XTAL clock counting, >0 = XTAL clock counting
//                via T13
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_StartXtalOsc,1)

// USER CODE END

void SCS_StartXtalOsc(unsigned int UseTimer)
{
   // enable HP oscillator with crystal or external clock
   SCU_HPOSCCON =
     (0U    <<1U) | // OSCWDTRST Oscillator Watchdog Reset
                    // 0 : Osc. watchdog is not reset and remains active
                    // 1 : Osc. watchdog is reset and restarted
     (0U    <<2U) | // MODE Oscillator Mode
                    // 0 : External crystal/clock, no osc. power-saving mode
                    // 1 : OSC_HP disabled, no osc. power-saving mode
                    // 2 : External clock, osc. power-saving mode
                    // 3 : OSC_HP disabled, osc. power-saving mode
     (0U    <<7U) | // X1DEN XTAL1 Data Enable
                    // 0 : 0 Bit X1D is not updated
                    // 1 : 1 Bit X1D can be updated
     (0U    <<8U) | // SHBY Shaper Bypass
                    // 0 : 0 Shaper is not bypassed
                    // 1 : 1 Shaper is bypassed
     (0U    <<9U) | // EMCLKEN OSCWDT Emergency System Clock Source Select Enable
                    // 0 : MCM controlled by SYSCON0.CLKSEL in OSCWDT emergency case
                    // 1 : MCM controlled by SYSCON0.EMCLKSEL in OSCWDT emergency case
     (0U    <<10U); // EMFINDISEN Emergency Input Clock Disconnect Enable
                    // 0 : PLLSTAT.FINDIS not updated in OSCWDT emergency case
                    // 1 : PLLSTAT.FINDIS is set in OSCWDT emergency case
   if (UseTimer)
   {
      #ifdef SCS_K2_WAIT
         // if system frequency < 2 * fXTAL:
      #  ifdef SCS_K2_WAIT_1
         //  set additional K2 divider step(s) if needed
         SetK2((unsigned int)SCS_K2_WAIT_1);
      #    ifdef SCS_K2_WAIT_2
             SetK2((unsigned int)SCS_K2_WAIT_2);
      #    endif // SCS_K2_WAIT_2
      #  endif // SCS_K2_WAIT_1
         // set final K2 divider for XTAL waiting time
         SetK2((unsigned int)SCS_K2_WAIT);
      #endif // SCS_K2_WAIT
      // Count SCS_CLOCKS_XTAL_START HP oscillator clocks
      SCS_CountClocksWithTimer(SCS_EXTCLK_F_OSC, SCS_CLOCKS_XTAL_START);
   }

} // end of function SCS_StartXtalOsc

//****************************************************************************;
// @Function      void SCS_SwitchSystemClock(unsigned int UseTimer)
//
//-----------------------------------------------------------------------------
// @Description   This function switches the system clock from its state after
//                boot program execution (internal oscillator and VCO usage) to
//                XTAL clock (crystal or external oscillator) and VCO usage.
//                If specified, the function waits for a certain number of XTAL
//                clocks before switching to this clock.
//                
//                Notes:
//                - It is assumed that the HP oscillator has been started.
//                - UseTimer > 0 should be used only if SCS_StartXtal() with
//                  parameter UseTimer > 0 has been called before.
//                - If UseTimer = 0, it must be guaranteed that XTAL
//                clock is stable.
//                - The user is responsible for disabling the register
//                protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    UseTimer: 0 = no XTAL clock counting, > 0 = XTAL clock counting
//                via T13
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_SwitchSystemClock,1)

// USER CODE END

void SCS_SwitchSystemClock(unsigned int UseTimer)
{
   if (UseTimer)
   {
      // wait until T13 is stopped
      WaitForTimer13Stop();

      #ifdef SCS_K2_WAIT
         // if system frequency was increased by using SCS_K2_WAIT:
      #  ifdef SCS_K2_WAIT_1
         // set additional K2 divider step(s) SCS_K2_WAIT_<x> if needed
      #    ifdef SCS_K2_WAIT_2
             SetK2((unsigned int)SCS_K2_WAIT_2);
      #     endif // SCS_K2_WAIT_2
           SetK2((unsigned int)SCS_K2_WAIT_1);
      #  endif // SCS_K2_WAIT_1
         // set original K2 value SCS_K2_BOOT
           SetK2((unsigned int)SCS_K2_BOOT);
      #endif // SCS_K2_WAIT
   }
   // set K1 divider = 1, enable K1 acknowledge  
   SCU_PLLCON2 =
     (0U    <<0U) | // K1DIV K1-Divider Value
                    // 0..511 : K1-Divider = K1DIV + 1
     (1U    <<15U); // K1ACK K1-Divider Ready Acknowledge
                    // Provides acknowledge to K1RDY
   EnableVcoBypass();
   // switch to PLL external oscillator, set P divider SCS_P and enable P acknowledge
   SCU_PLLCON1 = 
     (0U    <<0U) | // PLLPWD PLL Power Saving Mode
                    // 0 : Normal behavior
                    // 1 : PLL block is put into power saving mode
     (0U    <<1U) | // OSCSEL Oscillator Input Selection
                    // 0 : PLL external oscillator
                    // 1 : PLL internal oscillator
     (0U    <<2U) | // RESLD Restart VCO Lock Detection (w)
                    // Resets PLLSTAT.VCOLOCK and restarts VCO lock detection
     (0U    <<3U) | // AOSCSEL Asynchronous Oscillator Input Selection
                    // 0 : PLL external oscillator is selected asynchronously
                    // 1 : PLL internal oscillator is selected asynchronously
     (0U    <<5U) | // EMCLKEN VCOLCK Emergency System Clock Source Select Enable
                    // 0 : MCM controlled by SYSCON0.CLKSEL in VCOLCK emergency case
                    // 1 : MCM controlled by SYSCON0.EMCLKSEL in VCOLCK emergency case
     (0U    <<6U) | // EMFINDISEN Emergency Input Clock Disconnect Enable
                    // 0 : PLLSTAT.FINDIS not updated in VCOLCK emergency case
                    // 1 : PLLSTAT.FINDIS is set in VCOLCK emergency case
     (((unsigned int)(SCS_P) - 1U)
            <<8U) | // PDIV P-Divider Value
                    // 0..15 : P-Divider = PDIV + 1
     (1U    <<15U); // PACK P-Divider Ready Acknowledge
                    // Provides acknowledge to PRDY

      // keep VCO bypass, set SCS_VCOSEL, set N divider SCS_N and enable N acknowledge
     SCU_PLLCON0 = 
       (1U    <<0U) | // VCOBY Bypass
                      // 0 : Normal operation, VCO is not bypassed
                      // 1 : Prescaler Mode; VCO is bypassed
       (0U    <<1U) | // VCOPWD VCO Power Saving Mode
                      // 0 : Normal behavior
                      // 1 : VCO is put into power saving mode
       ((unsigned int)(SCS_VCOSEL)
              <<2U) | // VCOSEL VCO Range Select
                      // 0 : 48...112 MHz
                      // 1 : 96...160 MHz
       (0U    <<4U) | // REGENCLR PLL Power Regulator Enable Clear (w)
                      // 0 : Configuration for PLL power regulator  unchanged
                      // 1 : PLL is not powered (no operation possible)
       (0U    <<5U) | // REGENSET PLL PLL Power Regulator Enable Set (w)
                      // 0 : Configuration for PLL power regulator unchanged
                      // 1 : PLL is powered (operation possible)
       (((unsigned int)(SCS_N) - 1U)
             <<8U) |  // NDIV N-Divider Value
                      // 0..63 : N-Divider = NDIV + 1
       (1U    <<15U); // NACK N-Divider Ready Acknowledge
                      // Provides acknowledge to NRDY
   #ifdef SCS_K2_1
       // if additional frequency steps are required:
       // Set 1st additional K2 divider step SCS_K2_1
          SetK2((unsigned int)SCS_K2_1);

          DisableVcoBypass();

      // set additional K2 divider steps SCS_K2_<x> if needed
  #  ifdef SCS_K2_2
     SetK2((unsigned int)SCS_K2_2);
  #    ifdef SCS_K2_3
     SetK2((unsigned int)SCS_K2_3);
  #    endif // SCS_K2_3
  #  endif // SCS_K2_2

    // set final K2 divider SCS_K2
    SetK2((unsigned int)SCS_K2);

  #else
    // no additional frequency steps required:
    // set final K2 divider
    SetK2((unsigned int)SCS_K2);
    DisableVcoBypass();
  #endif // SCS_K2_1

  // Reset all the CCU60 registers used in SCS_CountClocksWithTimer().
    // reset port input select register high
    CCU60_PISELH = 0x0000;
    // reset timer control register 4
    CCU60_TCTR4 = 0x0000;
    // reset timer control register 2
    CCU60_TCTR2 = 0x0000;
    // reset timer 13 period register
    CCU60_T13PR = 0x0000;
    // reset kernel state configuration register
    CCU60_KSCFG  = 0x0002;

} // end of function SCS_SwitchSystemClock


//****************************************************************************;
// @Function      void SCS_SwitchToHighPrecBandgap(void)
//
//-----------------------------------------------------------------------------
// @Description   This function switches to high precision bandgap which is
//                required e. g. for flash operation.
//
//                Notes: 
//                - It is assumed that high precision bandgap has been enabled
//                  a sufficient time ago.
//                - The user is responsible for disabling the register
//                  protection.
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters    None
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_SwitchToHighPrecBandgap,1)

// USER CODE END

void SCS_SwitchToHighPrecBandgap(void)
{
   // set DMP1 = 1,5 V with high precision bandgap selected in STEP0
    SCU_STEP0 &=
    ~((0U    <<0U) | // VM DMP_M Voltage Configuration
                     // 0 : 1.5 V with high precision bandgap selected
                     // 1 : 1.0 V with low power bandgap selected
                     // 2 : 1.2 V with low power bandgap selected
                     // 3 : 1.5 V with low power bandgap selected
                     // 4..7 : 0 V
    (7U    <<3U) |   // V1 DMP_1 Voltage Configuration
                     // 0 : 1.5 V with high precision bandgap selected
                     //     keep setting for core-core isolation and for DMP_1 clock
                     // 1 : 1.0 V with low power bandgap selected
                     //     keep setting for core-core isolation and for DMP_1 clock
                     // 2 : 1.2 V with low power bandgap selected
                     //     keep setting for core-core isolation and for DMP_1 clock
                     // 3 : 1.5 V with low power bandgap selected
                     //     keep setting for core-core isolation and for DMP_1 clock
                     // 4 : 0 V; core-core isolation is active, DMP_1 clock is disabled
                     // 5 : Configuration is unchanged; core-core isolation is inactive
                     // 6 : Configuration is unchanged; DMP_1 clock is enabled
                     // 7 : Configuration is unchanged; DMP_1 clock is disabled
    (0U    <<6U) | // CLKENM System Clock Enable for DMP_M until next step
    (0U    <<7U) | // CLKEN1 System Clock Enable for DMP_1 until next step
    (0U    <<8U) | // TRGSEL Trigger Selection (for continuation)
                   // 0 : None of the signals
                   // 1 : OK 1 from PVC_M
                   // 2 : OK 2 from PVC_M
                   // 3 : OK 1 from PVC_M AND OK 2 from PVC_M
                   // 4 : OK 1 from PVC_1
                   // 5 : OK 1 from PVC_M AND OK 1 from PVC_1
                   // 6 : OK 2 from PVC_M AND OK 1 from PVC_1
                   // 7 : OK 1 from PVC_M AND OK 2 from PVC_M AND OK 1 from PVC_1
                   // 8 : OK 2 from PVC_1
                   // 9 : OK 1 from PVC_M AND OK 2 from PVC_1
                   // 10: OK 2 from PVC_M AND OK 2 from PVC_1
                   // 11: OK 1 from PVC_M AND OK 2 from PVC_M AND OK 2 from PVC_1
                   // 12: OK 1 from PVC_1 AND OK 2 from PVC_1
                   // 13: OK 1 from PVC_M AND OK 1 from PVC_1 AND OK 2 from PVC_1
                   // 14: OK 2 from PVC_M AND OK 1 from PVC_1 AND OK2 from PVC_1
                   // 15: OK 1 from PVC_M AND OK 2 from PVC_M AND OK 1 from PVC_1 AND OK2 from PVC_1
    (0U    <<12U)| // SYSDIV System Clock Divider
                   // 0 : The sequence is continued after 1 fSYS cycles
                   // 1 : The sequence is continued after 64 fSYS cycles
    (0U    <<13U)| // PVCMOFF PVC_M No valid results for this step
    (0U    <<14U)| // PVC1OFF PVC_1 No valid results for this step
    (0U    <<15U));// Read as 1, should be written with 1

} // end of function SCS_SwitchToHighPrecBandgap

//****************************************************************************;
// @Function      void SCS_CountClocksWithTimer (SCS_EXTCLK_Type Source,
//                unsigned int Clocks)
//-----------------------------------------------------------------------------
// @Description   This function enables and starts CCU6 timer T13 for counting
//                a certain number of ticks at EXTCLK. After that time, T13
//                will stop.
//
//                Notes:
//                - The function will enable CCU6 and will overwrite previous
//                  settings for T13; additionally, it will overwrite previous
//                  EXTCLK clock multiplexer settings.
//                - System frequency must be >= 2 * clock frequency.
//                - The user is responsible for disabling the register
//                  protection.
//
//-----------------------------------------------------------------------------
// @Returnvalue   None
//
//-----------------------------------------------------------------------------
// @Parameters     Source: The following clock source options are possible:
//                         SCS_EXTCLK_F_PLL     PLL output frequency
//                         SCS_EXTCLK_F_OSC     HP oscillator frequency
//                         SCS_EXTCLK_F_WU      wakeup oscillator frequency
//                         SCS_EXTCLK_F_TCK     test clock at Pin 2.9
//                         SCS_EXTCLK_F_OSC_FL  internal flash oscillator frequency
//                         SCS_EXTCLK_F_RTC     real-time clock
//
//                 Clocks: Number of clocks to count, 1...65535
//
//-----------------------------------------------------------------------------
// @Date          26.09.2012
//
//-----------------------------------------------------------------------------

// USER CODE BEGIN (SCS_CountClocksWithTimer,1)

// USER CODE END

void SCS_CountClocksWithTimer(SCS_EXTCLK_Type Source, unsigned int Clocks)
{
   volatile unsigned int Work;

   // enable CC6 module
   CCU60_KSCFG |=
     (1U    <<0U) | // MODEN Module Enable
     (1U    <<1U) | // BPMODEN Bit Protection for MODEN, set to 1 for change (w)
     (0U    <<4U) | // NOMCFG Normal Operation Mode Configuration
                    // kernel mode applied in normal operation mode
     (0U    <<7U) | // BPNOM Bit Protection for NOMCFG, set to 1 for change (w)
     (0U    <<8U) | // SUMCFG Suspend Mode Configuration
                    // Kernel mode applied in suspend mode
     (0U    <<11U)| // BPSUM Bit Protection for SUMCFG, set to 1 for change (w)
     (0U    <<12U)| // COMCFG Clock Off Mode Configuration
                    // kernel mode applied in clock off mode
     (0U    <<15U); // BPCOM Bit Protection for COMCFG, set to 1 for change (w)
   // read SFR back to avoid pipeline effects
   Work = CCU60_KSCFG;
   // enable T13 functionality
   CCU60_MCFG |=
     (0U    <<0U) | // T12 T12 Available
     (1U    <<1U) | // T13 T13 Available
     (0U    <<2U);  // MCM Multi-Channel Mode Available
 
   // stop T13, clear T13
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w)
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (1U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (0U    <<9U) | // T13RS Timer 13 Run Set (w)
     (1U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (1U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

   // set T13 period
   CCU60_T13PR = Clocks - 1;

   // enable T13 shadow transfer for period setting
   CCU60_TCTR4 =
     (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
     (0U    <<1U) | // T12RS Timer 12 Run Set (w)
     (0U    <<2U) | // T12RES Timer 12 Reset (w)
     (0U    <<3U) | // DTRES Dead-Time Counter Reset (w)
     (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
     (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
     (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
     (0U    <<8U) | // T13RR Timer 13 Run Reset (w)
     (0U    <<9U) | // T13RS Timer 13 Run Set (w)
     (0U    <<10U)| // T13RES Timer 13 Reset (w)
     (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
     (1U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
     (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)

   // enable T13 single-shot, clear other T13 bits, leave T12 bits unchanged
   CCU60_TCTR2 =
    (0U    <<0U) | // T12SSC T12 Single Shot Control
    (1U    <<1U) | // T13SSC T13 Single Shot Control
    (0U    <<2U) | // T13TEC T13 Trigger Event Control to start T13
                   // 0: No action
                   // 1: T13R set on T12 compare event on channel 0
                   // 2: T13R set on T12 compare event on channel 1
                   // 3: T13R set on T12 compare event on channel 2
                   // 4: T13R set on any T12 compare event (ch. 0, 1, 2)
                   // 5: T13R set on period-match of T12
                   // 6: T13R set on zero-match of T12 (while counting up)
                   // 7: Any edge of inputs CCPOSx
    (0U    <<5U) | // T13TED T13 Trigger Event Direction for T13TEC trigger
                   // 0: Reserved, no action
                   // 1: While T12 is counting up
                   // 2: While T12 is counting down
                   // 3: Independent on the count direction of T12
    (0U    <<8U) | // T12RSEL T12 External Run Selection
                   // 0: External setting of T12R disabled
                   // 1: T12R set on rising edge of T12HR
                   // 2: T12R set on falling edge of T12HR
                   // 3: T12R set on any edge of T12HR
    (0U    <<10U)  // T13RSEL T13 External Run Selection
                   // 0: External setting of T13R is disabled
                   // 1: T13R set on rising edge of T13HR
                   // 2: T13R set on falling edge of T13HR
                   // 3: T13R set on any edge of T13HR
    | (CCU60_TCTR2 & ((1U <<0U) | (3U <<8U)));

   // enable T13HRA input (rising edge), leave T12 bits unchanged
    CCU60_PISELH =
     (0U    <<0U) | // IST13HR T13 Input Select for T13HR
                    // 0: T13HRA, 1: T13HRB, 2: T13HRC, 3: T13HRD
     (0U    <<2U) | // ISCNT12 T12 Input Select for Counting Input
                    // 0: T12 prescaler, 1: TCTR4.CNT12 written with 1,
                    // 2: Rising edge on T12HR signal, 3: Falling edge on T12HR signal
     (2U    <<4U)   // ISCNT13 T13 Input Select for Counting Input
                    // 0: T13 prescaler, 1: TCTR4.CNT13 written with 1,
                    // 2: Rising edge on T13HR signal, 3: Falling edge on T13HR signal
     | (CCU60_PISELH & (3U <<2U));

   // enable EXTCLK = fOSC, fOUT settings = 0
   SCU_EXTCON =
      (1U    <<0U) | // EN External Clock Enable
                     // 0 : No external clock signal is provided
                     // 1 : The configured external clock signal is provided
      ((unsigned int)Source
             <<1U) | // SEL External Clock Select
                     // 0 : fSYS is selected
                     // 1 : fOUT is selected
                     // 2 : fPLL is selected
                     // 3 : fOSC is selected
                     // 4 : fWU is selected
                     // 5 : fTCK is selected
                     // 6 : fOSC_FL (flash module oscillator) is selected
                     // 8 : fRTC is selected
      (0U    <<8U) | // FORV Frequency Output Reload Value
                     // Copied to FOCNT upon each underflow of FOCNT
      (0U    <<14U)| // FOSS Frequency Output Signal Select
                     // 0 : Output of the toggle latch
                     // 1 : Output of the reload counter
      (0U    <<15U); // FOEN Frequency Output Enable
                     // 0 : Signal fOUT stops when fOUT is/becomes low
                     // 1 : FOCNT is running

    // start T13
    CCU60_TCTR4 =
      (0U    <<0U) | // T12RR Timer 12 Run Reset (w)
      (0U    <<1U) | // T12RS Timer 12 Run Set (w)
      (0U    <<2U) | // T12RES Timer 12 Reset (w)
      (0U    <<3U) | // DTRES Dead-Time Counter Reset (w)
      (0U    <<5U) | // T12CNT Timer T12 Count Event if enabled (PISELH) (w)
      (0U    <<6U) | // T12STR Timer 12 Shadow Transfer Request (w)
      (0U    <<7U) | // T12STD Timer 12 Shadow Transfer Disable (w)
      (0U    <<8U) | // T13RR Timer 13 Run Reset (w)
      (1U    <<9U) | // T13RS Timer 13 Run Set (w)
      (0U    <<10U)| // T13RES Timer 13 Reset (w)
      (0U    <<13U)| // T13CNT Timer T13 Count Event if enabled (PISELH) (w)
      (0U    <<14U)| // T13STR Timer 13 Shadow Transfer Request (w)
      (0U    <<15U); // T13STD Timer 13 Shadow Transfer Disable (w)


} // end of function SCS_CountClocksWithTimer
