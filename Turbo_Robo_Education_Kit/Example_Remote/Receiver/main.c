//====================================//
//==			Receiver			==//
//==								==//
//==		C Code by Alex.			==//
//====================================//

#include "lib/mct_fct.c"
#include <avr/interrupt.h>
#include <avr/signal.h>

void init_timer(void);
void wait(long int time);

int speed_left = 60;				//speed for left motor
int speed_right = 60;				//speed for right motor
char i_left, i_right = 0;

char left_motor = 0;				//direction of left motor (0 stop, 1 forward, 2 backward)
char right_motor = 0;				//direction of right motor (0 stop, 1 forward, 2 backward)

char rc_seq_exam = 0;
char exam_seq = 0;
unsigned int time_global, time_record = 0;

SIGNAL (SIG_OVERFLOW2)
{
	time_record++;
	time_global++;
}

SIGNAL (SIG_OVERFLOW0)
{
	if ((get_in_pin(8)) && (!exam_seq))
	{
		time_record = 1;
		exam_seq = 1;
	}
	if ((time_record == 15) && (exam_seq))
	{
		rc_seq_exam &= 0xfe;
		if (get_in_pin(8))
			rc_seq_exam |= 1;
	}
	if ((time_record == 21) && (exam_seq))
	{
		rc_seq_exam &= 0xfd;
		if (get_in_pin(8))
			rc_seq_exam |= (1<<1);
	}
	if ((time_record == 27) && (exam_seq))
	{
		rc_seq_exam &= 0xfb;
		if (get_in_pin(8))
			rc_seq_exam |= (1<<2);
	}
	if ((time_record == 33) && (exam_seq))
	{
		rc_seq_exam &= 0xf7;
		if (get_in_pin(8))
			rc_seq_exam |= (1<<3);
	}
	if ((time_record == 39) && (exam_seq))
	{
		rc_seq_exam &= 0xef;
		if (get_in_pin(8))
			rc_seq_exam |= (1<<4);
	}

	if ((time_record == 45) && (exam_seq))
	{
		exam_seq = 0;
//		set_out_ports(rc_seq_exam);
	}
	if ((time_record == 100) && (!exam_seq))
	{
		rc_seq_exam = 0;
//		set_out_ports(rc_seq_exam);
	}	
	
	i_left++;
	i_right++;
	
	if (i_left <= speed_left)
	{
		set_out_pin(1);
		
		switch (left_motor)
		{
			case 0:		//stop
			{
				reset_out_pin(2); 		//forward
				reset_out_pin(3); 		//backward
				break;
			}
			case 1:		//drive forward
			{
				set_out_pin(2); 		//forward
				reset_out_pin(3); 		//backward
				break;
			}
			case 2:		//drive backward
			{
				reset_out_pin(2); 		//forward
				set_out_pin(3); 		//backward
				break;
			}
			default:
				break;
		}
	}
	else
	{
		reset_out_pin(1); 		//enabled
	}

	if (i_right <= speed_right)
	{
		set_out_pin(4);
		
		switch (right_motor)
		{
			case 0:		//stop
			{
				reset_out_pin(5); 		//forward
				reset_out_pin(6); 		//backward
				break;
			}
			case 1:		//drive forward
			{
				set_out_pin(5); 		//forward
				reset_out_pin(6); 		//backward
				break;
			}
			case 2:		//drive backward
			{
				reset_out_pin(5); 		//forward
				set_out_pin(6); 		//backward
				break;
			}
			default:
				break;
		}
	}
	else
	{
		reset_out_pin(4); 		//enabled
	}
	
	if (i_left == 201)
	{
		i_left = 0;
	}
	if (i_right == 201)
	{
		i_right = 0;
	}
}

int main(void)
{
	init_out();
	init_in();
    init_timer();

	sei();
	
	while(1)
	{
		if (!exam_seq)
		{
			if (rc_seq_exam & 1)
				left_motor = 1;
			if (rc_seq_exam & (1<<1))
				right_motor = 1;
			if (rc_seq_exam & (1<<2))
				left_motor = 2;
			if (rc_seq_exam & (1<<3))
				right_motor = 2;
			if (!(rc_seq_exam & 1) && !(rc_seq_exam & (1<<2)))
				left_motor = 0;	
			if (!(rc_seq_exam & (1<<1)) && !(rc_seq_exam & (1<<3)))
				right_motor = 0;	
			if (rc_seq_exam & (1<<4))
			{
				speed_left = 200;
				speed_right = 200;
			}
			else
			{
				speed_left = 60;
				speed_right = 60;
			}
			if ((rc_seq_exam & 1) && !(rc_seq_exam & (1<<1)) && !(rc_seq_exam & (1<<2)) && !(rc_seq_exam & (1<<3)))
			{
				left_motor = 1;
				right_motor = 1;
				speed_left = 200;
				speed_right = 30;
			}
			if (!(rc_seq_exam & 1) && (rc_seq_exam & (1<<1)) && !(rc_seq_exam & (1<<2)) && !(rc_seq_exam & (1<<3)))
			{
				left_motor = 1;
				right_motor = 1;
				speed_left = 30;
				speed_right = 200;
			}
		}
	}
}

void wait(long int time)
{
	time_global = 1;
	while(time_global<=time);
}

void init_timer(void)
{
	TCNT2 = 0x00;
	TCCR2 = 0x04;
	
	TCNT0 = 0x00;
	TCCR0 = 0x02;
	
	TIMSK |= 0x41;
}