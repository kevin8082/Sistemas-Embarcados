#include <tm4c123gh6pm.h>
#include <inttypes.h>

int main(void)
{
    uint32_t aux;
    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_DEN_R = 0x0E;

    while(1){
        GPIO_PORTF_DATA_R = 0x00; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x02; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x04; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x06; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x08; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x0A; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x0C; //ligado
        for(aux = 0; aux < 2000000; aux++){}
        GPIO_PORTF_DATA_R = 0x0E; //ligado
        for(aux = 0; aux < 2000000; aux++){}
    }

    return 0;
}
