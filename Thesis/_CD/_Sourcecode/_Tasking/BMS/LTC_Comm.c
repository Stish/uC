#include "MAIN.h"

extern unsigned char LTC_data_read[17] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
extern unsigned char LTC_CRC_PEC = 0x74;
extern unsigned char LTC_CFG1R0 = 0x69;
extern unsigned char LTC_CFG1R1 = 0x00;
extern unsigned char LTC_CFG1R2 = 0x00;
extern unsigned char LTC_CFG1R3 = 0x00;
extern unsigned char LTC_CFG1R4 = 0x00;
extern unsigned char LTC_CFG1R5 = 0x00;

/*
 * LTC_Comm.c
 *
 *  Created on: Jul 23, 2012
 *      Author: Alexander Wegner
 */

void LTC_read (unsigned char *data, unsigned char length)
{
	for (unsigned char byte = 0; byte <= length-1; byte++)
	{
		for (unsigned char bit = 0; bit <= 7; bit++)
		{
			LTC_SCK_LOW;
			LTC_wait();
			if (LTC_SDO_READ == 0x01)
			{
				data[byte] = data[byte] | (0x80 >> bit);
			}
			else
			{
				data[byte] = data[byte] & ~(0x80 >> bit);
			}
			LTC_SCK_HIGH;
			LTC_wait();
		}
	}
}

void LTC_send (unsigned char data)
{
	for (unsigned char i = 0; i <= 7; i++)
	{
		LTC_SCK_LOW;
		if (data & 0x0080 >> i)
		{
			LTC_SDI_HIGH;
		}
		else
		{
			LTC_SDI_LOW;
		}
		LTC_wait();
		LTC_SCK_HIGH;
		LTC_wait();
	}
}

void LTC_wait (void)
{
	unsigned int x = 0;

	while (x <= 5000)
	{
		x++;
		IO_vTogglePin(IO_P10_7);
	}
}

void LTC_Init (void)
{
	// CS low - Starting SPI communication
	LTC_CS_LOW;
	LTC_wait();
	// Sending config commands
	LTC_send(0x01);		// WRCFG
	LTC_send(0xC7);		// WRCFG PEC
	LTC_send(LTC_CFG1R0);	// Command byte 0
	LTC_send(LTC_CFG1R1);	// Command byte 1
	LTC_send(LTC_CFG1R2);	// Command byte 2
	LTC_send(LTC_CFG1R3);	// Command byte 3
	LTC_send(LTC_CFG1R4);	// Command byte 4
	LTC_send(LTC_CFG1R5);	// Command byte 5
	LTC_send(LTC_CRC_PEC);	// Command bytes PEC
	// CS high - Ending SPI communication
	LTC_wait();
	LTC_CS_HIGH;
	LTC_wait();
	// CS low - Starting SPI communication
	LTC_CS_LOW;
	LTC_wait();
	// Sending STCVAD command for ADC start
	LTC_send(0x010);	// STCVAD
	LTC_send(0x0B0);	// STCVAD PEC
	// CS high - Ending SPI communication
	LTC_wait();
	LTC_CS_HIGH;
	LTC_wait();
}

void LTC_getCellVoltages (void)
{
	// CS low - Starting SPI communication
	LTC_CS_LOW;
	LTC_wait();
	// Sending config commands
	LTC_send(0x01);		// WRCFG
	LTC_send(0xC7);		// WRCFG PEC
	LTC_send(LTC_CFG1R0);	// Command byte 0
	LTC_send(LTC_CFG1R1);	// Command byte 1
	LTC_send(LTC_CFG1R2);	// Command byte 2
	LTC_send(LTC_CFG1R3);	// Command byte 3
	LTC_send(LTC_CFG1R4);	// Command byte 4
	LTC_send(LTC_CFG1R5);	// Command byte 5
	LTC_send(LTC_CRC_PEC);	// Command bytes PEC
	// CS high - Ending SPI communication
	LTC_wait();
	LTC_CS_HIGH;
	LTC_wait();
	// CS low - Starting SPI communication
	LTC_CS_LOW;
	LTC_wait();
	// Sending STCVAD command for ADC start
	LTC_send(0x010);	// STCVAD
	LTC_send(0x0B0);	// STCVAD PEC
	//LTC_send(0x020);	// STOWAD
	//LTC_send(0x020);	// STOWAD PEC
	// CS high - Ending SPI communication
	LTC_wait();
	LTC_CS_HIGH;
	LTC_wait();
	// CS low - Starting SPI communication
	LTC_CS_LOW;
	LTC_wait();
	// Sending RDCVA for reading cell voltages 1 to 4
	LTC_send(0x06);		// RDCVA command
	LTC_send(0xD2);		// RDCVA PEC
	// SDI low for reading data on SDI
	LTC_SDI_LOW;
	LTC_wait();
	// Reading SDI and save data to a byte array
	LTC_read(LTC_data_read, 6);
	// PEC from LTC
	for (unsigned int i = 1; i <= (1*8); i++)
	{
		LTC_SCK_Strope();
	}
	// CS high - Ending SPI communication
	LTC_wait();
	LTC_CS_HIGH;
	LTC_wait();
	// Saving data to CAN messages
	// Cell voltage 01
	ucaCellVolts14[0] = LTC_data_read[0];
	ucaCellVolts14[1] = 0x00 | (LTC_data_read[1] & 0x0f);
	// Cell voltage 02
	ucaCellVolts14[2] = 0x00 | ((LTC_data_read[1] & 0xf0) >> 4) | ((LTC_data_read[2] & 0x0f) << 4);
	ucaCellVolts14[3] = 0x00 | ((LTC_data_read[2] & 0xf0) >> 4);
	// Cell voltage 03
	ucaCellVolts14[4] = LTC_data_read[3];
	ucaCellVolts14[5] = 0x00 | (LTC_data_read[4] & 0x0f);
	// Cell voltage 04
	ucaCellVolts14[6] = 0x00 | ((LTC_data_read[4] & 0xf0) >> 4) | ((LTC_data_read[5] & 0x0f) << 4);
	ucaCellVolts14[7] = 0x00 | ((LTC_data_read[5] & 0xf0) >> 4);
	// CS low - Starting SPI communication
	LTC_CS_LOW;
	LTC_wait();
	// Sending RDCVB for reading cell voltages 5 to 8
	LTC_send(0x008);		// RDCVB command
	LTC_send(0x0F8);		// RDCVB PEC
	// SDI low for reading data on SDI
	LTC_SDI_LOW;
	LTC_wait();
	// Reading SDI and save data to a byte array
	LTC_read(LTC_data_read, 6);
	// PEC from LTC
	for (unsigned int i = 1; i <= (1*8); i++)
	{
		LTC_SCK_Strope();
	}
	// CS high - Ending SPI communication
	LTC_wait();
	LTC_CS_HIGH;
	LTC_wait();
	// Saving data to CAN messages
	// Cell voltage 05
	ucaCellVolts58[0] = LTC_data_read[0];
	ucaCellVolts58[1] = 0x00 | (LTC_data_read[1] & 0x0f);

	ucaCellVolts58[2] = 0x00 | ((LTC_data_read[1] & 0xf0) >> 4) | ((LTC_data_read[2] & 0x0f) << 4);
	ucaCellVolts58[3] = 0x00 | ((LTC_data_read[2] & 0xf0) >> 4);
	//LTC_wait();
}

void LTC_SCK_Strope (void)
{
	LTC_SCK_LOW;
	LTC_wait();
	LTC_SCK_HIGH;
	LTC_wait();
}

void LTC_CalcPec(void)
{
	#define DATA_BYTES 6

	unsigned char PEC = 0x41;
	unsigned char DIN = 0x00;
	unsigned char IN0 = 0x00;
	unsigned char IN1 = 0x00;
	unsigned char IN2 = 0x00;

	unsigned long long value = 0x00;
	value = ((long long)LTC_CFG1R0 << 40) | ((long long)LTC_CFG1R1 << 32) | ((long long)LTC_CFG1R2 << 24) | ((long long)LTC_CFG1R3 << 16) | ((long long)LTC_CFG1R4 << 8) | (LTC_CFG1R5 << 0);



	for (unsigned char i = 0; i <= (8 * DATA_BYTES) - 1; i++)
	{
		if (((long long)0x80 << (8*(DATA_BYTES - 1)) >> i) & value)
		{
			DIN = 1;
		}
		else
		{
			DIN = 0;
		}
		//DIN = ((0x80 >> i) & value) >> i;

		IN0 = (((DIN << 7) ^ PEC) >> 7) & 0x01;
		IN1 = (IN0 ^ PEC) & 0x01;
		IN2 = (((IN0 << 1) ^ PEC) >> 1)  & 0x01;

		PEC = (PEC << 1);
		if (IN2 == 0x01)
		{
			PEC = PEC | (0x01 << 2);
		}
		else
		{
			PEC = PEC & ~(0x01 << 2);
		}

		if (IN1 == 0x01)
		{
			PEC = PEC | (0x01 << 1);
		}
		else
		{
			PEC = PEC & ~(0x01 << 1);
		}

		if (IN0 == 0x01)
		{
			PEC = PEC | 0x01;
		}
	}
	LTC_CRC_PEC = PEC;
}

