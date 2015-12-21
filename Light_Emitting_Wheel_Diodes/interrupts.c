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


SIGNAL (INT0_vect)
{
	if (ovrflw_cnt > 20)	// Debouncing (~10,24ms)
	{
		//Calculate the time for one segment, 12 segments equal one revolution
		segment_times = ovrflw_cnt / 12;
		//Reset counter, time for the next revolution is measured
		ovrflw_cnt = 0;
		//Start of time segment 00
		if (switche_on == 1)
		{
			mLEDs_off();	//Reset all LEDs to start fresh
			mLED11_on();		
			mLED10_on();
			mLED9_on();
			mLED8_on();
		}
	}
}

SIGNAL (SIG_TIMER0_OVF) //8*0.064ms
{
	ovrflw_cnt++;
	if (switche_on == 1)
	{
		if (ovrflw_cnt == (segment_times * 1))		//Start of time segment 01
		{
			mLED12_on();
			mLED8_off();
		} 
		if (ovrflw_cnt == (segment_times * 2))		//Start of time segment 02
		{
			mLED1_on();
			mLED9_off();
		} 
		if (ovrflw_cnt == (segment_times * 3))		//Start of time segment 03
		{
			mLED2_on();
			mLED10_off();
		} 
		if (ovrflw_cnt == (segment_times * 4))		//Start of time segment 04
		{
			mLED3_on();
			mLED11_off();
		} 
		if (ovrflw_cnt == (segment_times * 5))		//Start of time segment 05
		{
			mLED4_on();
			mLED12_off();
		}
		if (ovrflw_cnt == (segment_times * 6))		//Start of time segment 06
		{
			mLED5_on();
			mLED1_off();
		} 
		if (ovrflw_cnt == (segment_times * 7))		//Start of time segment 07
		{
			mLED6_on();
			mLED2_off();
		} 
		if (ovrflw_cnt == (segment_times * 8))		//Start of time segment 08
		{
			mLED7_on();
			mLED3_off();
		} 
		if (ovrflw_cnt == (segment_times * 9))		//Start of time segment 09
		{
			mLED8_on();
			mLED4_off();
		} 
		if (ovrflw_cnt == (segment_times * 10))	//Start of time segment 10
		{
			mLED9_on();
			mLED5_off();
		} 
		if (ovrflw_cnt == (segment_times * 11))	//Start of time segment 11
		{
			mLED10_on();
			mLED6_off();
		}
	}
	if (switche_on == 0)
	{
		mLEDs_off();
	}
}