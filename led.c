#include"pic18f4520.h"

    void ConfiguraLed(void){
        TRISD = 0x00;
        PORTD = 0xFF;
    }
    
    void LigarLed(char num){
        BitClr(PORTD, num);  
    }
    
    void DesligarLed(char num){
        BitSet(PORTD, num);
    }

