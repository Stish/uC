#define F_CPU 16000000

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>

// SPI Pin configuration
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_MOSI 2
#define SPI_SCK 0
#define SPI_MISO 4
// MAX186 control register bits
#define MAX186_START 7
#define MAX186_SEL2 6
#define MAX186_SEL1 5
#define MAX186_SEL0 4
#define MAX186_UNI_BI 3
#define MAX186_SGL_DIF 2
#define MAX186_PD1 1
#define MAX186_PD0 0

unsigned int SW_SPI_ReadMAX186(char cData);
void SW_SPI_Wait(void);
void SW_SPI_MasterInit(void);
void SW_SPI_SCK_Strope(void);
