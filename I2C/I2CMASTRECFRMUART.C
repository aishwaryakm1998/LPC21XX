#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>

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
void I2C_INIT(void)
{ 	
	PINSEL0 = PINSEL0 | 0x00000050; /* Configure P0.2 and P0.3 as SCL0 and SDA0 respectively */
	I2CONSET = 0X40; /* Enable I2C  */	
	I2SCLL = 0x32; /* I2C data rate 300Khz and 50% duty cycle */
	I2SCLH = 0x32; /* I2C data rate 300Khz and 50% duty cycle */
}

void i2c_interrupt ()
{
	while(!(I2CONSET & 0X08)); /* Wait till SI = 1 */
	I2CONCLR = 0x08; /* Clear SI bit */	
}

void I2C_START(void)
{
	I2CONSET = 0x20; /* Set Start bit for Start condition */
	i2c_interrupt();
	I2CONCLR = 0x20;
}

void I2C_STOP( void )
{
	i2c_interrupt();
	I2CONSET = 0x50; /* Set Stop bit for Stop condition */
}

void I2C_WRITE( char data )
{ 
	//I2CONCLR = 0x08;
	I2DAT = data; /* Load data to be written into the data register */
	i2c_interrupt();
}

void UART0_init(void)
{
	PINSEL0 |=0x00000005;	/* Enable UART0 Rx0 and Tx0 pins of UART0 */
	U0LCR = 0x83;	 /* DLAB = 1, 1 stop bit, 8-bit character length */
	U0DLM = 0x00;	/* For baud rate of 9600 with Pclk = 15MHz */
	U0DLL = 0x61;		/* We get these values of U0DLL and U0DLM from formula */
	U0LCR = 0x03; 	/* DLAB = 0 */
}
void UART0_RxChar(void) /*A function to receive a byte on UART0 */
{
	while( (U0LSR & 0x01) == 0);	/*Wait till RDR bit becomes 1 which tells that receiver contains valid data */
	I2C_WRITE(U0RBR);
}

int main()
{
 //char ch;
 PINSEL0 =0X0;
 UART0_init();	
 I2C_INIT();
 delay_ms(1000);
 I2C_START();
 I2C_WRITE(0X84);
	
while(1)
{	
	UART0_RxChar();
  //delay_ms(20);
}
//I2C_STOP();
}
