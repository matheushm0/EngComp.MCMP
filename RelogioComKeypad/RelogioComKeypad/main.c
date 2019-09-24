#define F_CPU 16000000UL
#define setBit(valor,bit) (valor |= (1<<bit)) // coloca o bit em 1
#define clearBit(valor,bit) (valor &= ~(1<<bit)) // coloca o bit em 0
#define testBit(valor,bit) (valor & (1<<bit))

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

int count=0;
char tempo[6]; // armazena os digitos da hora digitada no lcd



void Envia_Dado_Lcd (unsigned char dado)
{
	clearBit(PORTC,5);
	setBit(PORTC,3);
	clearBit(PORTC,4);
	PORTB=dado;
	setBit(PORTC,5);
	clearBit(PORTC,5);
	_delay_ms(10);
}




void Envia_Texto (unsigned char dado[]){
	int i;
	for(i=0;i<strlen(dado);i++){
		Envia_Dado_Lcd(dado[i]);
	}
}


void Envia_Comando_Lcd (unsigned char dado)
{
	clearBit(PORTC,5); // LCD desabilitado
	clearBit(PORTC,3); //seleciona envio de instrução
	clearBit(PORTC,4); //seleciona modo de escrita no lcd
	PORTB=dado;
	setBit(PORTC,5);
	clearBit(PORTC,5); // habilita o LCD a receber a informação
	_delay_ms(10);
}

void Inicializa_Lcd ()
{
	Envia_Comando_Lcd (0x38);
	Envia_Comando_Lcd (0x38);
	Envia_Comando_Lcd (0x06);
	Envia_Comando_Lcd (0x0E);
	Envia_Comando_Lcd (0x01); // limpa LCD
}


void acionaRelogio(){
	Envia_Comando_Lcd(0xcb);
	Envia_Dado_Lcd(tempo[5]);
	tempo[5]++;
	if(tempo[5]==':')
	tempo[5]='9';
	
	
	if(tempo[0]=='2' && tempo[1] =='3' && tempo[2]=='5' && tempo[3]== '9' && tempo[4]=='5' && tempo[5]=='9' ){
		Envia_Comando_Lcd(0xcb);
		Envia_Dado_Lcd(tempo[5]);
		tempo[5]='0';
		tempo[4]='0';
		tempo[3]='0';
		tempo[2]='0';
		tempo[1]='0';
		tempo[0]='0';
		Envia_Comando_Lcd(0xca);
		Envia_Dado_Lcd(tempo[4]);
		Envia_Comando_Lcd(0xc8);
		Envia_Dado_Lcd(tempo[3]);
		Envia_Comando_Lcd(0xc7);
		Envia_Dado_Lcd(tempo[2]);
		Envia_Comando_Lcd(0xc5);
		Envia_Dado_Lcd(tempo[1]);
		Envia_Comando_Lcd(0xc4);
		Envia_Dado_Lcd(tempo[0]);
		_delay_ms(20);
	}
	
	if(tempo[1] =='9' && tempo[2]=='5' && tempo[3]== '9' && tempo[4]=='5' && tempo[5]=='9' ){
		Envia_Comando_Lcd(0xcb);
		Envia_Dado_Lcd(tempo[5]);
		tempo[5]='0';
		tempo[4]='0';
		tempo[3]='0';
		tempo[2]='0';
		tempo[1]='0';
		Envia_Comando_Lcd(0xca);
		Envia_Dado_Lcd(tempo[4]);
		tempo[0]++;
		Envia_Comando_Lcd(0xc8);
		Envia_Dado_Lcd(tempo[3]);
		Envia_Comando_Lcd(0xc7);
		Envia_Dado_Lcd(tempo[2]);
		Envia_Comando_Lcd(0xc5);
		Envia_Dado_Lcd(tempo[1]);
		Envia_Comando_Lcd(0xc4);
		Envia_Dado_Lcd(tempo[0]);
		_delay_ms(20);
	}
	
	if(tempo[2]=='5' && tempo[3]== '9' && tempo[4]=='5' && tempo[5]=='9' ){
		Envia_Comando_Lcd(0xcb);
		Envia_Dado_Lcd(tempo[5]);
		tempo[5]='0';
		tempo[4]='0';
		tempo[3]='0';
		tempo[2]='0';
		Envia_Comando_Lcd(0xca);
		Envia_Dado_Lcd(tempo[4]);
		tempo[1]++;
		Envia_Comando_Lcd(0xc8);
		Envia_Dado_Lcd(tempo[3]);
		Envia_Comando_Lcd(0xc7);
		Envia_Dado_Lcd(tempo[2]);
		Envia_Comando_Lcd(0xc5);
		Envia_Dado_Lcd(tempo[1]);
		_delay_ms(20);
	}
	
	if(tempo[3]== '9' && tempo[4]=='5' && tempo[5]=='9' ){
		Envia_Comando_Lcd(0xcb);
		Envia_Dado_Lcd(tempo[5]);
		tempo[5]='0';
		tempo[4]='0';
		tempo[3]='0';
		Envia_Comando_Lcd(0xca);
		Envia_Dado_Lcd(tempo[4]);
		tempo[2]++;
		Envia_Comando_Lcd(0xc8);
		Envia_Dado_Lcd(tempo[3]);
		Envia_Comando_Lcd(0xc7);
		Envia_Dado_Lcd(tempo[2]);
		_delay_ms(20);
		
	}
	
	if(tempo[4]=='5' && tempo[5]=='9'){
		Envia_Comando_Lcd(0xcb);
		Envia_Dado_Lcd(tempo[5]);
		tempo[5]='0';
		tempo[4]='0';
		Envia_Comando_Lcd(0xca);
		Envia_Dado_Lcd(tempo[4]);
		tempo[3]++;
		Envia_Comando_Lcd(0xc8);
		Envia_Dado_Lcd(tempo[3]);
		_delay_ms(20);
	}
	
	
	if(tempo[5]=='9'){
		Envia_Comando_Lcd(0xcb);
		Envia_Dado_Lcd(tempo[5]);
		tempo[5]='0';
		tempo[4]++;
		Envia_Comando_Lcd(0xca);
		Envia_Dado_Lcd(tempo[4]);
		
	}
}




int main(void)
{
	DDRB=0xFF;
	DDRD=0x0F;
	DDRC=0b1111111;
	Inicializa_Lcd ();
	Envia_Comando_Lcd (0x82); // usa a coluna 3 da linha 1
	Envia_Texto("Teste do Lcd");
	Envia_Comando_Lcd(0xc4);// usa a coluna 5 da linha 2
	while (1)
	{
		setBit(PORTD,3);
		if(testBit(PIND,7)){
			Envia_Comando_Lcd(0xc4);
			Envia_Texto("00:00:00");
			Envia_Comando_Lcd(0xc4);
			_delay_ms(5);
		}

		if(testBit(PIND,5)){
			_delay_ms(5);
			while(1){
				acionaRelogio();
				if(testBit(PIND,5)){
					_delay_ms(5);
					break;
				}
			}
		}

		if(testBit(PIND,4)){
			Envia_Dado_Lcd(':');
			_delay_ms(5);
		}
		
		clearBit(PORTD,3);
		
		
		setBit(PORTD,2);
		if(testBit(PIND,7)){
			Envia_Dado_Lcd('1');
			tempo[count]='1';
			count++;
			_delay_ms(5);
		}
		
		clearBit(PORTD,2);
		
		setBit(PORTD,1);
		if(testBit(PIND,7)){
			Envia_Dado_Lcd('4');
			tempo[count]='4';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,1);
		
		setBit(PORTD,0);
		if(testBit(PIND,7)){
			Envia_Dado_Lcd('7');
			tempo[count]='7';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,0);
		
		
		setBit(PORTD,3);
		if(testBit(PIND,6)){
			Envia_Dado_Lcd('0');
			tempo[count]='0';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,3);
		
		setBit(PORTD,2);
		if(testBit(PIND,6)){
			Envia_Dado_Lcd('2');
			tempo[count]='2';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,2);
		
		setBit(PORTD,1);
		if(testBit(PIND,6)){
			Envia_Dado_Lcd('5');
			tempo[count]='5';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,1);
		
		setBit(PORTD,0);
		if(testBit(PIND,6)){
			Envia_Dado_Lcd('8');
			tempo[count]='8';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,0);
		
		setBit(PORTD,2);
		if(testBit(PIND,5)){
			Envia_Dado_Lcd('3');
			tempo[count]='3';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,2);
		
		setBit(PORTD,1);
		if(testBit(PIND,5)){
			Envia_Dado_Lcd('6');
			tempo[count]='6';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,1);
		
		setBit(PORTD,0);
		if(testBit(PIND,5)){
			Envia_Dado_Lcd('9');
			tempo[count]='9';
			count++;
			_delay_ms(5);
		}
		clearBit(PORTD,0);
	}
}