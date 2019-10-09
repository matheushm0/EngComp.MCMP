/*
 * LimitadorDeVelocidade.c
 *
 * Created: 10/3/2019 8:45:50 PM
 * Author : Matheus
 */ 

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL
#define BAUD 4800UL
#define MYUBRR ((unsigned long)((F_CPU/(16*BAUD))-1))

#define toogleBit(valor,bit) (valor ^= (1<<bit))
#define setBit(valor,bit) (valor |= (1<<bit))

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

void lcd_clear(){
	lcd_comando(0x01);	
	_delay_ms(2);		
	lcd_comando(0x80);	
}
/* ------------------------------------------ INTERRUPT ---------------------------------------- */

float count;

ISR(PCINT2_vect){

	if((PIND&(1 << 2))){
		toogleBit(PORTC,3);
		count++;
	}
}

void interrupt_init(){
	
	PCICR = 0x04;
	PCMSK2 = 0x0c;
	sei();
	
}

/* --------------------------------------------- SPEED ------------------------------------------ */

// reverses a string 'str' of length 'len' 
void reverse(char *str, int len) 
{ 
    int i=0, j=len-1, temp; 
    while (i<j) 
    { 
        temp = str[i]; 
        str[i] = str[j]; 
        str[j] = temp; 
        i++; j--; 
    } 
} 

 // Converts a given integer x to string str[].  d is the number 
 // of digits required in output. If d is more than the number 
 // of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) 
{ 
    int i = 0; 
    while (x) 
    { 
        str[i++] = (x%10) + '0'; 
        x = x/10; 
    } 
  
    // If number of digits required is more, then 
    // add 0s at the beginning 
    while (i < d) 
        str[i++] = '0'; 
  
    reverse(str, i); 
    str[i] = '\0'; 
    return i; 
} 

// Converts a floating point number to string. 
void ftoa(float n, char *res, int afterpoint) 
{ 
    // Extract integer part 
    int ipart = (int)n; 
  
    // Extract floating part 
    float fpart = n - (float)ipart; 
  
    // convert integer part to string 
    int i = intToStr(ipart, res, 0); 
  
    // check for display option after point 
    if (afterpoint != 0) 
    { 
        res[i] = '.';  // add dot 
  
        // Get the value of fraction part upto given no. 
        // of points after dot. The third parameter is needed 
        // to handle cases like 233.007 
        fpart = fpart * pow(10, afterpoint); 
  
        intToStr((int)fpart, res + i + 1, afterpoint); 
    } 
} 

char vmax;
char x[5];

void speed(float tempo, float vmax){
	
	float distancia = 0.06;
	float velocidade = 0.0;
	float vatual = 0.0;
	
	velocidade = (tempo/distancia) * 3.6;
	
	vatual += velocidade; 
	
	ftoa(vatual, x, 5);
	for(int i = 0; i < 5; i++){
		lcd_dado(x[i]);
	}
	
}

/* --------------------------------------------------------------------------------------------- */

void welcome(){
	
	lcd_comando(0x80);
	lcd_print("Bem-Vindo!");
	lcd_comando(0xc0);
	lcd_print("VMax =     Km/h");
	lcd_comando(0xc7);
	USART_putstring("Digite a Velocidade Maxima: ");
	USART_Transmit(0x0d);
	
}

char tx;

int main(void)
{
	DDRB = 0xff;
	DDRC = 0xff;
	
	interrupt_init();
	lcd_init();
	USART_Init(MYUBRR);
	
	welcome();
	
	while (1) 
    {		
		
		tx = USART_Receive();
		USART_Transmit(tx);
		lcd_dado(tx);
		vmax = tx;
		
		if(tx == 0x0D){		
			lcd_clear();
			lcd_print("V Atual:");
			lcd_print(" 0 Km/h");
		}
    }
}

