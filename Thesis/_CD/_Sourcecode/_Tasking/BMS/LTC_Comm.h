/*
 * LTC_Comm.h
 *
 *  Created on: Jul 23, 2012
 *      Author: Alexander Wegner
 */

#ifndef LTC_COMM_H_
#define LTC_COMM_H_
// Defines
// Defines: LTC communication pins
#define LTC_CS 3
#define LTC_SDI 1
#define LTC_SCK 2
#define LTC_SDO 0
// Defines: LTC registers
//#define CFG1R0 0x61
//#define CFG1R0 0x69
//#define CFG1R1 0x00
//#define CFG1R2 0x00
//#define CFG1R3 0x00
//#define CFG1R4 0x00
//#define CFG1R5 0x00
//#define CFG_PEC 0x74
//#define CFG_PEC 0x00
// Macros
#define LTC_CS_LOW IO_vResetPin(IO_P0_3)
#define LTC_CS_HIGH IO_vSetPin(IO_P0_3)
#define LTC_SCK_LOW IO_vResetPin(IO_P0_2)
#define LTC_SCK_HIGH IO_vSetPin(IO_P0_2)
#define LTC_SDI_LOW IO_vResetPin(IO_P0_1)
#define LTC_SDI_HIGH IO_vSetPin(IO_P0_1)
#define LTC_SDO_READ IO_ubReadPin(IO_P0_0)
// Functions
void LTC_getCellVoltages (void);
void LTC_SCK_Strope (void);
void LTC_wait (void);
void LTC_send (unsigned char data);
void LTC_Init (void);
void LTC_read (unsigned char *data, unsigned char length);
void LTC_CalcPec(void);
// Global Variables
extern unsigned char LTC_data_read[17];
extern unsigned char LTC_CRC_PEC;
extern unsigned char LTC_CFG1R0;
extern unsigned char LTC_CFG1R1;
extern unsigned char LTC_CFG1R2;
extern unsigned char LTC_CFG1R3;
extern unsigned char LTC_CFG1R4;
extern unsigned char LTC_CFG1R5;

#endif /* LTC_COMM_H_ */
