#include <pic18f4520.h>
#include "adc.h"
#include "bits.h"
#include "config.h"
#include "keypad.h"
#include "ds1307.h"
#include "lcd.h"
#include "ssd.h"
#include "timer.h"
#include "pwm.h"
#include "delay.h"

//define comandos para o LCD
#define L_CLR 0x01
#define L_L1 0x80
#define L_L2 0xC0

int i = 0;
char * processo[4] = {"Esquentar comida", "Pao de queijo", "Assar bolo", "aquecer"};
int t[4] = {5, 15, 30, 10};
char state = 0; //parametros para entrar nos whiles
int minutos = 0, segundos = 0; //variáveis do tempo
char start = 0; //parametro para contagem do tempo
char fim = 0; //indica o final do tempo

void inicio(void) { //FUNCAO QUE MOSTRA A DATA E HORA NO LCD
    //Mostra Data
    lcdCommand(L_L1);
    lcdString("    ");
    lcdChar(((getDays() / 10) % 10) + 48);
    lcdChar((getDays() % 10) + 48);
    lcdChar('/');
    lcdChar(((getMonths() / 10) % 10) + 48);
    lcdChar((getMonths() % 10) + 48);
    lcdChar('/');
    lcdChar(((getYears() / 10) % 10) + 48);
    lcdChar((getYears() % 10) + 48);

    //Mostra Hora e minutos              
    lcdCommand(L_L2);
    lcdString("     ");
    lcdChar(((getHours() / 10) % 10) + 48);
    lcdChar((getHours() % 10) + 48);
    lcdChar(':');
    lcdChar(((getMinutes() / 10) % 10) + 48);
    lcdChar((getMinutes() % 10) + 48);
}

void atualizaLCD(void) { //FUNCAO PARA ATUALIZAR O LCD
    lcdCommand(L_L1);
    lcdString("Modo escolhido: ");
    lcdCommand(L_L2);
    lcdString(processo[i]);
}

void leTeclado(void) { //Realiza leitura do keypad, chama a fun?ao correspondente
    static unsigned int tecla = 16;

    kpDebounce();
    if (kpRead() != tecla) {
        tecla = kpRead();
        if (bitTst(tecla, 9)) {
            state = 1;
            lcdCommand(L_CLR);
        }

        if (bitTst(tecla, 4)) {
            start = !start;
            fim = 0;
        }
    }
}

void escolherProcesso(void) { //Escolhe o processo a ser realizado
    static unsigned int tecla = 16;

    kpDebounce();
    if (kpRead() != tecla) {

        tecla = kpRead();
        lcdCommand(L_L1);
        lcdString("Escolha o modo: ");
        lcdCommand(L_L2);
        lcdString(processo[i]);
        minutos = t[i];
        segundos = 0;

        if (bitTst(tecla, 7)) {
            lcdCommand(L_CLR);
            if (i < 3) {
                i++;
            } else {
                i = 0;
            }
        }
    }
}

void atualizaSSD(void) { //Atualiza os valores do SSD
    ssdDigit(((minutos / 10) % 10), 0);
    ssdDigit(((minutos / 1) % 10), 1);
    ssdDigit(((segundos / 10) % 10), 2);
    ssdDigit(((segundos / 1) % 10), 3);
}

void timer(void) { //Realiza a contagem de tempo
    if (start != 0) {
        segundos--;
        if ((segundos < 0)&&(minutos > 0)) {
            segundos = 59;
            minutos--;
        }
        if ((segundos < 0)&&(minutos == 0)) {
            segundos = 0;
            start = !start;
            fim = 1;
            //atualizaLCD();
        }
    }
}

void main(void) {
    char slot = 0;
    unsigned int tempo = 0;

    adcInit(); //Inicializa o ADC    
    ssdInit(); //Inicializa o 7-seg display
    lcdInit(); //Inicializa o LCD
    kpInit(); //Inicializa o keypad
    timerInit(); //Inicializa o Timer
    pwmInit(); //Inicializa o pwm (buzzer)    



    PORTD = 0b00000001;

   
    while (state == 0) {
        inicio(); //Mostra data e hora no LCD
        leTeclado(); //Le tecla ligar (9)
    }
    lcdCommand(L_CLR); //Limpa o LDC
    state = 0;

    while (state == 0) {
        escolherProcesso();
        leTeclado();
    }
    state = 0;
    lcdCommand(L_CLR); //Limpa o LCD
    while (state == 0) {
        atualizaLCD();
        leTeclado(); //Le tecla start
    }

    for (;;) {
        timerReset(15000);
        switch (slot) {
            case 0:
                leTeclado(); //Ler o teclado  
                slot = 1;
                break;
            case 1:
                PORTD = 0b00000010; // lED fica verde
                if (tempo > 100) {
                    tempo = 0;
                    timer();
                }
                atualizaSSD(); //Atualiza valores no ssd                
                slot = 0;
                break;
            default:
                slot = 0;
                break;
        }
        if (start != 0) {
            tempo++;
        }
        ssdUpdate();
        timerWait();
    }
}
