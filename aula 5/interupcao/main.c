#include <tm4c123gh6pm.h>
#include <inttypes.h>

void delay(){
    uint32_t cont;
    for(cont = 0; cont < 2000000; cont++);
}

void Trata_portf(){
    if ((GPIO_PORTF_RIS_R & 0x10) == 0x10){
    GPIO_PORTF_ICR_R = 0x10;
    GPIO_PORTF_DATA_R ^= 0x08;
    }
 }

int main(void)
{
    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R = 0x08;
    GPIO_PORTF_PUR_R = 0x10;

    GPIO_PORTF_IS_R = 0x00;
    GPIO_PORTF_IEV_R = 0x00;
    GPIO_PORTF_IBE_R = 0x00;

    GPIO_PORTF_IM_R = 0x10;
    GPIO_PORTF_DEN_R = 0x18;

    NVIC_EN0_R = 1 << 30;

    while(1){

    }
    return 0;
}
