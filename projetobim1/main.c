#include <main.h>
#include <tm4c123gh6pm.h>

/**
 * main.c
 */
void delay(){
    for(count1 = 0; count1 < 500000; count1++);
}

void startup(){
    level = 1;
    cores[0] = 1;
    mostrarOrdem = 1;
    entrada = 0;
    posAtual = 0;
    ultimoBotao = 0;
    controleCor = 1;
    count2 = 0;
    for(count2 = 0; count2 < 6;count2++){
        GPIO_PORTA_DATA_R ^= 0x3C;
        delay();
    }
}

void piscaLedVermelho(){
    GPIO_PORTA_DATA_R ^= 0x04;
    delay();
    GPIO_PORTA_DATA_R ^= 0x04;
}

void piscaLedAzul(){
    GPIO_PORTA_DATA_R ^= 0x08;
    delay();
    GPIO_PORTA_DATA_R ^= 0x08;
}

void piscaLedVerde(){
    GPIO_PORTA_DATA_R ^= 0x10;
    delay();
    GPIO_PORTA_DATA_R ^= 0x10;
}

void piscaLedAmarelo(){
    GPIO_PORTA_DATA_R ^= 0x20;
    delay();
    GPIO_PORTA_DATA_R ^= 0x20;
}

void Trata_portc(){

    //Botão 1 PC4
    if ((GPIO_PORTC_RIS_R & 0x10) == 0x10){
        GPIO_PORTC_ICR_R = 0x10;
        //Vermelho PA2
        piscaLedVermelho();
        ultimoBotao = 1;
        if(level == 1){
            cores[1] = novaCor;
        }
        if(level > 1 && cores[level] == 0 && controleCor == 0){
            cores[level] = novaCor;
            controleCor = 1;
        }
    }
    //Botão 2 PC5
    if ((GPIO_PORTC_RIS_R & 0x20) == 0x20){
        GPIO_PORTC_ICR_R = 0x20;
        //Azul PA3
        piscaLedAzul();
        ultimoBotao = 2;
    }
    //Botão 3 PC6
    if ((GPIO_PORTC_RIS_R & 0x40) == 0x40){
        GPIO_PORTC_ICR_R = 0x40;
        //Verde PA4
        piscaLedVerde();
        ultimoBotao = 3;
    }
    //Botão 4 PC7
    if ((GPIO_PORTC_RIS_R & 0x80) == 0x80){
        GPIO_PORTC_ICR_R = 0x80;
        //Amarelo PA5
        piscaLedAmarelo();
        ultimoBotao = 4;
    }
}

int main(void)
{
    SYSCTL_RCGCGPIO_R = 0x05;

    GPIO_PORTA_DIR_R = 0x3C;
    GPIO_PORTA_DEN_R = 0x3C;

    GPIO_PORTC_DIR_R = 0x00;
    GPIO_PORTC_PDR_R = 0xF0;

    GPIO_PORTC_IS_R = 0x10;
    GPIO_PORTC_IEV_R = 0x10;
    GPIO_PORTC_IBE_R = 0x00;

    GPIO_PORTC_IM_R = 0xF0;
    GPIO_PORTC_DEN_R = 0xF0;



    while(1){
        if(novaCor < 5){
        novaCor++;
        }else{novaCor = 1;}

        if(level == 0){
            startup();
        }

        if(level > 20){
            for(count2 = 0; count2 < 6;count2++){
                GPIO_PORTA_DATA_R ^= 0x3C;
                delay();
            }
            level = 0;
        }

        if(mostrarOrdem == 1 && entrada == 0){
            delay();
            count2 = 0;
            while(count2 < level){
                if(cores[count2] == 1){
                    piscaLedVermelho();
                }
                if(cores[count2] == 2){
                    piscaLedAzul();
                }
                if(cores[count2] == 3){
                    piscaLedVerde();
                }
                if(cores[count2] == 4){
                    piscaLedAmarelo();
                }
                delay();
                count2++;
            }
            mostrarOrdem = 0;
            entrada = 1;
            NVIC_EN0_R = 1 << 2;
        }

        if (entrada == 1 && mostrarOrdem == 0 && ultimoBotao != 0){
            while((posAtual < level && ultimoBotao != 0)){
                if(cores[posAtual] == ultimoBotao){
                    posAtual++;
                    ultimoBotao = 0;
                }else{
                    for(count2 = 0; count2 < 6;count2++){
                        GPIO_PORTA_DATA_R ^= 0x04;
                        delay();
                    }
                    level = 0;
                }
            }
            if(posAtual == level){
                for(count2 = 0; count2 < 6;count2++){
                    GPIO_PORTA_DATA_R ^= 0x10;
                    delay();
                }
                level++;
                mostrarOrdem = 1;
                entrada = 0;
                ultimoBotao = 0;
                controleCor = 0;
                posAtual = 0;
                NVIC_EN0_R = 0x0;
            }
        }


      }
	return 0;
}
