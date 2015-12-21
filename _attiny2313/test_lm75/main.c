/*****************************************************************************
*
* Copyright (C) 2003 Atmel Corporation
*
* File          : main.c
* Compiler      : IAR EWAAVR 2.28a
* Created       : 18.07.2002 by JLL
* Modified      : 11-07-2003 by LTA
*
* Support mail  : avr@atmel.com
*
* Description   : Example showing how you could use the USI_UART drivers
*
*
****************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/sleep.h>
#include <stdlib.h>
#include "i2csw.h"
#include "lm75.h"
#include "pcf.h"

#define NOP asm volatile ("nop");
#define __disable_interrupt cli
#define __enable_interrupt sei

//#include "USI_UART_config.h"

int temp_temp = 0;
unsigned char temperatur[2];

typedef struct {

	unsigned char seg[4];
	unsigned char is_seg_0:1;
	unsigned char is_seg_1:1;
	unsigned char is_seg_2:1;
	unsigned char seg_show;
	unsigned int send;

} seg;

seg Segments;

unsigned char BitReverse( unsigned char x )
{
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;    
}

SIGNAL (SIG_OVERFLOW1) {
	TCNT1 = 150;
}	

void get_temp(void) {
			Segments.send = 0;
			i2cReceive(9, 0, 2, temperatur);
			temp_temp = temperatur[0] + 3;
			Segments.seg[0] = temp_temp%10;
			Segments.seg[1] = (temp_temp%100 - (temp_temp%100)%10)/10;
			Segments.seg[2] = (temp_temp%1000 - (temp_temp%1000)%100)/100;
}

int main( void )
{

    i2cInit();
    __enable_interrupt();                                                   // Enable global interrupts
    	
	MCUCR = (1<<SE)|(0<<SM1)|(0<<SM0);                                      // Enable Sleepmode: Idle
    unsigned char Port, send;
	
	send = 0x0;
	i2cReceives(9, 0, 1, &send, 2, temperatur);
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	TCCR1B = (1 << CS10);
	TIMSK |= (1 << TOIE1);
		get_temp();
    for( ; ; )                                                              // Run forever
    {
		Segments.send ++;
		if (Segments.send > 5000) {
			get_temp();
			Segments.send = 0;
		}
		
		if (Segments.seg_show < 2)
			Segments.seg_show++;
		else 
			Segments.seg_show = 0;
			
		if (Segments.seg_show == 0x0)
				Port = 0x0 | (1 << 6) | ( 1 << 7) |  (0xf & Segments.seg[1]);
				
		if (Segments.seg_show == 0x1)
				Port = 0x0 | (1 << 5) | ( 1 << 7) |  (0xf & Segments.seg[0]);
				
		if (Segments.seg_show == 0x2) {
				if (temperatur[1] & 0x80) 
					Port = 0x0 | (1 << 5) | ( 1 << 6) | (0xf & 0x5) ;
				else
					Port = 0x0 | (1 << 5) | ( 1 << 6) | (0xf & 0x0);	
		}
		i2cSend(4,0,1, &Port);
		sleep_mode();
	}
}


