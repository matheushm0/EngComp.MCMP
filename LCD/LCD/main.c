/*
 * LCD.c
 *
 * Created: 9/10/2019 8:27:41 PM
 * Author : Matheus
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#define F_CPU 16000000UL	// 16 MHz
#define LCD_Port PORTD
#define LCD_DPin DDRD		//Define 4-Bit (PD4-PD7 na PORT D)
#define RS PD0				//Pino RS
#define EN PD1				//Pino E

void lcd_init(void) {
	
	LCD_DPin = 0xFF;	//Controla Pinos (D4-D7)
	_delay_ms(15);		//Espera a ativação do LCD
	lcd_action(0x02);	//Controle de 4-Bit
	lcd_action(0x28);	//Controla Matriz
	lcd_action(0x0c);	//Desativa o Cursor
	lcd_action(0x06);	//Move o Cursor
	lcd_action(0x01);	//Limpa o LCD
	_delay_ms(2);
			
}

void lcd_action(unsigned char comando) {
	
	LCD_Port = (LCD_Port & 0x0F) | (comando & 0xF0);
	LCD_Port &= ~ (1<<RS);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (comando << 4);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);

}

void lcd_clear(){
	
	lcd_action(0x01);	//Limpa o LCD
	_delay_ms(2);		//Espera até limpar o LCD
	lcd_action(0x80);	//Move o cursor pra posição 1
	
}

void lcd_print (char *str){
	
	int i;
	for(i=0; str[i]!=0; i++)
	{
		LCD_Port = (LCD_Port & 0x0F) | (str[i] & 0xF0);
		LCD_Port |= (1<<RS);
		LCD_Port|= (1<<EN);
		_delay_us(1);
		LCD_Port &= ~ (1<<EN);
		_delay_us(200);
		LCD_Port = (LCD_Port & 0x0F) | (str[i] << 4);
		LCD_Port |= (1<<EN);
		_delay_us(1);
		LCD_Port &= ~ (1<<EN);
		_delay_ms(2);
	}
	
}

int main(void)
{
	
	lcd_init();
	lcd_print("Teste LCD");
    while (1) 
    {
		lcd_action(0xC0);
		lcd_print(__TIME__);
		_delay_ms(1000);
    }
}

