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
	I2CONCLR = 0x08;
	I2DAT = data; /* Load data to be written into the data register */
	i2c_interrupt();
}

int main()
{
 int i=0;
 PINSEL0 =0X0;	
 I2C_INIT();
 delay_ms(1000);
 I2C_START();
 I2C_WRITE(0X00);
	
while(i<26)
{		
	I2C_WRITE('A'+i);	
	i++;
}
I2C_STOP();
}
