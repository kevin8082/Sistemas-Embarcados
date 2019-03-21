#include <tm4c123gh6pm.h>
#include <inttypes.h>

int main(void)
{
    uint32_t aux;
    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R = 0x08;
    GPIO_PORTF_PUR_R = 0x10;
    GPIO_PORTF_DATA_R = 0x00;
    GPIO_PORTF_DEN_R = 0x18;

    while(1){
        if((GPIO_PORTF_DATA_R & 0x10) == 0x00 ){
            GPIO_PORTF_DATA_R = 0x08; //ligado
        }else{
            GPIO_PORTF_DATA_R = 0x00; //desligado
        }
    }
    return 0;
}
