/*
 * InterfaceSerial.c
 *
 * Created: 9/26/2019 9:55:52 PM
 * Author : Matheus
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define BAUD 28000
#define MYUBRR F_CPU/16/BAUD-1
#define F_CPU 16000000UL 

/* -------------------------------------------- SERIAL ----------------------------------------- */

void USART_Init(unsigned int ubrr){
	/*Set baud rate */
	/* UBRR0H contains the 4 most significant bits of the
	baud rate. UBRR0L contains the 8 least significant
	bits.*/  
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	

	/*Enable transmitter and receiver */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: 8data */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
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

int i;

void lcd_print (char *c){
		while (*c != 0)         
			lcd_dado(*c++);     
}


/* --------------------------------------------------------------------------------------------- */

char tx;

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
	
	while (1) 
    {		
		
		tx = USART_Receive();
		
		if((PIND&0x04) == 0){			
			tx = 'A';
		}

		if((PIND&0x08) == 0){
			tx = 'B';
		}
		
		USART_Transmit(0x0d);
		lcd_comando(0xc0);
		lcd_dado(tx);
    }
}

