#include <avr/io.h>
#define F_CPU 14745600
#include <util/delay.h>

#ifndef LCD_C
#define LCD_C


/*	With these functions you can access the LCD of the EVA board
*/


//inits the display, NEEDS TO BE CALLED ONCE IN THE BEGINNING OF THE PROGRAM
//eg: LCD_init();
void LCD_init(void);


//writes one character to the current cursor position
//eg: LCD_Write_data('A');
void LCD_write_data(unsigned char c);


//sets the cursor postion
//0-63:   first display line
//64-127: second display line
//eg: LCD_setCursor(0);
void LCD_setCursor(unsigned char c);


//clears the display screen and sets the cursor to position 0
//eg: LCD_cls();
void LCD_cls(void);

/**********************end of header ************************************/

//write routine for direct IO to the RAM address space
void SRAM_write(unsigned int addr,unsigned char c){
	*(volatile unsigned char *)(addr) = (unsigned char)(c);
}

//read routine for direct IO to the RAM address space
unsigned char SRAM_read(unsigned int addr){
	return (*(volatile unsigned char *)(addr) );
}

//simple wait routine, based on a 16MHZ crytal
void DelayUs(unsigned int i){
	while(i!=0){
		i--;
		_delay_us(1);
	}
}

//simple wait routine, based on DelayUs
void DelayMs(unsigned int i){
	while(i!=0){
		i--;
		DelayUs(1000);
	}
}

#define LCD_BASE 0xFF00
#define LCD_ENABLE 1
#define LCD_RS 2
#define LCD_CLS 1
#define LCD_WAIT DelayUs(8);

void LCD_write_data(unsigned char c){
	//higher 4 bits first
	SRAM_write(LCD_BASE,(c&0xF0)|LCD_RS);
	LCD_WAIT;
	//LCD_ENABLE=1
	SRAM_write(LCD_BASE,(c&0xF0)|LCD_RS|LCD_ENABLE);
	LCD_WAIT;
	//LCD_ENABLE=0
	SRAM_write(LCD_BASE,(c&0xF0)|LCD_RS);
	LCD_WAIT;
	//the lower 4 bits
	SRAM_write(LCD_BASE,((c<<4)&0xF0)|LCD_RS);
	LCD_WAIT;
	SRAM_write(LCD_BASE,((c<<4)&0xF0)|LCD_RS|LCD_ENABLE);
	LCD_WAIT;
	SRAM_write(LCD_BASE,((c<<4)&0xF0)|LCD_RS);
	LCD_WAIT;
	//make all latch pins 0
	SRAM_write(LCD_BASE,0);
	LCD_WAIT;
}

void LCD_write_command(unsigned char c){
	//higher 4 bits first
	SRAM_write(LCD_BASE,(c&0xF0));
	LCD_WAIT;
	//LCD_ENABLE=1
	SRAM_write(LCD_BASE,(c&0xF0)|LCD_ENABLE);
	LCD_WAIT;
	//LCD_ENABLE=0
	SRAM_write(LCD_BASE,(c&0xF0));
	LCD_WAIT;
	//the lower 4 bits
	SRAM_write(LCD_BASE,((c<<4)&0xF0));
	LCD_WAIT;
	SRAM_write(LCD_BASE,((c<<4)&0xF0)|LCD_ENABLE);
	LCD_WAIT;
	SRAM_write(LCD_BASE,((c<<4)&0xF0));
	LCD_WAIT;
	//make all latch pins 0
	SRAM_write(LCD_BASE,0);
	LCD_WAIT;
}

void LCD_setCursor(unsigned char c){
	LCD_write_command(0x80|c);
	DelayMs(2);
}

void LCD_cls(void){
	//clear screen
	LCD_write_command(LCD_CLS);
	DelayMs(2);
}

void LCD_init(void){
	//enable the external SRAM interface
	MCUCR=128;
	XMCRA=0;
	XMCRB=128;
	
	LCD_write_command(0x33);
	//function set: 4Bit interface,RS=0
	SRAM_write(LCD_BASE,0x20);
	LCD_WAIT;
	//LCD_ENABLE=1
	SRAM_write(LCD_BASE,0x20|LCD_ENABLE);
	LCD_WAIT;
	//LCD_ENABLE=0
	SRAM_write(LCD_BASE,0x20);
	LCD_WAIT;
	//make all latch pins 0
	SRAM_write(LCD_BASE,0);
	LCD_WAIT;
	//clear screen
	LCD_write_command(LCD_CLS);
	DelayMs(2);
	//function set: 4 bit mode, 2 lines
	LCD_write_command(0x28);
	DelayMs(2);
	//clear screen
	LCD_write_command(LCD_CLS);
	DelayMs(2);
	//display on, blinking cursor
	LCD_write_command(0x0c);
	DelayMs(2);
}

#endif