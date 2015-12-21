#define F_CPU 16000000

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "can_at90can128.c"

void TIMER0_ini (void);

can_message msg;
uint8_t timer0_cnt = 0;