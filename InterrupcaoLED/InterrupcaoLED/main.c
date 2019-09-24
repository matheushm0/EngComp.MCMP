#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define setBit(valor,bit) (valor |= (1<<bit)) // coloca o bit com o valor 1
#define clearBit(valor,bit) (valor &= ~(1<<bit)) // coloca o bit com valor 0
#define toogleBit(valor,bit) (valor ^= (1<<bit)) // inverte o estado do bit
#define testBit(valor,bit)    (valor & (1<<bit)) // se o valor do bit for 1, retorna 1,caso contrario, retorna 0


ISR(PCINT2_vect){
	toogleBit(PORTB,7);
}




int main(void)
{
	DDRD=0x00;
	DDRB=0xFF;
		
    while (1) 
    {
		setBit(PCICR,2); // habilita interrupcoes para a porta D
		setBit(PCMSK2,2); // habilita a interrupção do pino 2 da porta D
		sei(); // ativa todas as interrupções globalmente	
    }
}