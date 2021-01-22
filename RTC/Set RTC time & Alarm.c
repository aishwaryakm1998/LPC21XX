#include <lpc21xx.h>
#include <stdio.h>
#include <stdlib.h>
int flag,alarm;
typedef unsigned int u32;
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
__irq void RTC_ISR(void)
{
	if (ILR & 0x01)
	{
	  flag=1;
		ILR = ILR | 0x01;
	}	
	if (ILR & 0x02)
	{
		alarm = 1;		
		ILR = ILR | 0x02;	
	}

 VICVectAddr = 0x0;	
}

void INT_INIT (void)

{
	
VICIntSelect =0x00000000;
VICIntEnable = 0x00002000;
VICVectCntl0 = 0x0000002D;
VICVectAddr0 = (unsigned) RTC_ISR; // Pointer for the ISR
}

void TIME_INIT(void)
{
	SEC =00; 
	MIN =01;
	HOUR =11;
	DOM = 9;
	DOW = 4;
	DOY = 343;
	MONTH =12;
	YEAR = 2020;
}

void ALARM_INIT(void)
{
	ALSEC =10;
	ALMIN = 01;
	ALHOUR =11;
	ALDOM = 9;
	ALDOW = 4;
	ALDOY = 343;
	ALMON = 12;
	ALYEAR = 2020;
	
}

void RTC_INIT(void)
{
	INT_INIT();
	
	ILR=0x00;
	CCR=0x02;
	CCR =0x00;
	CIIR=0X00;
	AMR=0x00;
	
PREINT = 0x01C8; /* For 15MHz Fpclk, PREINT value */
PREFRAC = 0x61C0; /* For 15MHz Fpclk, PREFRAC value */
	
	TIME_INIT();
	
  CIIR = 0x01; /* Seconds value increment interrupt */
	ALARM_INIT();
	
	CCR = 0x01; /* Clock Enable */
	ILR = 0x03; /* RTC interrupts enabled */

}
int main()
{
	
	char timestr[30], datestr[30];

  flag=0;
	alarm=0;
	
	  PINSEL2 |=0X00000000;// p1.20 as led
	  IODIR1 |=1<<20; //
	  IOSET1 |=1<<20;
	  LCD_INIT();
	  RTC_INIT();	
	  

	while(1)
{
		
	if (flag == 1)
		{
			
			sprintf(timestr,"Time: %d:%d:%d ",HOUR,MIN,SEC);
			sprintf(datestr,"Date: %d/%d/%d ",DOM,MONTH,YEAR);
			LCD_CMD(0x80);
			LCD_STRING(timestr);
			LCD_CMD(0xC0);
			LCD_STRING(datestr);
			flag = 0;
			
		}
		if(alarm == 1)
	{		
		IO1CLR |= 1<<20;;		
	  if(SEC==16)
		{
	  IO1SET |= 1<<20;			
    alarm = 0;			
		}
	}
}
	
}
