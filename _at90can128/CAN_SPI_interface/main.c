#define SPIDDR 	DDRB
#define SPISCK	PB1
#define SPISS	PB0
#define SPIMOSI	PB2
#define SPIMISO	PB3

#include <avr/io.h>			
#include <avr/interrupt.h>	

void SPI_MasterInit(void);
void SPI_MasterTransmit(char cData);

int main(void)
{
	DDRB |= (0x01 << PB4);
	
	SPI_MasterInit();
	while(1)
	{
		PORTB |= (0x01 << PB4);
		PORTB &= ~(0x01 << PB4);
		SPI_MasterTransmit('A');
	}
}

void SPI_MasterInit(void)
{
	// SCK, SS!!, MOSI as outputs
	SPIDDR |= (1<<SPISCK)|(1<<SPISS)|(1<<SPIMOSI);
	// MISO as input
	SPIDDR &= ~(1<<SPIMISO);	// cbi(SPIDDR,SPIMISO);
	// INIT interface, Master, set clock rate fck/128 TODO: check prescaler
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPR1);
}

void SPI_MasterTransmit(char cData)
{
	unsigned char dummy_read;
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	dummy_read = SPDR;
}