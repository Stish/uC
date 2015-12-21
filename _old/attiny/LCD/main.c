#include <avr/io.h>
#include "lcd_sr.c"

int main(void)
{
	while(1)
	{
		lcd_init();
		lcd_clr();
		lcd_setpos(0, 0);
		lcd_string("Hello world!");
	}
}