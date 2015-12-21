//=== Ports ===

#define SDI_PORT	PORTA
#define SDI_DDR		DDRA

#define SCK_PORT	PORTD
#define SCK_DDR		DDRD

#define CS_PORT		PORTD
#define CS_DDR		DDRD

#define SDO_PIN		PINA
#define SDO_DDR		DDRA
// === Pins ===
#define SDI		1
#define SCK		1
#define CS		0
#define SDO		0
//=== Macros ===
	// SDO
//#define clr_SDO(); SDO_PORT&=~(1<<SDO);
//#define set_SDO(); SDO_PORT|=(1<<SDO);
	// SDI
#define clr_SDI(); SDI_PORT&=~(1<<SDI);
#define set_SDI(); SDI_PORT|=(1<<SDI);
	// CS
#define clr_CS(); CS_PORT&=~(1<<CS);
#define set_CS(); CS_PORT|=(1<<CS);
	// SCK
#define clr_SCK(); SCK_PORT&=~(1<<SCK);
#define set_SCK(); SCK_PORT|=(1<<SCK);
	// macro for calculating frequency value out of frequency in MHz
#define RF12FREQ(freq)	((freq-860.0)/0.005) //see RF12b_programming-guide.pdf Page 2

unsigned short rf12_trans(unsigned short wert);					// transfer 1 word to/from module
void rf12_init(void);												// initialize module
void rf12_setfreq(unsigned short freq);							// set center frequency
void rf12_setbaud(unsigned short baud);							// set baudrate
void rf12_setpower(unsigned char power, unsigned char mod);		// set transmission settings
void rf12_setbandwidth(void);										// set receiver settings
//void rf12_txdata(unsigned char *data, unsigned char number);		// transmit number of bytes from array
void rf12_rxdata(unsigned char *data, unsigned char number);		// receive number of bytes into array
void rf12_ready(void);											// wait until FIFO ready (to transmit/read data)

unsigned short rf12_trans(unsigned short wert)
{	
	unsigned char i;

	clr_CS();
	
	for (i = 0; i < 16; i++)
	{	
		if (wert & 0x8000)
		{
			set_SDI();	//send '1'
		}
		else
		{
			clr_SDI();	//send '0'
		}
		wert <<= 1;		//shift
		
		if (SDO_PIN & (1 << SDO))
		{
			wert |= 1;
		}
		// generate clock signal SCK
		set_SCK();
		asm("nop");
		asm("nop");
		clr_SCK();
	}
	set_CS();
	return wert;
}

void rf12_init(void)
{
	// Init Prots and Pins
	SDI_DDR |= (1 << SDI);
	SCK_DDR |= (1 << SCK);
	CS_DDR |= (1 << CS);
	SDO_DDR &= ~(1 << SDO);
	
	CS_PORT |= (1 << CS);

	for (unsigned char i = 0; i < 50; i++)
	{
		_delay_ms(10);			// wait until POR done
	}
	rf12_trans(0xC0E0);			// AVR CLK: 10MHz
	rf12_trans(0x80E7);			// Enable FIFO
	rf12_trans(0xC2AB);			// Data Filter: internal
	rf12_trans(0xCA81);			// Set FIFO mode
	rf12_trans(0xE000);			// disable wakeuptimer
	rf12_trans(0xC800);			// disable low duty cycle
	rf12_trans(0xC4F7);			// AFC settings: autotuning: -10kHz...+7,5kHz
	
	rf12_setfreq(RF12FREQ(868.3));	// Sende/Empfangsfrequenz auf 868.3MHz einstellen
	rf12_setbandwidth();			// 200kHz Bandbreite, -6dB Verstärkung, DRSSI threshold: -79dBm 
	rf12_setbaud(19200);			// 19200 baud
	rf12_setpower(0, 6);			// 1mW Ausgangangsleistung, 120kHz Frequenzshift
}

void rf12_setbandwidth(void)
{
	// Receiver Control Command
	rf12_trans(0x9000 
						| ((0x04) << 8) 	/* P16 = 1: VDI Output, d1..d0 = 0x00: Responsetime fast */  
						| ((0x04) << 5) 	/* i2..i0 = 0x04: 200kHz */ 
						| ((0x01) << 3)	/* g1..g0 = 0x01: -6dB */ 
						| (0x04));  		/*r2..r0 = 0x04: -79dBm */ 
}

void rf12_setfreq(unsigned short freq)	// 36 <= freq <= 3903
{	
/*	if (freq < 36)				// 860...MHz
	{
		freq = 36;
	}
	else if (freq > 3903)			// 868...MHz
	{
		freq = 3903;
	} */
	
	rf12_trans(0xA000 | freq); //set freq
}

void rf12_setbaud(unsigned short baud)
{
	if (baud < 664)
	{
		baud = 664;
	}
	if (baud < 5400)					// Baudrate= 344827,58621/(R+1)/(1+CS*7)
	{
		rf12_trans(0xC680 | ((43104 / baud) - 1));	// R=(344828/8)/Baud-1
	}
	else
	{
		rf12_trans(0xC600 | ((344828UL / baud) - 1));	// R=344828/Baud-1
	}
}

void rf12_setpower(unsigned char power, unsigned char mod)
{	
	rf12_trans(0x9800 | (power & 7) | ((mod & 15) << 4));
}

void rf12_ready(void)
{	
	unsigned long timeout = 0;

 	clr_SDI();
	clr_CS();
  	asm("nop");
    //wait until FIFO ready or timeout
  	while (!(SDO_PIN & (1 << SDO)))
  	{
		if (((timeout++) > 70000))
		{
			break;
		}
 	}
}
/*
void rf12_txdata(unsigned char *data, unsigned char number)
{	
	unsigned char i;
	
	rf12_trans(0x8238);			// TX on
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB8AA);
	rf12_ready();
	rf12_trans(0xB82D);
	rf12_ready();
	rf12_trans(0xB8D4);
	for (i = 0; i < number; i++)
	{		
		rf12_ready();
		rf12_trans(0xB800 | (*data++));
	}
	rf12_ready();
	rf12_trans(0xB800);
	rf12_ready();
	_delay_us(10);
	rf12_trans(0x8208);			// TX off
}
*/
void rf12_rxdata(unsigned char *data, unsigned char number)
{	
	unsigned char i;
	
	rf12_trans(0x82C8);			// RX on
	rf12_trans(0xCA81);			// set FIFO mode
	rf12_trans(0xCA83);			// enable FIFO
	for (i = 0; i < number; i++)
	{	
		rf12_ready();
		*data++ = rf12_trans(0xB000);
	}
	rf12_trans(0x8208);			// RX off
}
