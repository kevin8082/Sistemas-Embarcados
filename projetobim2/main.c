#include <tm4c123gh6pm.h>
#include <inttypes.h>

uint32_t count, leiturax, leituray, tempo = 400000, ordem[200], posicao = 0;
int i;

//todo santo port utilizado
//display
//04 RS   - PA5
//05 R/W  - PA7
//06 E    - PA6
//07 DB0  - PB0
//08 DB1  - PB1
//09 DB2  - PB2
//10 DB3  - PB3
//11 DB4  - PB4
//12 DB5  - PB5
//13 DB6  - PB6
//14 DB7  - PB7

//analógico
//VRx - PE2 - AIN1
//VRy - PE1 - AIN2

//digital
//cima     - PF2 - 1
//baixo    - PF3 - 2
//direita  - PC4 - 3
//esquerda - PC5 - 4
//A        - PC6 - 5
//B        - PC7 - 6
//C        - PD6 - 7
//D        - PE0 - 8
//E        - PF4 - 9
//F        - PA3 - 10
//start    - PA2 - 11

//delay pra usa nos pontos necessários
void delay(int n){
    for(count = 0; count < n; count++){}
}

#define UART_FR_TXFF            0x00000020          // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010          // UART Receive FIFO Empty

//função de inicializa o uart do prof
void UART0_Init(void){
    SYSCTL_RCGCUART_R |= 0x01;                        // activate UART0
    SYSCTL_RCGCGPIO_R |= 0x01;                        // activate port A
    GPIO_PORTA_AFSEL_R |= 0x03;                       // enable alt funct on PA1-0
    GPIO_PORTA_PCTL_R |= 0x11;                        // enable UART funct on PA1-0

    UART0_CTL_R = 0x00;                               // disable UART
    UART0_IBRD_R = 8;                                 // IBRD = int(16.000.000 / (16 * 115.200)) = int(8,6805)
    UART0_FBRD_R = 44;                                // FBRD = int(0,6805 * 64 + 0.5) = 44
    UART0_LCRH_R = 0x70;                              // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART0_CC_R = 0x00;
    UART0_CTL_R = 0x301;                              // enable UART

    GPIO_PORTA_DEN_R |= 0x03;                         // enable digital I/O on PA1-0

}

//função de saida de dados do uart do prof
void UART0_OutChar(unsigned char data){
    while ((UART0_FR_R & UART_FR_TXFF) != 0) { }
    UART0_DR_R = data;
}

#define ENABLE GPIO_PORTA_DATA_R^=0x40

//função de pulso do lcd do prof
void pulso_enable()
{
    ENABLE;
    delay(20000);
    ENABLE;
}

//função do lcd do prof
void cmd_LCD(unsigned char c, int count)
{
    if (count == 1)
    {
        GPIO_PORTA_DATA_R = 0x00;
        delay(4000);
        GPIO_PORTB_DATA_R = 0x01;
        pulso_enable();
        GPIO_PORTB_DATA_R = 0x02;
        pulso_enable();
    }
    if (count == 16)
    {
        GPIO_PORTA_DATA_R = 0x00;
        delay(4000);
        GPIO_PORTB_DATA_R = 0xC0;
        pulso_enable();
    }
    GPIO_PORTA_DATA_R = 0x20;
    delay(4000);
    GPIO_PORTB_DATA_R = c;
    pulso_enable();

}

//função do lcd do prof
void escreve_LCD(char *c)
{
    int count = 0;
    for (; *c != 0; c++)
    {
        count++;
        cmd_LCD(*c, count);

    }
}

//setup do lcd do prof
void inicializa_LCD()
{
    SYSCTL_RCGCGPIO_R |= 0x03;
    GPIO_PORTB_DATA_R = 0x00;
    GPIO_PORTA_DATA_R = 0x00;
    GPIO_PORTA_DIR_R |= 0xE0;
    GPIO_PORTB_DIR_R |= 0xFF;
    GPIO_PORTB_DEN_R |= 0xFF;
    GPIO_PORTA_DEN_R |= 0xE0;

    delay(60500);
    GPIO_PORTB_DATA_R = 0x30;

    delay(100000);
    pulso_enable();

    delay(20000);
    pulso_enable();

    delay(20000);
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x3C;
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x08;
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x01;
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x07;
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x0C;
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x06;
    pulso_enable();

    GPIO_PORTB_DATA_R = 0x01;
    pulso_enable();

    escreve_LCD("INICIALIZADO, AGUARDE...");

}

//define só pra ter isso como algo legível em vez e registrador
#define leiturax ADC0_SSFIFO0_R
#define leituray ADC1_SSFIFO0_R

//função pra salva a ordem e não passa se por algum acaso chega em mais que 199 de posições
void ordemApertado(int a){
    ordem[posicao] = a;
    posicao++;
    if(posicao>199)posicao = 0;
}

//manda o texto pro lcd e pro uart de todos os casos
void cima(){
    escreve_LCD("CIMA");
    UART0_OutChar(' ');
    UART0_OutChar('C');
    UART0_OutChar('I');
    UART0_OutChar('M');
    UART0_OutChar('A');
    UART0_OutChar(' ');
}

void baixo(){
    escreve_LCD("BAIXO");
    UART0_OutChar(' ');
    UART0_OutChar('B');
    UART0_OutChar('A');
    UART0_OutChar('I');
    UART0_OutChar('X');
    UART0_OutChar('O');
    UART0_OutChar(' ');
}

void direita(){
    escreve_LCD("DIREITA");
    UART0_OutChar(' ');
    UART0_OutChar('D');
    UART0_OutChar('I');
    UART0_OutChar('R');
    UART0_OutChar('E');
    UART0_OutChar('I');
    UART0_OutChar('T');
    UART0_OutChar('A');
    UART0_OutChar(' ');
}

void esquerda(){
    escreve_LCD("ESQUERDA");
    UART0_OutChar(' ');
    UART0_OutChar('E');
    UART0_OutChar('S');
    UART0_OutChar('Q');
    UART0_OutChar('U');
    UART0_OutChar('E');
    UART0_OutChar('R');
    UART0_OutChar('D');
    UART0_OutChar('A');
    UART0_OutChar(' ');
}

void botaoA(){
    escreve_LCD("A");
    UART0_OutChar(' ');
    UART0_OutChar('A');
    UART0_OutChar(' ');
}

void botaoB(){
    escreve_LCD("B");
    UART0_OutChar(' ');
    UART0_OutChar('B');
    UART0_OutChar(' ');
}

void botaoC(){
    escreve_LCD("C");
    UART0_OutChar(' ');
    UART0_OutChar('C');
    UART0_OutChar(' ');
}

void botaoD(){
    escreve_LCD("D");
    UART0_OutChar(' ');
    UART0_OutChar('D');
    UART0_OutChar(' ');
}

void botaoE(){
    escreve_LCD("E");
    UART0_OutChar(' ');
    UART0_OutChar('E');
    UART0_OutChar(' ');
}

void botaoF(){
    escreve_LCD("F");
    UART0_OutChar(' ');
    UART0_OutChar('F');
    UART0_OutChar(' ');
}

void start(){
    escreve_LCD("START");
    UART0_OutChar(' ');
    UART0_OutChar('S');
    UART0_OutChar('T');
    UART0_OutChar('A');
    UART0_OutChar('R');
    UART0_OutChar('T');
    UART0_OutChar(' ');
}

//a ordem dos botões é guardada num array que quando ele é checado inteiro ele é zerado
//assim tem como ter a ordem correta e poder usar dentro do delay de 50ms
void trataSysTick(){
    for(i = 0;i <= posicao; i++){
        if(ordem[i] == 1) cima();
        if(ordem[i] == 2) baixo();
        if(ordem[i] == 3) direita();
        if(ordem[i] == 4) esquerda();
        if(ordem[i] == 5) botaoA();
        if(ordem[i] == 6) botaoB();
        if(ordem[i] == 7) botaoC();
        if(ordem[i] == 8) botaoD();
        if(ordem[i] == 9) botaoE();
        if(ordem[i] == 10) botaoF();
    }
    for(i = 0; i <= posicao;i++)ordem[i] = 0;
    posicao = 0;

    //usa variável em vez do registrador trava nos casos de nível baixo e não muda
    //tem que usa o registrador direto, nesse caso apartir de um nome no define
    if(leiturax > 2300) direita();
    if(leiturax < 1800) esquerda();
    if(leituray > 2300) cima();
    if(leituray < 1800) baixo();
}

void trataPortA(){
    //botão start PA2
    if ((GPIO_PORTA_RIS_R & 0x04) == 0x04){
        GPIO_PORTA_ICR_R = 0x04;
        delay(15000);
        start();
    }
    //botão F PA3
    if ((GPIO_PORTA_RIS_R & 0x08) == 0x08){
        GPIO_PORTA_ICR_R = 0x08;
        delay(15000);
        ordemApertado(10);
    }
}

void trataPortC(){
    //botão direita PC4
    if ((GPIO_PORTC_RIS_R & 0x10) == 0x10){
        GPIO_PORTC_ICR_R = 0x10;
        delay(15000);
        ordemApertado(3);
    }
    //botão esquerda PC5
    if ((GPIO_PORTC_RIS_R & 0x20) == 0x20){
        GPIO_PORTC_ICR_R = 0x20;
        delay(15000);
        ordemApertado(4);
    }
    //botão A PC6
    if ((GPIO_PORTC_RIS_R & 0x40) == 0x40){
        GPIO_PORTC_ICR_R = 0x40;
        delay(15000);
        ordemApertado(5);

    }
    //botão B PC7
    if ((GPIO_PORTC_RIS_R & 0x80) == 0x80){
        GPIO_PORTC_ICR_R = 0x80;
        delay(15000);
        ordemApertado(6);
    }
}

void trataPortD(){
    //botão C PD6
    if ((GPIO_PORTD_RIS_R & 0x40) == 0x40){
        GPIO_PORTD_ICR_R = 0x40;
        delay(15000);
        ordemApertado(7);
    }
}

void trataPortE(){
    //botão D PE0
    if ((GPIO_PORTE_RIS_R & 0x01) == 0x01){
        GPIO_PORTE_ICR_R = 0x01;
        delay(15000);
        ordemApertado(8);
    }
}

void trataPortF(){
    //botão cima PF2
    if ((GPIO_PORTF_RIS_R & 0x04) == 0x04){
        GPIO_PORTF_ICR_R = 0x04;
        delay(15000);
        ordemApertado(1);
    }
    //botão baixo PF3
    if ((GPIO_PORTF_RIS_R & 0x08) == 0x08){
        GPIO_PORTF_ICR_R = 0x08;
        delay(15000);
        ordemApertado(2);
    }
    //botão E PF4
    if ((GPIO_PORTF_RIS_R & 0x10) == 0x10){
        GPIO_PORTF_ICR_R = 0x10;
        delay(15000);
        ordemApertado(9);
    }
}

int main(void){

    SYSCTL_RCGCGPIO_R = 0x3D; // ativa os ports A C D E F
    SYSCTL_RCGCADC_R = 0x03; // ativa adc0-1
    GPIO_PORTE_AFSEL_R = 0x06; // PE1-2 com função alternativa
    GPIO_PORTE_AMSEL_R = 0x06; // PE1-2 com função analógica

    //setar ports A C D E F como inputs funcionando como pull down
    GPIO_PORTA_DIR_R = 0x00;
    GPIO_PORTA_PDR_R = 0x0C;
    GPIO_PORTA_IS_R = 0x10;
    GPIO_PORTA_IEV_R = 0x10;
    GPIO_PORTA_IBE_R = 0x00;
    GPIO_PORTA_IM_R = 0x0C;

    GPIO_PORTC_DIR_R = 0x00;
    GPIO_PORTC_PDR_R = 0xF0;
    GPIO_PORTC_IS_R = 0x10;
    GPIO_PORTC_IEV_R = 0x10;
    GPIO_PORTC_IBE_R = 0x00;
    GPIO_PORTC_IM_R = 0xF0;

    GPIO_PORTD_DIR_R = 0x00;
    GPIO_PORTD_PDR_R = 0xC0;
    GPIO_PORTD_IS_R = 0x10;
    GPIO_PORTD_IEV_R = 0x10;
    GPIO_PORTD_IBE_R = 0x00;
    GPIO_PORTD_IM_R = 0xC0;

    GPIO_PORTF_DIR_R = 0x00;
    GPIO_PORTF_PDR_R = 0x1C;
    GPIO_PORTF_IS_R = 0x10;
    GPIO_PORTF_IEV_R = 0x10;
    GPIO_PORTF_IBE_R = 0x00;
    GPIO_PORTF_IM_R = 0x1C;

    GPIO_PORTE_DIR_R = 0x00;
    GPIO_PORTE_PDR_R = 0x01;
    GPIO_PORTE_IS_R = 0x10;
    GPIO_PORTE_IEV_R = 0x10;
    GPIO_PORTE_IBE_R = 0x00;
    GPIO_PORTE_IM_R = 0x01;

    //setar o adc0-1 no port E com o ADC0 sendo o AIN1 e o ADC1 sendo o AIN2
    ADC0_ACTSS_R = 0x00;
    ADC0_EMUX_R = 0xF;
    ADC0_SSMUX0_R = 0x01;
    ADC0_SSCTL0_R = 0x02;
    ADC0_ACTSS_R = 0x01;
    ADC0_PSSI_R = 0x01;
    ADC1_ACTSS_R = 0x00;
    ADC1_EMUX_R = 0xF;
    ADC1_SSMUX0_R = 0x02;
    ADC1_SSCTL0_R = 0x02;
    ADC1_ACTSS_R = 0x01;
    ADC1_PSSI_R = 0x01;

    //ativa os ports dos botões digitalmente
    GPIO_PORTA_DEN_R = 0x0C;
    GPIO_PORTC_DEN_R = 0xF0;
    GPIO_PORTD_DEN_R = 0xC0;
    GPIO_PORTE_DEN_R = 0x01;
    GPIO_PORTF_DEN_R = 0x1C;

    NVIC_EN0_R = 1 << 0 | 1 << 2 | 1 << 3 | 1 << 4 |1 << 30;// liga interupção ports A C D E F

    UART0_Init(); // inicializa o uart
    inicializa_LCD(); // iniciliaza o display

    NVIC_ST_RELOAD_R = tempo; //seta o tempo de 50ms de resposta dos botões
    NVIC_ST_CTRL_R = 0x7; // ativa o systick com interrupt e usando o clock do sistema

    escreve_LCD("Pronto!");//só pra sabe que termino de faze tudo
    while(1){}
}
