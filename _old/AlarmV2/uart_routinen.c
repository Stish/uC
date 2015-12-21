#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(void){		// initialize uart 
  UCSRB |= (1 << TXEN );	// UART TX einschalten
  UCSRB |= (1 << RXEN );    // RX enable
  UCSRB |= (1<<RXCIE);      // enable empfangs-interrupt
  UBRRL =UART_BAUD_SELECT;  // setze Baudrate
}

// Zeichen ausgeben
int uart_putc(unsigned char c)
{
    while(!(UCSRA & (1 << UDRE))); /* warte, bis UDR bereit */
    UDR = c;                       /* sende Zeichen */
    return 0;
}

// Zeichenkette ausgeben
void uart_puts (char *s)
{
    while (*s)
    {   /* so lange *s != '\0' also ungleich dem "String-Endezeichen" */
        uart_putc(*s);
        s++;
    }
}
