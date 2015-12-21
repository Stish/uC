#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdint.h>
#include <Stdio.h>
#include <stdbool.h>
#include "BinaryNr.h"

#define F_CPU            2457600         // QuarzTakt 
#define UART_BAUD_RATE     19200         // Baudrate zum Handy  
#define UART_BAUD_SELECT (F_CPU/(UART_BAUD_RATE*16l)-1)
#define Prescaler 1024;                  // Timer 1 Vorteiler
#define startwert 65537-F_CPU/Prescaler; //Zähler Startwert, Überlauf bei 65536
#include "uart_routinen.c"
#include "unterprogramme.h"
