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

#define BAUD 4800UL
#define MYUBRR ((unsigned long)((F_CPU/(16*BAUD))-1))
#define F_CPU 1000000UL
/* -------------------------------------------- SERIAL ----------------------------------------- */

void USART_Init(unsigned int ubrr){
	
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
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

char tx;

ISR(PCINT2_vect){
	if(!(PIND&(1<< 2))){
		tx = 'A';
		lcd_dado(tx);
		USART_Transmit(0x0d);
		USART_Transmit(tx);
		lcd_comando(0xc0);
	}
	
	if(!(PIND&(1<< 3))){
		tx = 'B';
		lcd_dado(tx);
		USART_Transmit(0x0d);
		USART_Transmit(tx);
		lcd_comando(0xc0);
	}
}

void ativa_interrupcao(){
	PCICR = 0x04;
	PCMSK2 = 0x0c;
	sei();
}

int main(void)
{
	DDRB = 0xff;
	DDRC = 0x07;
	lcd_init();
	USART_Init(MYUBRR);
    lcd_comando(0x80);
    lcd_print("Interface Serial");
	USART_putstring("USART para LCD");
	USART_Transmit(0x0d);
	ativa_interrupcao();
	
	while (1) 
    {		
		
		tx = USART_Receive();		
		lcd_comando(0xc0);
		lcd_dado(tx);
		USART_Transmit(0x0d);
		USART_Transmit(tx);
    }
}

