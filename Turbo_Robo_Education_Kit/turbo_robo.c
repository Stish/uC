//=========================================
//=== 	Output Pins for motorcontrol 	===
//=========================================
#ifndef LEFT_M_ENA_PIN
#define LEFT_M_ENA_PIN 1					//pin for left motor enable
#endif
#ifndef LEFT_M_FOR_PIN
#define LEFT_M_FOR_PIN 2					//pin for left motor forward
#endif
#ifndef LEFT_M_BACK_PIN
#define LEFT_M_BACK_PIN 3					//pin for left motor backward
#endif
#ifndef RIGHT_M_ENA_PIN
#define RIGHT_M_ENA_PIN 4					//pin for right motor enable
#endif
#ifndef RIGHT_M_FOR_PIN
#define RIGHT_M_FOR_PIN 5					//pin for right motor forward
#endif
#ifndef RIGHT_M_BACK_PIN
#define RIGHT_M_BACK_PIN 6					//pin for right motor backward
#endif
//=========================================
//=== Input Pins for sensors (digital)	===
//=========================================
#ifndef LEFT_INFRA_PIN
#define LEFT_INFRA_PIN 1						//pin for left infrared input
#endif
#ifndef RIGHT_INFRA_PIN
#define	RIGHT_INFRA_PIN 2						//pin for right infrared input
#endif
#ifndef LEFT_TRIGGER_PIN
#define	LEFT_TRIGGER_PIN 3						//pin for left trigger input
#endif
#ifndef RIGHT_TRIGGER_PIN
#define	RIGHT_TRIGGER_PIN 4						//pin for right trigger input
#endif


unsigned char i = 0;						//counting var
	
unsigned int time_global = 0;				//time

unsigned int speed_left = 0;				//speed for left motor
unsigned int speed_right = 0;				//speed for right motor

unsigned char left_motor = 0;				//direction of left motor (0 stop, 1 forward, 2 backward)
unsigned char right_motor = 0;			//direction of right motor (0 stop, 1 forward, 2 backward)

unsigned char left_trigger = 0;			//trigger left (0 open, 1 closed)
unsigned char right_trigger = 0;			//trigger right (0 open, 1 closed)
unsigned char left_infra = 0;				//trigger left (0 open, 1 closed)
unsigned char right_infra = 0;			//trigger right (0 open, 1 closed)

unsigned int left_line = 700;				//left line sensor brighness
unsigned int right_line = 700;				//right line sensor brighness
unsigned int battery_state_pre = 0;

unsigned char ad_pin = 0;					//switching between pin 0 (AD in 1) and pin 1 (AD in 2)	 and pin 3 (AD in 3)		

unsigned int battery_state = 0;			//ad value for battery

unsigned char sensor_fct = 0;				//is 1 when a sensor is trigged, due this, the drive fcts are not interrupted
unsigned char brght_left[5] = {}; 		//string for left brightness
unsigned char brght_right[5] = {};		//string for right brightness
unsigned char battery[5] = {};			//string for battery voltage

unsigned char i_left, i_right = 0;		//counting var

SIGNAL (SIG_ADC)
{
	switch (ad_pin)
	{
		case 0:
			right_line = ADCL;
			right_line |= (ADCH<<8);	
			ad_pin = 1;
			ADMUX = 0x41;
			break;
		case 1:
			left_line = ADCL;
			left_line |= (ADCH<<8);
			ad_pin = 0;
			ADMUX = 0x40;
			break;
		default:
			break;
	}
}

SIGNAL (SIG_OVERFLOW2)
{
	time_global++;
}

SIGNAL (SIG_OVERFLOW0)
{
	i_left++;
	i_right++;
	
	if (i_left <= speed_left)
	{
		set_out_pin(LEFT_M_ENA_PIN);
		
		switch (left_motor)
		{
			case 0:		//stop
			{
				reset_out_pin(LEFT_M_FOR_PIN); 		//forward
				reset_out_pin(LEFT_M_BACK_PIN); 		//backward
				break;
			}
			case 1:		//drive forward
			{
				set_out_pin(LEFT_M_FOR_PIN); 		//forward
				reset_out_pin(LEFT_M_BACK_PIN); 		//backward
				break;
			}
			case 2:		//drive backward
			{
				reset_out_pin(LEFT_M_FOR_PIN); 		//forward
				set_out_pin(LEFT_M_BACK_PIN); 		//backward
				break;
			}
			default:
				break;
		}
	}
	else
	{
		reset_out_pin(LEFT_M_ENA_PIN); 		//enabled
	}

	if (i_right <= speed_right)
	{
		set_out_pin(RIGHT_M_ENA_PIN);
		
		switch (right_motor)
		{
			case 0:		//stop
			{
				reset_out_pin(RIGHT_M_FOR_PIN); 		//forward
				reset_out_pin(RIGHT_M_BACK_PIN); 		//backward
				break;
			}
			case 1:		//drive forward
			{
				set_out_pin(RIGHT_M_FOR_PIN); 		//forward
				reset_out_pin(RIGHT_M_BACK_PIN); 		//backward
				break;
			}
			case 2:		//drive backward
			{
				reset_out_pin(RIGHT_M_FOR_PIN); 		//forward
				set_out_pin(RIGHT_M_BACK_PIN); 		//backward
				break;
			}
			default:
				break;
		}
	}
	else
	{
		reset_out_pin(RIGHT_M_ENA_PIN); 		//enabled
	}
	
	if (i_left == 101)
	{
		i_left = 0;
	}
	if (i_right == 101)
	{
		i_right = 0;
	}
	
	left_infra = get_in_pin(LEFT_INFRA_PIN);
	right_infra = get_in_pin(RIGHT_INFRA_PIN);
	left_trigger = get_in_pin(LEFT_TRIGGER_PIN);
	right_trigger = get_in_pin(RIGHT_TRIGGER_PIN);
	
}

void init_timer(void)
{
	TCNT2 = 0x00;
	TCCR2 = 0x04;
	
	TCNT0 = 0x00;
	TCCR0 = 0x02;
	
	TIMSK |= 0x41;
}

void speed(int spd_left, int spd_right)
{
	if (spd_left < 0)
	{
		spd_left = 0;
	}
	if (spd_left > 10)
	{
		spd_left = 10;
	}
	if (spd_right < 0)
	{
		spd_right = 0;
	}
	if (spd_right > 10)
	{
		spd_right = 10;
	}

	switch (spd_left)
	{
		case 1: speed_left = 12;
				break;
		case 2: speed_left = 20;
				break;
		case 3: speed_left = 30;
				break;
		case 4: speed_left = 40;
				break;
		case 5: speed_left = 50;
				break;
		case 6: speed_left = 60;
				break;
		case 7: speed_left = 70;
				break;
		case 8: speed_left = 80;
				break;
		case 9: speed_left = 90;
				break;
		case 10: speed_left = 100;
				break;
	}
	
	switch (spd_right)
	{
		case 1: speed_right = 12;
				break;
		case 2: speed_right = 20;
				break;
		case 3: speed_right = 30;
				break;
		case 4: speed_right = 40;
				break;
		case 5: speed_right = 50;
				break;
		case 6: speed_right = 60;
				break;
		case 7: speed_right = 70;
				break;
		case 8: speed_right = 80;
				break;
		case 9: speed_right = 90;
				break;
		case 10: speed_right = 100;
				break;
	}
}

void fct_wait_ts(int time)
{
	while(time_global<=((time*9)/10))
	{
#ifdef LEFT_INFRA
		if ((!sensor_fct) && (!left_infra))
		{
			break;
		}
#endif
#ifdef RIGHT_INFRA
		if ((!sensor_fct) && (!right_infra))
		{
			break;
		}
#endif
#ifdef LEFT_TRIGGER
		if ((!sensor_fct) && ((left_trigger)))
		{
			break;
		}
#endif
#ifdef RIGHT_TRIGGER
		if ((!sensor_fct) && ((right_trigger)))
		{
			break;
		}
#endif
#ifdef LEFT_LINE
		if ((!sensor_fct) && ((left_line >= LEFT_MIN) && (left_line <= LEFT_MAX)))
		{
			break;
		}
#endif
#ifdef RIGHT_LINE
		if ((!sensor_fct) && ((right_line >= RIGHT_MIN) && (right_line <= RIGHT_MAX)))
		{
			break;
		}
#endif
	}
}

void fct_drive(int time, int spd_left, int spd_right)
{
//	Left Motor direction
	if (spd_left < 0)
	{
		left_motor = 2;
	}
	if (spd_left > 0)
	{
		left_motor = 1;
	}
	if (spd_left == 0)
	{
		left_motor = 0;
	}
//	Right Motor	direction
	if (spd_right < 0)
	{
		right_motor = 2;
	}
	if (spd_right > 0)
	{
		right_motor = 1;
	}
	if (spd_right == 0)
	{
		right_motor = 0;
	}
	
	speed(spd_left, spd_right);

	time_global = 1;
	
	fct_wait_ts(time);
	
	left_motor = 0;
	right_motor = 0;

}

void fct_forward(int time, char spd)
{
	speed(spd, spd);
	
	left_motor = 1;
	right_motor = 1;
	
	time_global = 1;
	
	fct_wait_ts(time);
	
	left_motor = 0;
	right_motor = 0;
}

void fct_forward_left(int time, char spd)
{
	speed(spd, spd);

	left_motor = 0;
	right_motor = 1;
	
	time_global = 1;

	fct_wait_ts(time);

	left_motor = 0;
	right_motor = 0;
}

void fct_forward_right(int time, char spd)
{
	speed(spd, spd);

	left_motor = 1;
	right_motor = 0;
	
	time_global = 1;

	fct_wait_ts(time);
	
	left_motor = 0;
	right_motor = 0;
}

void fct_backward(int time, char spd)
{
	speed(spd, spd);

	left_motor = 2;
	right_motor = 2;
	
	time_global = 1;

	fct_wait_ts(time);

	left_motor = 0;
	right_motor = 0;
}

void fct_backward_left(int time, char spd)
{
	speed(spd, spd);

	left_motor = 0;
	right_motor = 2;
	
	time_global = 1;

	fct_wait_ts(time);

	left_motor = 0;
	right_motor = 0;
}

void fct_backward_right(int time, char spd)
{
	speed(spd, spd);

	left_motor = 2;
	right_motor = 0;
	
	time_global = 1;

	fct_wait_ts(time);

	left_motor = 0;
	right_motor = 0;
}

void fct_stop(int time)
{
	left_motor = 0;
	right_motor = 0;
	
	time_global = 1;

	fct_wait_ts(time);
}

void fct_turn_right(int time, char spd)
{
	speed(spd, spd);

	left_motor = 1;
	right_motor = 2;
	
	time_global = 1;

	fct_wait_ts(time);

	left_motor = 0;
	right_motor = 0;
}

void fct_turn_left(int time, char spd)
{
	speed(spd, spd);

	left_motor = 2;
	right_motor = 1;
	
	time_global = 1;

	fct_wait_ts(time);

	left_motor = 0;
	right_motor = 0;
}
