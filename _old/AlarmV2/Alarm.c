/* 
; Bollealarm
;
; Hardware

;              ___________
;       __    /           |  
;+5V O-|__|--|RESET    PC5|--O     
;            |            |             
;  Handy  O--|PD0      PC4|--O     
;            |            |             
;  Handy  O--|PD1      PC3|--O    
;            |            |             
;  Sirene O--|PD2      PC2|--O Analog Alarmkontakt     
;            |            |              
;    Rel1 O--|PD3      PC1|--O Alarmkontakt      
;            |    A T     |              
;         O--|PD4      PC0|--O Ein/Aus     
;            |   mega     |              
;     +5V O--|VCC      GND|--O GND     
;            |     8      |             
;     GND O--|GND     AREF|--O           
;            |            |              
;         O--|XTAL1   AVCC|--O        
;            |            |          
;         O--|XTAL2 PB5SCK|--O         
;            |            |             
;     LED O--|PD5  PB4MISO|--O          
;            |            |             
;         O--|PD6  PB3MOSI|--O          
;            |            |             
;         O--|PD7  PB2OC1B|--O          
;            |            |             
;         O--|PB0  PB1OC1A|--O 
;            |____________|
;
    .-------------------------------------------------.
    |                                                 |
    |                                                 |
    |                                            Handy|
    |                                            .---.|
    |                                            |   ||
    |                                            |   ||
    |                                            |   ||
    |                                            |   ||
    |     GND   + Aus              Sir.  PC0   PC2   ||
    |  Batt  Zus.Batt     Rel.        LED   PC1  '---'|
    | .--.--.--.--.      .--.--.--.--.--.--.--.--.    |
    | |+ |  |+ |+ |      |  |+ |+ |  |+ |  |  |  |    |
    | '--'--'--'--'      '--'--'--'--'--'--'--'--'    |
    '-------------------------------------------------'

(created by AACircuit v1.28.6 beta 04/19/05 www.tech-chat.de)

*/
#define Alarmdauer 3  // Alarmdauer in Sekunden
#define Sirenentyp 1  // 1= aktive 12V Sirene; 2=Piezolautsprecher
#define Abheben 3     // nach dem 2. Klingeln abheben

#include "Alarm.h"

/*********************/
int main(){
  port_init();                  // Ein-Ausgabeport initilisieren
  uart_init();                  // Serielle Schnittstelle für Handy
  timer1_init();                // Timer init. Interrupt jede Sekunde
  ad_init();
  Relais_OFF();                 // Relais1 in Ruhe aus
  Pause(200);
  sei();                        // enable interrupts  
  for(;;){                      // Endlosschleife

    if (Handy_Ring()){          // bei Anruf
	  Anrufzaehler++;           // Zähler erhöhen
	}

	if (Anrufzaehler >=Abheben){
	  uart_puts("ATA\r\n");     // Sende "ATA" an Handy (abheben)
      Anrufzaehler=0;
    }

    if (scharf()){
      if (Alarm()){             // Alarmfall eingetreten, Anlage scharf
	    Zeit = 0;               // Uhr auf 0 stellen  
        Relais_ON();            // Relais zieht an
	 	if(!Sirene_gesperrt){   // wenn nicht gesperrt
		  rufe_an();            // erste Nr. im Tel.Buch wählen
	      Sirene(Alarmdauer);   // Sirene für XX Sek einschalten (Zeit=0)
          Sirene_gesperrt=true; // Kein Alarm mehr möglich    
		  merke_Zeit=Zeit;      // beim ersten Alarm aktuelle Zeit merken		         
        }
      } 
	  if (!Alarm()){            // Alarmsignal ist weg
  	    Sirene_OFF();           // Sirene aus
      }
      if (Handy_OK()){          // LED blinkt wenn Handy OK
        LED_ON();               // LED einschalten
        Pause(100);             // kurze Pause
        LED_OFF();              // LED ausschalten
        Pause(100);             // kurze Pause 
      }
    }

	if (!scharf()){             // Anlage deaktiviert
	  Sirene_OFF();             // Sirene aus
  	  Relais_OFF();             // Relais in Ruhe aus
	  Sirene_gesperrt=false;    // Alarmsirene wieder möglich
	  if (Handy_OK()){          // LED blinkt wenn Handy OK
        LED_ON();               // LED einschalten
        Pause(100);             // kurze Pause
	    LED_OFF();              // LED ausschalten
        Pause(5000);            // kurze Pause
      } 
    }
  }
}



