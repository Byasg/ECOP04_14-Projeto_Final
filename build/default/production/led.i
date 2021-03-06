# 1 "led.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files (x86)/Microchip/MPLABX/v5.35/packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "led.c" 2
# 1 "./pic18f4520.h" 1
# 1 "led.c" 2


    void ConfiguraLed(void){
        (*(volatile __near unsigned char*)0xF95) = 0x00;
        (*(volatile __near unsigned char*)0xF83) = 0xFF;
    }

    void LigarLed(char num){
        (((*(volatile __near unsigned char*)0xF83)) &= ~(1<<num));
    }

    void DesligarLed(char num){
        (((*(volatile __near unsigned char*)0xF83)) |= (1<<num));
    }
