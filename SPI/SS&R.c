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
//void SLAVE_WRITE(char data)
void SLAVE(char data)
{
   // S0SPDR=data;
	  char v2;
	  S0SPDR=data;
	  while(S0SPSR != 1<<7); 
	  v2 = S0SPDR;
	  UART0_TxChar(v2);
}
/*void UART0_RxChar(void) 
{
	while( (U0LSR & 0x01) == 0);	
	SLAVE_WRITE(U0RBR);
}*/

void SPI_INIT(void)
{
	PINSEL0 |=0X00005500;
}
int main()
	
{
//	char str[]="slave";
  UART0_INIT();
	SPI_INIT();
   while(1)
	   {
	     SLAVE('S');
	   }
}
