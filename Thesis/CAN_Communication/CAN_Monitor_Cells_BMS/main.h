#define F_CPU 16000000

#include <avr/io.h>
#include "lib/graphics.c"
#include "lib/LC7981.c"
#include <avr/interrupt.h>						//library for interrupt functions
#include "lib/touch_pictures.c"					//library for cover picture function
#include "can_at90can128_AB_ID.h"
#include "can_at90can128_AB_ID.c"

void getMaxMinCell (void);
void USART1_ini (void);
void TIMER0_ini (void);

//uint8_t timer0_cnt = 0;