uint8_t ton,y,Auszeit;
int Zeit,Empfang,merke_Zeit,Anrufzaehler;
char s[7];
bool Sirene_gesperrt;
int ADmax =950;
int ADmin =400;

void port_init(void){
  DDRB = B00000000;                 // PORTB definieren
  DDRD = B11111100;                 // PORTD Ausgang					
  DDRC = B00000000;                 // PORTC alles Eingang
  PORTC= B00000001;                 // Pullup PC0 (Ein/aus)
}

void timer1_init(void){             // timer 1 overflow 
  TCCR1B = B00000101;               // Prescaler auf 1024 stellen
  TIMSK |= (1 << TOIE1);            // Timer 1 Overflow Interrupt enable
  TCNT1  = startwert;               // vorbesetzen (zählt bis FFFF)
}

void ad_init(void){                 // initialize ad-wandler 
  ADMUX = 0x02;                     // Kanal2
  ADMUX |= (1<<REFS1)|(1<<REFS0);   // interne Referenzspannung 2,5V nutzen 
  ADCSRA=B11000111;                 // enable ADC
}

void Pause(int x){                  // kurze Pause
  int i;
  int j;
  for(i=0;i<x;i++) 
  {
    for(j=0;j<120;j++) {}
  }
}

int leseAD(void){ 
  uint16_t wert;
  uint8_t  highwert;
  ADCSR |= (1<<ADSC);               // eine Wandlung starten
  while ( ADCSR & (1<<ADSC) ) {;}   // auf Abschluss der Konvertierung warten
  wert     = ADCL;                  // Low  auslesen	
  highwert = ADCH;                  // High auslesen
  wert = wert + highwert*256;//	
  return wert;
}

bool Alarm(){                       // prüft den Alarmzustand
  bool alarm;
  int adwert;
  adwert = leseAD();
  alarm = false;
  if (bit_is_clear(PINC,PC1)) alarm=true;
  if (ADmin > adwert || adwert > ADmax){   // wenn Analogwert ausserhalb Bereich
     Pause (1);                            // eine ms warten 
     adwert = leseAD();                    // nochmal prüfen
     if (ADmin > adwert || adwert > ADmax){// wenn Analogwert ausserhalb Bereich
     alarm = true;
     }
  }
  return alarm;
}

bool scharf(){
  bool scharf;
  if (bit_is_clear(PINC,PC0)) scharf=false;  //Zustand der Anlage
  if (bit_is_set(PINC,PC0))   scharf=true;
  return scharf;
}

//Interrupt wenn Timer2 überläuft
ISR(TIMER2_OVF_vect)
{
      PORTD ^= (1<<2);             // Bit 2 setzen
      TCNT2 = ton;                 // Zaehler   
}

bool Handy_OK(void){
   bool t=false;
   uart_puts("at\r\n");            // Sende "at" an Handy
   Pause(100);                     // pro Zeichen mind. 10
   if (Empfang == 4){              // OK empfangen				
      t=true;
      Empfang = 0;
   }
   return t;
}

bool Handy_Ring(void){
   bool t=false;
   if (Empfang == 10){              // RING empfangen				
      t=true;
      Empfang = 0;
   }
   return t;
}

void rufe_an(void){                 // wählt den ersten Eintrag im Telefonbuch
   uart_puts("atd>1;\r\n");         // sende atd>1: ans Handy
}

//Interrupt wenn Zeichen per uart empfangen
SIGNAL(USART_RXC_vect){
    cli();
    y = UDR;                        // lese byte aus UART
    switch(Empfang){
      case(0):
        switch(y){
          case('O'):
            Empfang=1;
            break;
          case('R'):
            Empfang=5;
            break;
          default:
          Empfang = 0;
        }
        break;
        
      case(1):  
        if (y=='K') Empfang=2;
        else Empfang = 0;
        break;

      case(2):  
        if (y==13) Empfang=3;
        else Empfang = 0;
        break;

      case(3):  
        if (y==10) Empfang=4;
        else Empfang = 0;
        break;
				        
      case(5):		
        if (y=='I') Empfang=6;
        else Empfang = 0;
        break;
        
      case(6):
        if (y=='N') Empfang=7;
        else Empfang = 0;
        break;
        
      case(7):
        if (y=='G') Empfang=8;
        else Empfang = 0;
        break;

      case(8):  
        if (y==13) Empfang=9;
        else Empfang = 0;
        break;

      case(9):  
        if (y==10) Empfang=10;
        else Empfang = 0;
        break;

      default:
        Empfang = 0;
    }
      sei();

}

void Relais_ON(void){
  PORTD |=  (1 << 3);
}
void Relais_OFF(void){
  PORTD &= ~(1 << 3);
}

void LED_ON(void){
  PORTD |=  (1 << 5);
}

void LED_OFF(void){
  PORTD &= ~(1 << 5);
}

void Sirene_ON(void){
  PORTD |=  (1 << 2);
}

void Sirene_OFF(void){
  PORTD &= ~(1 << 2);
}

void Sirene1(int sek){              // Sireneausgang durchschalten
  Auszeit=sek;
  Sirene_ON();
}

void Sirene2(int sek){              // Auf- und abschwellender Ton
  int z = 0;
  int a = 0;
  TIMSK |= (1 << TOIE2);            // Timer2 Overflow freigeben
  TCCR2 = B00000011;                // Vorteiler Timer2 CLK/32 
  for (z=0;z <sek; z++){            // Sirenenton erzeugen
    for (a=0;a <=50; a++){
      ton = 180+a;
      Pause(20);
    }
    for (a=50;a >=0; a--){
      ton = 180+a;
      Pause(20);
    }
  }
  TIMSK &= ~(1 << TOIE2);           // Timer2 Overflow sperren 
}

void Sirene(int sek){
  if (Sirenentyp == 1){
    Sirene1(sek);                   // aktive Sirene für XX Sek einschalten (Zeit=0)
  }
  if (Sirenentyp == 2){
    Sirene2(sek);                   // Piezo-Sirene für XX Sek einschalten (Zeit=0) 
  }
}

//Timer Interrupt - kommt genau jede Sekunde
SIGNAL (TIMER1_OVF_vect){           // Timer1 overflow 
  cli();                            // Alle Int. sperren solange in INT-Routine
  TCNT1 = startwert;                // Vorbesetzen (1s)
  Zeit++;
  if (Zeit > Auszeit){
    Sirene_OFF();
  }
  sei();                            // enable interrupts
}
