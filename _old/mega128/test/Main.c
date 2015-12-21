#define STR_LEN 26

#include "lib/io_fct.c"                                // ATmega128 MCU
#include "lib/LC7981.c"
#include <avr/signal.h>
#include <avr/interrupt.h>
//#include "lib/insel.c"
//#include "lib/draw_penguin.c"
//#include "lib/layout.c"
//#include <delay.h>                                  // Delay functions

void g_draw_prog(unsigned char x, unsigned char y, unsigned char perc);
void g_draw_layout(void);
void g_draw_triangel_right(unsigned char x, unsigned char y);

unsigned char getting_artist = 0;
unsigned char artist_pos = 0;

unsigned char getting_title = 0;
unsigned char title_pos = 0;

unsigned char getting_tit_length = 0;
unsigned char tit_length_pos = 0;

unsigned char getting_tit_time_ratio = 0;

unsigned char artist[STR_LEN] = "Artist";
unsigned char title[STR_LEN] = "Title";
unsigned char tit_length[STR_LEN] = "Title Length";
unsigned char tit_time_ratio = 0;

unsigned char button = 0;

ISR(USART0_RX_vect)
{
	unsigned char udr_buffer;
	unsigned char i = 0;
	udr_buffer = UDR0;
//============================================
//==		 	Getting Artist 				==
//============================================
	if ((udr_buffer == 255) || (artist_pos == (STR_LEN-1)))
	{
		getting_artist = 0;
		for (artist_pos; artist_pos <= (STR_LEN-1); artist_pos++)
		{
			artist[artist_pos] = ' ';
		}
		artist[STR_LEN-1] = '\0';
	}
	if (getting_artist)
	{
		artist[artist_pos] = udr_buffer;
		artist_pos++;
	}
	if (udr_buffer == 2)
	{
		getting_artist = 1;
		artist_pos = 0;
	}
//============================================
//==		 	Getting Title 				==
//============================================	
	if ((udr_buffer == 254) || (title_pos == (STR_LEN-1)))
	{
		getting_title = 0;
		for (title_pos; title_pos <= (STR_LEN-1); title_pos++)
		{
			title[title_pos] = ' ';
		}
		title[STR_LEN-1] = '\0';
	}	
	if (getting_title)
	{
		title[title_pos] = udr_buffer;
		title_pos++;
	}
	if (udr_buffer == 3)
	{
		getting_title = 1;
		title_pos = 0;
	}
//============================================
//==		 	Getting Tit_length 			==
//============================================	
	if ((udr_buffer == 253) || (tit_length_pos == (STR_LEN-1)))
	{
		getting_tit_length = 0;
		for (tit_length_pos; tit_length_pos <= (STR_LEN-1); tit_length_pos++)
		{
			tit_length[tit_length_pos] = ' ';
		}
		tit_length[STR_LEN-1] = '\0';
	}	
	if (getting_tit_length)
	{
		tit_length[tit_length_pos] = udr_buffer;
		tit_length_pos++;
	}
	if (udr_buffer == 4)
	{
		getting_tit_length = 1;
		tit_length_pos = 0;
	}
//============================================
//==		 	Getting Tit_time_ratio		==
//============================================	
	if (udr_buffer == 251)
	{
		getting_tit_time_ratio = 0;
	}	
	if (getting_tit_time_ratio)
	{
		tit_time_ratio = udr_buffer-10;
	}
	if (udr_buffer == 6)
	{
		getting_tit_time_ratio = 1;
	}
//============================================
//==		 	Sending Commands			==
//============================================	
	if (!(getting_artist) && !(getting_title))
	{	
		UDR0 = button;
	}
}

int main(void)
{
	init_in_pin('b', 0);
	init_serial();

	UCSR0B |= (1 << RXCIE);	//enable interrupt
	sei();
	
	reset_pin('a', 1);
	set_pin('a', 2);
	
	lcd_graphics_init();
	lcd_graphics_clear();

	g_draw_layout();
	
	while(1)													// Loop Continue
	{
		if (!(getting_artist) && !(getting_title) && !(getting_tit_time_ratio) && !(getting_tit_length))
		{
			g_draw_string(3, 3, artist);
			g_draw_string(3, 13, title);	
			g_draw_string(3, 23, tit_length);
			g_draw_prog(4, 54, tit_time_ratio);
		}
		button = get_in_pin('b', 0);
	} 
}	

void g_draw_prog(unsigned char x, unsigned char y, unsigned char perc)
{
	unsigned char i = 0;
	
	for (i = 1; i <= (perc*1.5); i++)
	{
		lcd_graphics_plot_pixel(x+i, y, PIXEL_ON);
		lcd_graphics_plot_pixel(x+i, y+1, PIXEL_ON);
	}
	for (i = (perc*1.5)+1; i <= 150; i++)
	{
		lcd_graphics_plot_pixel(x+i, y, PIXEL_OFF);
		lcd_graphics_plot_pixel(x+i, y+1, PIXEL_OFF);
	}
}

void g_draw_layout(void)
{
	g_draw_rectangle(0, 0, 160, 80);		//Frame
//========== Button1 ==========================================	
	g_draw_rectangle(0, 59, 28, 21);		//Button1
//========== Button2 ==========================================
	g_draw_rectangle(27, 59, 28, 21);		//Button2
	g_draw_triangel_right(35, 63);
//========== Button3 ==========================================
	g_draw_rectangle(54, 59, 28, 21);		//Button3
//========== Button4 ==========================================
	g_draw_rectangle(81, 59, 27, 21);		//Button4
//========== Button5 ==========================================
	g_draw_rectangle(107, 59, 27, 21);		//Button5
//========== Button6 ==========================================
	g_draw_rectangle(133, 59, 27, 21);		//Button6
//========== ProgBar ==========================================
	g_draw_horizontal_line(0, 50, 160);		//Seperater ProgBar
	g_draw_rectangle(3, 52, 154, 6);		//frame ProgBar
}

void g_draw_triangel_right(unsigned char x, unsigned char y)
{
	g_draw_vertical_line(x, y, 13);
	lcd_graphics_plot_pixel(x+1, y+1, PIXEL_ON);
	lcd_graphics_plot_pixel(x+2, y+1, PIXEL_ON);
	lcd_graphics_plot_pixel(x+3, y+2, PIXEL_ON);
	lcd_graphics_plot_pixel(x+4, y+2, PIXEL_ON);
	lcd_graphics_plot_pixel(x+5, y+3, PIXEL_ON);
	lcd_graphics_plot_pixel(x+6, y+3, PIXEL_ON);
	lcd_graphics_plot_pixel(x+7, y+4, PIXEL_ON);
	lcd_graphics_plot_pixel(x+8, y+4, PIXEL_ON);
	lcd_graphics_plot_pixel(x+9, y+5, PIXEL_ON);
	lcd_graphics_plot_pixel(x+10, y+5, PIXEL_ON);
	lcd_graphics_plot_pixel(x+11, y+6, PIXEL_ON);
	lcd_graphics_plot_pixel(x+12, y+6, PIXEL_ON);
	lcd_graphics_plot_pixel(x+10, y+7, PIXEL_ON);
	lcd_graphics_plot_pixel(x+9, y+7, PIXEL_ON);
	lcd_graphics_plot_pixel(x+8, y+8, PIXEL_ON);
	lcd_graphics_plot_pixel(x+7, y+8, PIXEL_ON);
	lcd_graphics_plot_pixel(x+6, y+9, PIXEL_ON);
	lcd_graphics_plot_pixel(x+5, y+9, PIXEL_ON);
	lcd_graphics_plot_pixel(x+4, y+10, PIXEL_ON);
	lcd_graphics_plot_pixel(x+3, y+10, PIXEL_ON);
	lcd_graphics_plot_pixel(x+2, y+11, PIXEL_ON);
	lcd_graphics_plot_pixel(x+1, y+11, PIXEL_ON);
}