#include <tm4c123gh6pm.h>
#include <inttypes.h>

void Trata_ST(){
    GPIO_PORTF_DATA_R ^= 0x08;
}

int main(void)
{
    NVIC_ST_RELOAD_R = 8000000;
    NVIC_ST_CTRL_R = 0x03;

    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R = 0x08;
    GPIO_PORTF_DEN_R = 0x08;

    while(1){

    }
	return 0;
}
