#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>

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

void I2C_INIT(void)
{ 	
	PINSEL0 = PINSEL0 | 0x00000050; /* Configure P0.2 and P0.3 as SCL0 and SDA0 respectively */
	I2CONCLR = 0XFF ;
	I2CONSET = 0X40; /* Enable I2C  */	
	I2SCLL = 0x32; /* I2C data rate 300Khz and 50% duty cycle */
	I2SCLH = 0x32; /* I2C data rate 300Khz and 50% duty cycle */
}

void i2c_interrupt ()
{
	while(!(I2CONSET & 0X08)); /* Wait till SI = 1 */
	I2CONCLR = 0x08; /* Clear Start bit and SI bit */	
}

void I2C_START(void)
{
	I2CONCLR = 0XFF ;
	I2CONSET = 0x20; /* Set Start bit for Start condition */
	i2c_interrupt();
}

void I2C_STOP( void )
{
	i2c_interrupt();
	I2CONSET = 0x50; /* Set Stop bit for Stop condition */
}

unsigned char I2C_READ_NACK( void )
{
	I2CONSET = 0x40; /* Enable I2C without Acknowledge */
	i2c_interrupt();
	return I2DAT; /* Return received data */
}

int main()
	
{
	
 unsigned char data;
 I2ADR=0X01;//SLAVE ADDRESS gen call enable
	
 I2C_INIT();
 UART0_INIT();
 I2C_START();
	
	while(1)
		
{
	
	while(!(I2STAT & 0X60)) //Own SLA+W has been received; ACK has been returned.
	{
		printf("adress matched");
		i2c_interrupt();		
	}
	
while(!(I2STAT & 0X60)) //0x88 Previously addressed with own SLA; DATA byte has been received; NOT ACK has been returned.
  {
		data=I2C_READ_NACK();
		UART0_TxChar(data);
		i2c_interrupt();		
	}

 I2C_STOP();
 
}

}
