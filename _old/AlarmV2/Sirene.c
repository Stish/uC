#include <stdlib.h>
#include <stdint.h>
#include <Stdio.h>

void Pause(void) //etwa 0,5s
{
  uint8_t i;
  uint8_t j;
  for(i=0;i<255;i++) 
  {
    for(j=0;j<255;j++) {}
  }
}

void Sirene(int ton){
  for(;;){                      // tue nix Schleife
    ton = 200;
    Pause();
    ton = 215;
    Pause();
    ton = 230;
    Pause();
  }
}


