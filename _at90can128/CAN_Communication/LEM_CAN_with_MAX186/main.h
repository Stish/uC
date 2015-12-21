#define F_CPU	16000000	//Board frequency
// SPI Pin configuration
#define SPI_DDR DDRA
#define SPI_PORT PORTA
#define SPI_PIN PINA
#define SPI_MOSI 3
#define SPI_SCK 5
#define SPI_MISO 1
// MAX186 control register bits
#define MAX186_START 7
#define MAX186_SEL2 6
#define MAX186_SEL1 5
#define MAX186_SEL0 4
#define MAX186_UNI_BI 3
#define MAX186_SGL_DIF 2
#define MAX186_PD1 1
#define MAX186_PD0 0

#include <avr/io.h>			//IO Ports library
#include <avr/interrupt.h>						//library for interrupt functions
#include "can_at90can128_AB_ID.c"
#include <util/delay.h>

void TIMER0_ini (void);
unsigned int SW_SPI_ReadMAX186(char cData);
void SW_SPI_Wait(void);
void SW_SPI_MasterInit(void);
void SW_SPI_SCK_Strope(void);

unsigned char val_ad_01_bcd[6] = "0000+";
signed int val_ad_storage[10];
unsigned char val_ad_storage_index = 0;

unsigned char timer0_cnt = 0;

can_message msg;
