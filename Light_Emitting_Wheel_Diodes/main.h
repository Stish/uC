//	=== Pin configurations on the ATTiny2313 and schematic of the project ===
//
//                                                                       VCC
//                                                                        +
//        VCC                                       _____   _____         |
//         +                            _____      |     \_/     |        |
//         |                            Reset O--1-|PA2      VCC |-20-O --'
//  .------o------.                                |             |                           LED8
//  |             |              CS ----- RXD O--2-|PD0       PB7|-19-O UCSK/SCL/PCINT7 ----->|---.
//  |  .-----.    |                                |             |                           LED7 |
//  |  | HAL |   .-.            SCK ----- TXD O--3-|PD1       PB6|-18-O MISO/DO/PCINT6 ------>|---o
//  |  |     |   | | 1.8k                          |             |                           LED6 |
//  |  |1 2 3|   | |            SDI --- XTAL2 O--4-|PA1       PB5|-17-O MOSI/DI/PCINT5 ------>|---o
//  |  '-----'   '-'                               |             |                           LED5 |
//  |   | | | Q   |             SDO --- XTAL1 O--5-|PA0       PB4|-16-O OC1B/PCINT4 --------->|---o
//  |   | | |     |                                |             |                           LED4 |
//  '---' | '-----o----------- CKOUT/XCK/INT0 O--6-|PD2       PB3|-15-O OC1A/PCINT3 --------->|---o
//        |              LED9                      |             |                           LED3 |
//        |         .----|<------------- INT1 O--7-|PD3       PB2|-14-O OC0A/PCINT2 --------->|---o
//        |         |    LED10                     |             |                           LED2 |
//        '---------o----|<--------------- T0 O--8-|PD4       PB1|-13-O AIN1/PCINT1 --------->|---o
//                  |    LED11                     |             |                           LED1 |
//                  o----|<---------- OC0B/T1 O--9-|PD5       PB0|-12-O AIN0/PCINT0 --------->|---o
//                  |                              |             |                           LED12|
//                  |                     .-- O-10-|GND       PD6|-11-O ICP ----------------->|---o
//                  |                     |        |_____________|                                |
//                  |                     |          ATTiny 2313                                  |
//                  |                     |                                                       |
//                 ===                   ===                                                     ===
//                 GND                   GND                                                     GND

// === LED Pins === //
#define PIN_LED1 0
#define PIN_LED2 1
#define PIN_LED3 2
#define PIN_LED4 3
#define PIN_LED5 4
#define PIN_LED6 5
#define PIN_LED7 6
#define PIN_LED8 7
#define PIN_LED9 3
#define PIN_LED10 4
#define PIN_LED11 5
#define PIN_LED12 6

// === LED Ports === //
#define PORT_LED1 PORTB
#define PORT_LED2 PORTB
#define PORT_LED3 PORTB
#define PORT_LED4 PORTB
#define PORT_LED5 PORTB
#define PORT_LED6 PORTB
#define PORT_LED7 PORTB
#define PORT_LED8 PORTB
#define PORT_LED9 PORTD
#define PORT_LED10 PORTD
#define PORT_LED11 PORTD
#define PORT_LED12 PORTD

// === LED Ports (ini) === //
#define PORT_INI_LED1 DDRB
#define PORT_INI_LED2 DDRB
#define PORT_INI_LED3 DDRB
#define PORT_INI_LED4 DDRB
#define PORT_INI_LED5 DDRB
#define PORT_INI_LED6 DDRB
#define PORT_INI_LED7 DDRB
#define PORT_INI_LED8 DDRB
#define PORT_INI_LED9 DDRD
#define PORT_INI_LED10 DDRD
#define PORT_INI_LED11 DDRD
#define PORT_INI_LED12 DDRD

// === LEDs off makros === //
#define mLED1_off(); PORT_LED1&=~(0x01<<PIN_LED1);
#define mLED2_off(); PORT_LED2&=~(0x01<<PIN_LED2);
#define mLED3_off(); PORT_LED3&=~(0x01<<PIN_LED3);
#define mLED4_off(); PORT_LED4&=~(0x01<<PIN_LED4);
#define mLED5_off(); PORT_LED5&=~(0x01<<PIN_LED5);
#define mLED6_off(); PORT_LED6&=~(0x01<<PIN_LED6);
#define mLED7_off(); PORT_LED7&=~(0x01<<PIN_LED7);
#define mLED8_off(); PORT_LED8&=~(0x01<<PIN_LED8);
#define mLED9_off(); PORT_LED9&=~(0x01<<PIN_LED9);
#define mLED10_off(); PORT_LED10&=~(0x01<<PIN_LED10);
#define mLED11_off(); PORT_LED11&=~(0x01<<PIN_LED11);
#define mLED12_off(); PORT_LED12&=~(0x01<<PIN_LED12);

// === LEDs on makros === //
#define mLED1_on(); PORT_LED1|=(0x01<<PIN_LED1);
#define mLED2_on(); PORT_LED2|=(0x01<<PIN_LED2);
#define mLED3_on(); PORT_LED3|=(0x01<<PIN_LED3);
#define mLED4_on(); PORT_LED4|=(0x01<<PIN_LED4);
#define mLED5_on(); PORT_LED5|=(0x01<<PIN_LED5);
#define mLED6_on(); PORT_LED6|=(0x01<<PIN_LED6);
#define mLED7_on(); PORT_LED7|=(0x01<<PIN_LED7);
#define mLED8_on(); PORT_LED8|=(0x01<<PIN_LED8);
#define mLED9_on(); PORT_LED9|=(0x01<<PIN_LED9);
#define mLED10_on(); PORT_LED10|=(0x01<<PIN_LED10);
#define mLED11_on(); PORT_LED11|=(0x01<<PIN_LED11);
#define mLED12_on(); PORT_LED12|=(0x01<<PIN_LED12);

// === LEDs off all makro === //
#define mLEDs_off(); PORT_LED1&=~(0x01<<PIN_LED1);PORT_LED2&=~(0x01<<PIN_LED2);PORT_LED3&=~(0x01<<PIN_LED3);PORT_LED4&=~(0x01<<PIN_LED4);PORT_LED5&=~(0x01<<PIN_LED5);PORT_LED6&=~(0x01<<PIN_LED6);PORT_LED7&=~(0x01<<PIN_LED7);PORT_LED8&=~(0x01<<PIN_LED8);PORT_LED9&=~(0x01<<PIN_LED9);PORT_LED10&=~(0x01<<PIN_LED10);PORT_LED11&=~(0x01<<PIN_LED11);PORT_LED12&=~(0x01<<PIN_LED12);

// === Global variables === //
volatile unsigned long ovrflw_cnt = 0;
volatile unsigned long segment_times = 1250;
//volatile unsigned char chr = '1';
volatile unsigned char switche_on = 1;
//volatile unsigned char chr = '1';

// === Libraries === //
#include <avr/interrupt.h>
#include "functions.c"
#include "interrupts.c"
//#include <avr/signal.h>
//#include <avr/io.h>
//#include <stdlib.h>