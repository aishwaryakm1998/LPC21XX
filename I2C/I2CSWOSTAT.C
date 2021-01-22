#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>
char data;
void delay_ms(int count)
{ 
  T0TCR = 0X02;//reset timer
	T0PR = 0x0F;//15MHZ//15 to get 1MHZ
	T0MR0= 0x03E8*count; //1MHZ * 10^6to get 1 sec
	T0MCR= 0x03;
  T0TCR = 0X01;//enable timer
	while(T0IR !=1); //delay
	T0IR |=0x01;
}

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

void I2C_INIT(void)
{ 	
	PINSEL0 = PINSEL0 | 0x00000050; /* Configure P0.2 and P0.3 as SCL0 and SDA0 respectively */
	I2CONSET = 0X44; /* Enable I2C with ack  */	
	I2ADR=0X01;
}
void I2C_READ_ACK( void )
{ 
	while(!(I2CONSET & 0X08));
	I2CONCLR=0X08;
	UART0_TxChar(I2DAT); /* Return received data */
}

int main()
	
{
	
 PINSEL0 =0X0;
 UART0_INIT();	
 I2C_INIT();
 delay_ms(1000);
 I2C_READ_ACK();
	
	while(1)
		
{
	I2C_READ_ACK();
}
}
