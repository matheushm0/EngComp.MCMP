/*
 * InterfaceSerial.c
 *
 * Created: 9/26/2019 9:55:52 PM
 * Author : Matheus
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#define BAUD 28800
#define MYUBRR FOSC/16/BAUD-1
#define F_CPU 1000000UL
#define FOSC 16000000
/* -------------------------------------------- SERIAL ----------------------------------------- */

void USART_Init(unsigned int ubrr){
	
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0A |= (1 << U2X0);
	UCSR0B |= (1 << RXEN0)|(1 << TXEN0);
	UCSR0B |= (1 << RXCIE0);
	UCSR0C = 0x2C; 
}

void USART_Transmit(unsigned char data){

	while ( !( UCSR0A & (1<<UDRE0)) );
	
	UDR0 = data;
}

unsigned char USART_Receive(void){
	
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0){
		USART_Transmit(*StringPtr++);  
	}
}

/* --------------------------------------------- LCD ------------------------------------------- */

void lcd_init(){
	lcd_comando(0x38);
	lcd_comando(0x0E);
	lcd_comando(0x06);
	lcd_comando(0x01);
}

void lcd_comando(char comando){
	PORTB = comando;
	PORTC &= ~(0x01);
	PORTC &= ~(0x02);
	PORTC |= (0x04);
	_delay_ms(50);
	PORTC &= ~(0x04);
}

void lcd_dado(char dado){
	PORTB = dado;
	PORTC |= (0x01);
	PORTC &= ~(0x02);
	PORTC |= (0x04);
	_delay_ms(50);
	PORTC &= ~(0x04);
	
}

void lcd_print (char *c){
		while (*c != 0)         
			lcd_dado(*c++);     
}

/* --------------------------------------------------------------------------------------------- */

volatile char tx;

ISR(USART_RX_vect)
{
	tx = UDR0;
	lcd_dado(tx);
}

ISR(PCINT2_vect){
	if(!(PIND&(1<< 2))){
		lcd_dado('A');
	}
	
	if(!(PIND&(1<< 3))){
		lcd_dado('B');
	}
}


int main(void)
{
	DDRD = 0x00;
	PORTD = 0x0c;
	DDRB = 0xff;
	DDRC = 0x07;
	PORTB = 0xff;

	USART_Init(MYUBRR);
	
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0)|(1 << INT1);
	
	sei();
	
	lcd_init();
	
	while (1) 
    {		
		
    }
}

