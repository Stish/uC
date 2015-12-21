#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>						//library for interrupt functions
#include "can_at90can128_AB_ID.h"
#include "can_at90can128_AB_ID.c"

void USART1_ini (void);
void TIMER0_ini (void);
