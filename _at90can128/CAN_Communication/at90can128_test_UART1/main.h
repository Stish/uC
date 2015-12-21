#define BAUD 8	//UBRR0 for 16MHz, U2X0 = 0
#define F_CPU 16000000

#include <avr/io.h>	
#include <avr/interrupt.h>


void TIMER0_ini (void);
void USART0_ini (void);
void USART1_ini (void);

uint8_t timer0_cnt = 0;