//	=== LED timing diagram rear ===
// 
//				  Time segments
//     |00|01|02|03|04|05|06|07|08|09|10|11|
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   01|  |  |  |  |  |  |  |  |##|##|##|##|
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   02|  |  |  |  |  |  |  |##|##|##|##|  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
// L 03|  |  |  |  |  |  |##|##|##|##|  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   04|  |  |  |  |  |##|##|##|##|  |  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
// E 05|  |  |  |  |##|##|##|##|  |  |  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   06|  |  |  |##|##|##|##|  |  |  |  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
// D 07|  |  |##|##|##|##|  |  |  |  |  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   08|  |##|##|##|##|  |  |  |  |  |  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
// S 09|##|##|##|##|  |  |  |  |  |  |  |  |
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   10|##|##|##|  |  |  |  |  |  |  |  |##|
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   11|##|##|  |  |  |  |  |  |  |  |##|##|
//   --+--+--+--+--+--+--+--+--+--+--+--+--+
//   12|##|  |  |  |  |  |  |  |  |##|##|##|
//   --+--+--+--+--+--+--+--+--+--+--+--+--+


#include "main.h"	//Import all needed defines, makros, libraries, global variables

//=== Receiver ===

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "rf12.c" 

int main(void)
{
	unsigned char chr = '1';

	vTIMER0_ini();	//Initialize ans start timer 0 (overflow after 8*0.064ms), 8bit timer, prescaler 8, interrupt enable, no preload
	vPORTS_ini();	//Initialize all ports for LED usage
	vINT0_ini();	//Initialize external interrupt 0 (int0)
	sei();			//Global interrupt enable on	
	rf12_init();
	
	while(1)		//Endless loop
	{
		rf12_rxdata(&chr, 1);
		if (chr == '1')
		{
			switche_on = 1;
		}
		if (chr == '0')
		{
			switche_on = 0;
		}
	}
}
