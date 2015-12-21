#define BAUD 103	//UBRR0 for 16MHz, U2X0 = 0
#define F_CPU 16000000
#define START_OF_MESSAGE 0x55	//Start of message

#include <avr/io.h>	
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "can_at90can128_AB_ID.c"

void TIMER0_ini (void);
void USART0_ini (void);

unsigned char send_data = 0;
unsigned char send_data_position = 0;
unsigned char msg_uart30[31] = {0x30,
								0x00, 0x00,		/* Cell 1 */
								0x00, 0x00,		/* Cell 2 */ 
								0x00, 0x00,		/* Cell 3 */
								0x00, 0x00,		/* Cell 4 */
								0x00, 0x00,		/* Cell 5 */
								0x00, 0x00,		/* Cell 6 */
								0x00, 0x00,		/* Cell 7 */
								0x00, 0x00,		/* Cell 8 */
								0x00, 0x00,		/* Cell 9 */
								0x00, 0x00,		/* Cell 10 */
								0x00, 0x00,		/* Cell 11 */
								0x00, 0x00,		/* Cell 12 */
								0x00, 0x00,		/* Cell 13 */
								0x00, 0x00,		/* Cell 14 */
								0x00, 0x00};	/* Cell 15 */

can_message msg_test, msg_bat_1, msg_bat_2;
uint8_t timer0_cnt = 0;