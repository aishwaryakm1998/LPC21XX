#include <lpc21xx.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
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
void LCD_CMD(char command)
{
	IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (command<<8) );
	IO0SET = 0x00000040; /* EN = 1 */
	IO0CLR = 0x00000030; /* RS = 0, RW = 0 */
	delay_ms(2);
	IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = RW = 0) */
	delay_ms(5);
}

void LCD_INIT(void)
{
	IO0DIR = 0x0000FFF0; /* P0.8 to P0.15 LCD Data. P0.4,5,6 as RS RW and EN */
	delay_ms(20);
	LCD_CMD(0x38);  /* Initialize lcd */
	LCD_CMD(0x0C);   /* Display on cursor off */
	LCD_CMD(0x06);  /* Auto increment cursor */
	LCD_CMD(0x01);   /* Display clear */
	LCD_CMD(0x80);  /* First line first position */
}
void LCD_CHAR (char msg)
{
		IO0PIN = ( (IO0PIN & 0xFFFF00FF) | (msg<<8) );
		IO0SET = 0x00000050; /* RS = 1, , EN = 1 */
		IO0CLR = 0x00000020; /* RW = 0 */
		delay_ms(2);
		IO0CLR = 0x00000040; /* EN = 0, RS and RW unchanged(i.e. RS = 1, RW = 0) */
		delay_ms(5);
}
void LCD_STRING (char* msg)
{

	while(*msg!=0)
	{
		LCD_CHAR(*msg);
		msg++;
	}
}
void UART0_INIT(void)
{
	PINSEL0 |=0x00000005;	/* Enable UART0 Rx0 and Tx0 pins of UART0 */
	U0LCR = 0x83;	 /* DLAB = 1, 1 stop bit, 8-bit character length */
	U0DLM = 0x00;	/* For baud rate of 9600 with Pclk = 15MHz */
	U0DLL = 0x61;		/* We get these values of U0DLL and U0DLM from formula */
	U0LCR = 0x03; 	/* DLAB = 0 */
}
void UART0_TxChar(char ch) /*A function to send a byte on UART0 */
{
	U0THR = ch;
	while( (U0LSR & 0x40) == 0 );	/* Wait till THRE bit becomes 1 which tells that transmission is completed */
}
void UART0_STRING (char* msg)
{

	while(*msg!=0)
	{
		UART0_TxChar(*msg);
		msg++;
	}
}


void ADC_INIT(void)
{
PINSEL1 |=1<<22;
ADCR=0x00200401; //configure AD0.0 p0.27
}

int main(void)
{

uint32_t result;
float voltage;
char volt[18];

LCD_INIT();
UART0_INIT();
ADC_INIT(); 
	
	

 while(1)
 {
		
   ADCR = ADCR | (1<<24); /* Start Conversion */
   while ( !(ADDR & 0x80000000) );/* Wait till DONE */
 
   result = ADDR;
   result = (result>>6);
   result = (result & 0x000003FF);
   voltage = ( (result/1023.0) * 3.3 ); /* Convert ADC value to equivalent voltage */
   LCD_CMD(0x80);
   sprintf(volt, "Voltage=%.2f V  \r", voltage);
	 LCD_STRING(volt);
	 UART0_STRING(volt);
	 delay_ms(1000);
 }
}
