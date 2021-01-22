#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>

void UART0_INIT(void)
{
	PINSEL0 |=0x00000005;	
	U0LCR = 0x83;	 
	U0DLM = 0x00;	
	U0DLL = 0x61;	
	U0LCR = 0x03; 
}
void UART0_TxChar(char ch)
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	
}
char UART0_RxChar(void) 
{
	while( (U0LSR & 0x01) == 0);	
	return U0RBR;
}
void MASTER_READ(char data)
{
	char v1;
	IO0CLR = (1<<7); 
	S0SPDR=data;
	while(S0SPSR != 1<<7);
	v1=S0SPDR;
	IO0SET = (1<<7);  
	UART0_TxChar(v1);  
}
void SPI_INIT(void)
{
	PINSEL0 |= 0X00001500;
	IODIR0 = 1<<7;
	IOSET0= 1<<7;
	S0SPCR = 0x0020; 
	S0SPCCR = 0x10; 	
}

int main()
	
{
	UART0_INIT();
	SPI_INIT();
	//while(1)
	{
	MASTER_READ('A');
	}
}
