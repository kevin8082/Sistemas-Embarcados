#include <tm4c123gh6pm.h>
#include <inttypes.h>

int main(void)
{
    uint32_t aux;
    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R  = 0x08;
    GPIO_PORTF_DEN_R  = 0x08;

    while(1){
        GPIO_PORTF_DATA_R = 0x08; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x00; //desligado
        for(aux = 0; aux < 2000000; aux++){}
    }

	return 0;
}
