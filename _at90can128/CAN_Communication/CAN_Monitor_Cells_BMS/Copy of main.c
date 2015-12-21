// RX Module
//#include "main.h"
// Buttons
#define BUT_DDR DDRB
#define BUT_PIN PINB
#define BUT_BLACK 4
#define BUT_RED 6
// Status LED Port
#define LED_PORT PORTA
#define LED_DDR DDRA
// Status LED Pins
#define LED_RED 1
#define LED_YEL 3
#define LED_GRE 5
// Display Data Port
#define LCD_DATA_DDR	DDRC
#define LCD_DATA_PORT	PORTC
#define LCD_DATA_PIN	PINC
// Display Control Port
#define LCD_CTRL_DDR	DDRA
#define LCD_CTRL_PORT	PORTA
// Display Control Pins
#define LCD_CTRL_RS	7
//#define LCD_CTRL_RW	5
#define LCD_CTRL_E	6
// Display Size 
#define LCD_WIDTH	160
#define LCD_HEIGHT	80
// Cell Values
#define MAX_VOLTAGE 360		// 3.60V
#define MIN_VOLTAGE 310		// 3.10V
#define VOLTAGE_DIFF_TRASHHOLD 50	// 50mV
//
#define BAUD 3	//UBRR0 for 16MHz, U2X0 = 0 -> 250k Baud
#define F_CPU	16000000

#include <avr/io.h>
#include "lib/LC7981.c"
#include <avr/interrupt.h>						//library for interrupt functions
//#include "lib/touch_pictures.c"					//library for cover picture function
#include "lib/cells_pic.c"
#include "lib/status_pic.c"
#include "can_at90can128_AB_ID.c"

// Macros
#define LED_RED_ON LED_PORT|=(1<<LED_RED)
#define LED_GRE_ON LED_PORT|=(1<<LED_GRE)
#define LED_YEL_ON LED_PORT|=(1<<LED_YEL)

#define LED_RED_OFF LED_PORT&=~(1<<LED_RED)
#define LED_GRE_OFF LED_PORT&=~(1<<LED_GRE)
#define LED_YEL_OFF LED_PORT&=~(1<<LED_YEL)

#define LED_YEL_TOGGLE LED_PORT^=(1<<LED_YEL)

void USART1_ini(void);
void TIMER0_ini(void);

// Total voltage
unsigned char UART_MSG_01[13] = {0x80, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Total current
unsigned char UART_MSG_02[13] = {0x80, 0x00, 0x00, 0x01, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Cell voltages 11-18
unsigned char UART_MSG_03[13] = {0x80, 0x00, 0x00, 0x01, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char UART_MSG_04[13] = {0x80, 0x00, 0x00, 0x01, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Cell voltages 21-28
unsigned char UART_MSG_05[13] = {0x80, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char UART_MSG_06[13] = {0x80, 0x00, 0x00, 0x01, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Cell voltages 31-38
unsigned char UART_MSG_07[13] = {0x80, 0x00, 0x00, 0x01, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char UART_MSG_08[13] = {0x80, 0x00, 0x00, 0x01, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned int timer0_cnt = 0;
unsigned char send_data = 0;
unsigned char send_data_position = 0;

unsigned char cell_voltage_bcd[15][5] = {("N.a."), ("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a."),("N.a.")};
unsigned char cell_voltage_na[5] = "N.a.";

unsigned int cell_voltage_total = 0;
unsigned int cell_voltage[15];
unsigned int cell_voltage_buffer[15];
unsigned char cell_max_voltage = 0;
unsigned char cell_min_voltage = 0;

can_message msg;
can_message msg_rec;
// Cell voltages 1-5
can_message msg_CellVolt_BMC1_14;
can_message msg_CellVolt_BMC1_58;
// Cell voltages 6-10
can_message msg_CellVolt_BMC2_14;
can_message msg_CellVolt_BMC2_58;
// Cell voltages 11-15
can_message msg_CellVolt_BMC3_14;
can_message msg_CellVolt_BMC3_58;
// Current
can_message msg_CurrentLEM;
// BMC Status
can_message msg_BMC1_status;
can_message msg_BMC2_status;
can_message msg_BMC3_status;

unsigned char ucButtonRed = 0;
unsigned char ucButtonBlack = 0;
unsigned char ucButtonRedPressed = 0;
unsigned char ucButtonBlackPressed = 0;

unsigned char ucScreenStatus = 0;

unsigned char ucScreenStatus_0_Ack = 0;
unsigned char ucScreenStatus_1_Ack = 0;
unsigned char ucScreenStatus_2_Ack = 0;

unsigned char ucConductorStatus = 1;

unsigned char ucBMC1_CAN_Status = 0;
unsigned char ucBMC2_CAN_Status = 0;
unsigned char ucBMC3_CAN_Status = 0;
unsigned char ucLEM_CAN_Status = 0;

unsigned char ucBMC1_CAN_Timeout = 0;
unsigned char ucBMC2_CAN_Timeout = 0;
unsigned char ucBMC3_CAN_Timeout = 0;
unsigned char ucLEM_CAN_Timeout = 0;

unsigned char ucaCAN_online[8] = "online ";
unsigned char ucaCAN_offline[8] = "offline";

unsigned char ucaConductor_open[7] = "open  ";
unsigned char ucaConductor_closed[7] = "closed";

unsigned char uacGlobal_Current[9] = "+000.00A";

unsigned char uacLowestCell[] = "Zelle 00: 0.00V";
unsigned char uacHighestCell[] = "Zelle 00: 0.00V";

SIGNAL (SIG_OVERFLOW0)	
{
	TCNT0 = 0x05;
	timer0_cnt++;
	
	if (timer0_cnt % 20 == 0)
	{
		
		ucBMC1_CAN_Timeout++;
		ucBMC2_CAN_Timeout++;
		ucBMC3_CAN_Timeout++;
		ucLEM_CAN_Timeout++;
		// Button Red
		if (BUT_PIN & (1 << BUT_RED))
		{
			if (ucButtonRed == 0)
			{
				ucButtonRedPressed = 1;
			}
			else
			{
				ucButtonRedPressed = 0;
			}
			ucButtonRed = 1;
		}
		else
		{
			ucButtonRed = 0;
			ucButtonRedPressed = 0;
		}
		// Button Black
		if (BUT_PIN & (1 << BUT_BLACK))
		{
			if (ucButtonBlack == 0)
			{
				ucButtonBlackPressed = 1;
			}
			else
			{
				ucButtonBlackPressed = 0;
			}
			ucButtonBlack = 1;
		}
		else
		{
			ucButtonBlack = 0;
			ucButtonBlackPressed = 0;
		}
		// Screenchange
		if (ucButtonRedPressed == 1)
		{
			ucScreenStatus++;
			ucScreenStatus_0_Ack = 0;
			ucScreenStatus_1_Ack = 0;
			ucScreenStatus_2_Ack = 0;
			if (ucScreenStatus == 3)
			{
				ucScreenStatus = 0;
			}
		}
		// Timeouts
		if (ucBMC1_CAN_Timeout >= 5)
		{
			ucBMC1_CAN_Status = 0;
		}
		
		if (ucBMC2_CAN_Timeout >= 5)
		{
			ucBMC2_CAN_Status = 0;
		}
		
		if (ucBMC3_CAN_Timeout >= 5)
		{
			ucBMC3_CAN_Status = 0;
		}
		
		if (ucLEM_CAN_Timeout >= 5)
		{
			ucLEM_CAN_Status = 0;
		}
		send_data = 1;
		send_data_position = 1;
		UDR0 = UART_MSG_01[0];
	}
	
	if (timer0_cnt == 1000)
	{
		timer0_cnt = 0;
		/*
		ucScreenStatus += 0x01;
		ucScreenStatus_0_Ack = 0;
		ucScreenStatus_1_Ack = 0;
		ucScreenStatus_2_Ack = 0;
		if (ucScreenStatus == 3)
		{
			ucScreenStatus = 0;
		}*/
		/*
		if (ucScreenStatus == 0)
		{
			lcd_graphics_clear();	
			draw_cells();
		}
		
		if (ucScreenStatus == 1)
		{
			lcd_graphics_clear();	
			draw_status();
		}*/
		//send_data = 1;
		//UDR1 = val_ad_01_bcd[send_data_position];
		//send_data_position++;
	}
}

SIGNAL (SIG_UART0_TRANS)								
{
	if (send_data >= 1)
	{
		switch (send_data)
		{
			case 1:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_01[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 2:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_02[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 3:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_03[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 4:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_04[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 5:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_05[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 6:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_06[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 7:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_07[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data++;
				}
			break;
			case 8:
				if (send_data_position < 13)
				{
					UDR0 = UART_MSG_08[send_data_position];
					send_data_position++;
				}
				else if (send_data_position == 13)
				{
					UDR0 = 0x0d;
					//send_data_position++;
					send_data_position = 0;
					send_data =0;
				}
			break;
		}
		/*
		else if (send_data_position >= 14)
		{
			UDR0 = UART_MSG_02[send_data_position - 14];
			send_data_position++;
		}
		else if (send_data_position == 28)
		{
			UDR0 = 0x0d;
			send_data = 0;
			send_data_position = 0;
		}*/
	}
}

int main(void)								//main program
{
	// LED init
	LED_DDR |= (1 << LED_RED) | (1 << LED_YEL) | (1 << LED_GRE);
	LED_PORT &= ~(1 << LED_RED) & ~(1 << LED_YEL) & ~(1 << LED_GRE);
	// Button init
	BUT_DDR &= ~(1 << BUT_RED) & ~(1 << BUT_BLACK);
	// CAN init, 125kbit/s and RX interrupt enabled
	can_init(125, RX);
	// CAN test message
	msg_rec.id = 0x030;
	msg_rec.idm = 0xfff;
	msg_rec.id_type = 0;
	// CAN message: Cell voltages 1 - 4 of BMC1
	msg_CellVolt_BMC1_14.id = 0x030;
	msg_CellVolt_BMC1_14.idm = 0xfff;
	msg_CellVolt_BMC1_14.id_type = 0;
	// CAN message: Cell voltages 5 - 8 of BMC1
	msg_CellVolt_BMC1_58.id = 0x031;
	msg_CellVolt_BMC1_58.idm = 0xfff;
	msg_CellVolt_BMC1_58.id_type = 0;
	// CAN message: Cell voltages 1 - 4 of BMC2
	msg_CellVolt_BMC2_14.id = 0x040;
	msg_CellVolt_BMC2_14.idm = 0xfff;
	msg_CellVolt_BMC2_14.id_type = 0;
	// CAN message: Cell voltages 5 - 8 of BMC2
	msg_CellVolt_BMC2_58.id = 0x041;
	msg_CellVolt_BMC2_58.idm = 0xfff;
	msg_CellVolt_BMC2_58.id_type = 0;
	// CAN message: Cell voltages 1 - 4 of BMC3
	msg_CellVolt_BMC3_14.id = 0x050;
	msg_CellVolt_BMC3_14.idm = 0xfff;
	msg_CellVolt_BMC3_14.id_type = 0;
	// CAN message: Cell voltages 5 - 8 of BMC3
	msg_CellVolt_BMC3_58.id = 0x051;
	msg_CellVolt_BMC3_58.idm = 0xfff;
	msg_CellVolt_BMC3_58.id_type = 0;
	// CAN message: Status of BMC1
	msg_BMC1_status.id = 0x020;
	msg_BMC1_status.idm = 0xfff;
	msg_BMC1_status.id_type = 0;
	// CAN message: Status of BMC2
	msg_BMC2_status.id = 0x021;
	msg_BMC2_status.idm = 0xfff;
	msg_BMC2_status.id_type = 0;
	// CAN message: Status of BMC3
	msg_BMC3_status.id = 0x022;
	msg_BMC3_status.idm = 0xfff;
	msg_BMC3_status.id_type = 0;
	// CAN message: Current from LEM
	msg_CurrentLEM.id = 0x060;
	msg_CurrentLEM.idm = 0xfff;
	msg_CurrentLEM.id_type = 0;
	// CAN message objects RX
	can_enable_mob(0, RECEIVE_DATA, msg_CellVolt_BMC1_14);
	can_enable_mob(1, RECEIVE_DATA, msg_CellVolt_BMC1_58);
	can_enable_mob(2, RECEIVE_DATA, msg_CellVolt_BMC2_14);
	can_enable_mob(3, RECEIVE_DATA, msg_CellVolt_BMC2_58);
	can_enable_mob(4, RECEIVE_DATA, msg_CellVolt_BMC3_14);
	can_enable_mob(5, RECEIVE_DATA, msg_CellVolt_BMC3_58);
	can_enable_mob(6, RECEIVE_DATA, msg_BMC1_status);
	can_enable_mob(7, RECEIVE_DATA, msg_BMC2_status);
	can_enable_mob(8, RECEIVE_DATA, msg_BMC3_status);
	can_enable_mob(9, RECEIVE_DATA, msg_CurrentLEM);
	
	PORTE &= ~(0x01 << 6);
	PORTE |= (0x01 << 6);

	lcd_graphics_init();						//init LCD
	lcd_graphics_clear();						//clear LCD

	draw_cells();
	sei();										//global interrupt enable on

	TIMER0_ini();
	USART0_ini();

	while(1)
	{
		/*if (BUT_PIN & (1 << BUT_RED))
		{
			LED_GRE_ON;
		}
		else
		{	
			LED_GRE_OFF;
		}*/	
		
		if ((ucBMC1_CAN_Status == 1) && (ucBMC2_CAN_Status == 1) && (ucBMC3_CAN_Status == 1) && (ucLEM_CAN_Status == 1))
		{
			LED_RED_OFF;
			LED_GRE_ON;
		}
		else
		{
			LED_RED_ON;
			LED_GRE_OFF;
		}
		
		if (ucScreenStatus == 0)
		{
			if (ucScreenStatus_0_Ack == 0)
			{
				lcd_graphics_clear();	
				draw_cells();
				ucScreenStatus_0_Ack = 1;
			}
			
			if (ucBMC1_CAN_Status == 0)
			{
				g_draw_string(6, 16, cell_voltage_na);
				g_draw_string(37, 16, cell_voltage_na);
				g_draw_string(68, 16, cell_voltage_na);
				g_draw_string(99, 16, cell_voltage_na);
				g_draw_string(130, 16, cell_voltage_na);
			}
			else
			{
				g_draw_string(6, 16, cell_voltage_bcd[0]);
				g_draw_string(37, 16, cell_voltage_bcd[1]);
				g_draw_string(68, 16, cell_voltage_bcd[2]);
				g_draw_string(99, 16, cell_voltage_bcd[3]);
				g_draw_string(130, 16, cell_voltage_bcd[4]);
			}

			if (ucBMC2_CAN_Status == 0)
			{		
				g_draw_string(6, 42, cell_voltage_na);
				g_draw_string(37, 42, cell_voltage_na);
				g_draw_string(68, 42, cell_voltage_na);
				g_draw_string(99, 42, cell_voltage_na);
				g_draw_string(130, 42, cell_voltage_na);
			}
			else
			{
				g_draw_string(6, 42, cell_voltage_bcd[5]);
				g_draw_string(37, 42, cell_voltage_bcd[6]);
				g_draw_string(68, 42, cell_voltage_bcd[7]);
				g_draw_string(99, 42, cell_voltage_bcd[8]);
				g_draw_string(130, 42, cell_voltage_bcd[9]);
			}

			if (ucBMC3_CAN_Status == 0)
			{				
				g_draw_string(6, 68, cell_voltage_na);
				g_draw_string(37, 68, cell_voltage_na);
				g_draw_string(68, 68, cell_voltage_na);
				g_draw_string(99, 68, cell_voltage_na);
				g_draw_string(130, 68, cell_voltage_na);
			}
			else
			{
				g_draw_string(6, 68, cell_voltage_bcd[10]);
				g_draw_string(37, 68, cell_voltage_bcd[11]);
				g_draw_string(68, 68, cell_voltage_bcd[12]);
				g_draw_string(99, 68, cell_voltage_bcd[13]);
				g_draw_string(130, 68, cell_voltage_bcd[14]);				
			}
			cell_voltage_total = 0;
			for (int i = 0; i <= 14; i++)
			{
				cell_voltage_buffer[i] = cell_voltage[i];
				cell_voltage_buffer[i] -= 512;
				cell_voltage_buffer[i] *= 1.5;
				if (cell_voltage[i] != 0)
				{
					cell_voltage_total += cell_voltage_buffer[i];
				}
				cell_voltage_bcd[i][0] = (cell_voltage_buffer[i] / 1000) + 0x30;
				cell_voltage_buffer[i] = cell_voltage_buffer[i] % 1000;
				cell_voltage_bcd[i][2] = (cell_voltage_buffer[i] / 100) + 0x30;
				cell_voltage_buffer[i] = cell_voltage_buffer[i] % 100;
				cell_voltage_bcd[i][3] = (cell_voltage_buffer[i] / 10) + 0x30;
			}
			
			if ((ucBMC1_CAN_Status == 1) || (ucBMC2_CAN_Status == 1)  || (ucBMC3_CAN_Status == 1))
			{
				// Platzhalter
			}
			else
			{
				cell_voltage_total = 0;
			}
			UART_MSG_01[5] = (cell_voltage_total / 10000) + 0x30;
			cell_voltage_total = cell_voltage_total % 10000;
			UART_MSG_01[6] = (cell_voltage_total / 1000) + 0x30;
			cell_voltage_total = cell_voltage_total % 1000;
			UART_MSG_01[7] = (cell_voltage_total / 100) + 0x30;
			cell_voltage_total = cell_voltage_total % 100;
			UART_MSG_01[8] = (cell_voltage_total / 10) + 0x30;
			cell_voltage_total = cell_voltage_total % 10;
			UART_MSG_01[9] = cell_voltage_total + 0x30;
		}
		
		if (ucScreenStatus == 1)
		{
			if (ucScreenStatus_1_Ack == 0)
			{
				lcd_graphics_clear();	
				draw_status();
				ucScreenStatus_1_Ack = 1;
			}
			
			if (ucConductorStatus == 0)
			{
				g_draw_string(18, 9, ucaConductor_open);
			}
			else
			{
				g_draw_string(18, 9, ucaConductor_closed);
			}
			
			if (ucBMC1_CAN_Status == 0)
			{
				g_draw_string(10, 44, ucaCAN_offline);
			}
			else
			{
				g_draw_string(10, 44, ucaCAN_online);
			}
			
			if (ucBMC2_CAN_Status == 0)
			{
				g_draw_string(60, 44, ucaCAN_offline);
			}
			else
			{
				g_draw_string(60, 44, ucaCAN_online);
			}
			
			if (ucBMC3_CAN_Status == 0)
			{
				g_draw_string(110, 44, ucaCAN_offline);
			}
			else
			{
				g_draw_string(110, 44, ucaCAN_online);
			}
			
			if (ucLEM_CAN_Status == 0)
			{
				g_draw_string(102, 12, ucaCAN_offline);
			}
			else
			{
				g_draw_string(102, 12, uacGlobal_Current);
			}
			
		}
		
		if (ucScreenStatus == 2)
		{
			if (ucScreenStatus_2_Ack == 0)
			{
				lcd_graphics_clear();	
				//draw_status();
				ucScreenStatus_2_Ack = 1;
				g_draw_string(6, 1, "Ladeschlussspannung:");
				g_draw_string(6, 21, "Entladeschlussspannung:");
				g_draw_string(6, 41, "Hoechste Zellspannung:");
				g_draw_string(6, 61, "Niedrigste Zellspannung:");
			}
			
			getMaxMinCell();
			
			//g_draw_string(6, 1, "Ladeschlussspannung:");
			g_draw_string(6, 11, "3.60V");
			//g_draw_string(6, 21, "Entladeschlussspannung:");
			g_draw_string(6, 31, "3.10V");
			//g_draw_string(6, 41, "Höchste Zellspannung");
			g_draw_string(6, 51, uacHighestCell);
			//g_draw_string(6, 61, "Niedrigste Zellspannung:");
			g_draw_string(6, 71, uacLowestCell);
		}
	}
}	

void getMaxMinCell (void)
{
	unsigned char MaxCell = 0;
	unsigned char MinCell = 0;
	unsigned int MaxVal = 0;
	unsigned int MinVal = 0;
	unsigned int val_buffer = 0;
	
	if (ucBMC1_CAN_Status == 1)
	{
		MaxVal = cell_voltage[0];
		MinVal = cell_voltage[0];
		
		for (unsigned char i = 0; i <= 4; i++)
		{
			if (cell_voltage[i] > MaxVal)
			{
				MaxVal = cell_voltage[i];
				MaxCell = i+1;
			}
			if (cell_voltage[i] < MinVal)
			{
				MinVal = cell_voltage[i];
				MinCell = i+1;
			}
		}
	}
	
	if (ucBMC2_CAN_Status == 1)
	{
		if (ucBMC1_CAN_Status == 0)
		{
			MaxVal = cell_voltage[5];
			MinVal = cell_voltage[5];
		}
		
		for (unsigned char i = 5; i <= 9; i++)
		{
			if (cell_voltage[i] > MaxVal)
			{
				MaxVal = cell_voltage[i];
				MaxCell = i;
			}
			if (cell_voltage[i] < MinVal)
			{
				MinVal = cell_voltage[i];
				MinCell = i;
			}
		}
	}

	if (ucBMC3_CAN_Status == 1)
	{
		if ((ucBMC1_CAN_Status == 0) && (ucBMC2_CAN_Status == 0))
		{
			MaxVal = cell_voltage[10];
			MinVal = cell_voltage[10];
		}
		
		for (unsigned char i = 10; i <= 14; i++)
		{
			if (cell_voltage[i] > MaxVal)
			{
				MaxVal = cell_voltage[i];
				MaxCell = i;
			}
			if (cell_voltage[i] < MinVal)
			{
				MinVal = cell_voltage[i];
				MinCell = i;
			}
		}
	}	
	// unsigned char uacHighestCell[] = "Zelle 00: 0.00V";
	// Max cell voltage
	val_buffer = cell_voltage[MaxCell];
	uacHighestCell[10] = (val_buffer / 1000) + 0x30;
	val_buffer = val_buffer % 1000;
	uacHighestCell[12] = (val_buffer / 100) + 0x30;
	val_buffer = val_buffer % 100;
	uacHighestCell[13] = (val_buffer / 10) + 0x30;
	MaxCell++;
	uacHighestCell[6] = (MaxCell / 10) + 0x30;
	MaxCell = MaxCell % 10;
	uacHighestCell[7] = MaxCell + 0x30;
	// Min cell voltage
	val_buffer = cell_voltage[MinCell];
	val_buffer -= 512;
	val_buffer *= 1.5;
	uacLowestCell[10] = (val_buffer / 1000) + 0x30;
	val_buffer = val_buffer % 1000;
	uacLowestCell[12] = (val_buffer / 100) + 0x30;
	val_buffer = val_buffer % 100;
	uacLowestCell[13] = (val_buffer / 10) + 0x30;
	MinCell++;
	uacLowestCell[6] = (MinCell / 10) + 0x30;
	MinCell = MinCell % 10;
	uacLowestCell[7] = MinCell + 0x30;
}

void USART0_ini (void)
{
	/* Set baud rate */
	UBRR0H = (unsigned char) (BAUD>>8);
	UBRR0L = (unsigned char) BAUD;
	UCSR0C = (0 << UMSEL1) | //asynchronous
			(0 << UPM10) | (0<<UPM01) |	//Mode 00: no parity
			(0 << USBS1) | 	//1 stop bit
			(1 << UCSZ11) | (1 << UCSZ00);	//Mode 011: 8 bit data
	/* Enable transmitter, disable receiver */
	UCSR0B = (0 << RXCIE1) | (1 << TXCIE1) | (0 << RXEN1) | (1 << TXEN1) | (0 << UCSZ12);
	UCSR0A = (0 << U2X1);
}

void TIMER0_ini (void)
{
	TCCR0A = (1 << CS02) | (0 << CS01) | (0 << CS00);	//Prescaler 256
	//TCNT0 = 0x00;
	TIMSK0 = (0 << OCIE0A) | (1 << TOIE0);
	TCNT0 = 0x05;
}

SIGNAL (SIG_CAN_INTERRUPT1)
{
	uint8_t	save_canpage;
 	static can_message message;
	// Aktuelle CANPAGE sichern
 	save_canpage	= CANPAGE;
    // Index des MOB ermitteln, der den Interrupt ausgelöst hat
	uint8_t mob 	= can_get_mob_interrupt();
	// Falls es kein gültiges MOB war abbrechen
	if(mob == NOMOB)
	{
		return;
	}
	// Objekt das den Interrupt ausgelöst hat holen
	can_select_mob(mob);
	// Daten des MOBs aus CANMSG auslesen
	message	= can_get_mob_data();
	// Id der Nachricht holen
	message.id = can_get_mob_id(can_get_mob_id_type());
	
	LED_YEL_TOGGLE;
	// Checking for message IDs
	if (message.id == 0x030)
	{
		cell_voltage[0] = ((((message.data[1] << 8) | message.data[0])) - 512) * 1.5;
		cell_voltage[1] = ((((message.data[3] << 8) | message.data[2])) - 512) * 1.5;
		cell_voltage[2] = ((((message.data[5] << 8) | message.data[4])) - 512) * 1.5;
		cell_voltage[3] = ((((message.data[7] << 8) | message.data[6])) - 512) * 1.5;
		
		UART_MSG_03[5] = message.data[0];
		UART_MSG_03[6] = message.data[1];
		UART_MSG_03[7] = message.data[2];
		UART_MSG_03[8] = message.data[3];
		UART_MSG_03[9] = message.data[4];
		UART_MSG_03[10] = message.data[5];
		UART_MSG_03[11] = message.data[6];
		UART_MSG_03[12] = message.data[7];
	}

	if (message.id == 0x031)
	{
		cell_voltage[4] = ((message.data[1] << 8) | message.data[0]);
		
		UART_MSG_04[5] = message.data[0];
		UART_MSG_04[6] = message.data[1];
		UART_MSG_04[7] = message.data[2];
		UART_MSG_04[8] = message.data[3];
		UART_MSG_04[9] = message.data[4];
		UART_MSG_04[10] = message.data[5];
		UART_MSG_04[11] = message.data[6];
		UART_MSG_04[12] = message.data[7];
	}

	if (message.id == 0x040)
	{
		cell_voltage[5] = ((message.data[1] << 8) | message.data[0]);
		cell_voltage[6] = ((message.data[3] << 8) | message.data[2]);
		cell_voltage[7] = ((message.data[5] << 8) | message.data[4]);
		cell_voltage[8] = ((message.data[7] << 8) | message.data[6]);
		
		UART_MSG_05[5] = message.data[0];
		UART_MSG_05[6] = message.data[1];
		UART_MSG_05[7] = message.data[2];
		UART_MSG_05[8] = message.data[3];
		UART_MSG_05[9] = message.data[4];
		UART_MSG_05[10] = message.data[5];
		UART_MSG_05[11] = message.data[6];
		UART_MSG_05[12] = message.data[7];
	}
	
	if (message.id == 0x041)
	{
		cell_voltage[9] = ((message.data[1] << 8) | message.data[0]);
		
		UART_MSG_06[5] = message.data[0];
		UART_MSG_06[6] = message.data[1];
		UART_MSG_06[7] = message.data[2];
		UART_MSG_06[8] = message.data[3];
		UART_MSG_06[9] = message.data[4];
		UART_MSG_06[10] = message.data[5];
		UART_MSG_06[11] = message.data[6];
		UART_MSG_06[12] = message.data[7];
	}

	if (message.id == 0x050)
	{
		cell_voltage[10] = ((message.data[1] << 8) | message.data[0]);
		cell_voltage[11] = ((message.data[3] << 8) | message.data[2]);
		cell_voltage[12] = ((message.data[5] << 8) | message.data[4]);
		cell_voltage[13] = ((message.data[7] << 8) | message.data[6]);
		
		UART_MSG_07[5] = message.data[0];
		UART_MSG_07[6] = message.data[1];
		UART_MSG_07[7] = message.data[2];
		UART_MSG_07[8] = message.data[3];
		UART_MSG_07[9] = message.data[4];
		UART_MSG_07[10] = message.data[5];
		UART_MSG_07[11] = message.data[6];
		UART_MSG_07[12] = message.data[7];
	}
	
	if (message.id == 0x051)
	{
		cell_voltage[14] = ((message.data[1] << 8) | message.data[0]);
		
		UART_MSG_08[5] = message.data[0];
		UART_MSG_08[6] = message.data[1];
		UART_MSG_08[7] = message.data[2];
		UART_MSG_08[8] = message.data[3];
		UART_MSG_08[9] = message.data[4];
		UART_MSG_08[10] = message.data[5];
		UART_MSG_08[11] = message.data[6];
		UART_MSG_08[12] = message.data[7];
	}
	
	if (message.id == 0x020)
	{
		ucBMC1_CAN_Timeout = 0;
		ucBMC1_CAN_Status = 1;
	}
	
	if (message.id == 0x021)
	{
		ucBMC2_CAN_Timeout = 0;
		ucBMC2_CAN_Status = 1;
	}
	
	if (message.id == 0x022)
	{
		ucBMC3_CAN_Timeout = 0;
		ucBMC3_CAN_Status = 1;
	}
	
	if (message.id == 0x060)
	{
		ucLEM_CAN_Timeout = 0;
		ucLEM_CAN_Status = 1;
	
		uacGlobal_Current[0] = message.data[0];
		uacGlobal_Current[1] = message.data[1];
		uacGlobal_Current[2] = message.data[2];
		uacGlobal_Current[3] = message.data[3];
		uacGlobal_Current[5] = message.data[4];
		
		UART_MSG_02[5] = message.data[0];
		UART_MSG_02[6] = message.data[1];
		UART_MSG_02[7] = message.data[2];
		UART_MSG_02[8] = message.data[3];
		UART_MSG_02[9] = message.data[4];
	}
	// RXOK-Flag löschen
	clearbit(CANSTMOB, RXOK);
	// MOB auf Empfang und CAN 2.0B Standard setzen
	can_set_mob_mode(RECEIVE_DATA, can_get_mob_id_type());
	//CANPAGE wiederherstellen
	CANPAGE	= save_canpage;
}