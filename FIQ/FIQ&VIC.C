#include <LPC21xx.h>
#include <stdlib.h>

void delay_ms(unsigned int millisec)
{
	T0PR=15000-1;
	T0PC=T0TC=0x00;
	T0TCR=0x01;
	while(T0TC<millisec);
	T0TCR=0x00;
}

// FOR FIQ INTERRUPT, GO TO STARTUP CODE & THEN COMMENT FIQ HANDLER THERE BY USING SEMICOLON AND THEN IMPORT **FIQ Handler(FIQ ISR)** FROM MAIN CODE.

 extern __irq void FIQ_Handler (void)

{
	  unsigned int TState=T0TC;
	
	     EXTINT |=0X04; //FLAG SET FOR EINT2 I.E P0.7 AS FIQ		
	     IOCLR0 |=0X02;
	     delay_ms(1000);
	     IOSET0 |=0X02;
	
	  T0TC=TState; 
	  T0TCR = 0X01;//enable timer
	
	VICVectAddr =0X00;
}

void FIQ_INIT(void)
{
	
	PINSEL0 |=0x3 <<14; //P0.7 AS FIQ EINT2
	VICIntSelect |=1<<16; //ENABLE EINT2 AS FIQ
	
	EXTMODE &=0X04; //EDGE	
  EXTPOLAR &=0X00; //RISING( AFTER RELEASING SWITCH)
  
  VICIntEnable |= 0x00010000; //ENABLE EINT2
	
}

__irq void EXT1_isr (void)

{
  unsigned int TState=T0TC;
	
  EXTINT |= 0X2; //FLAG SET FOR EINT1 2ND BIT
  IOCLR0 |= 0x04; //P0.2 AS L2
  delay_ms(1000);
  IOSET0 |= 0x04;
	
	  T0TC=TState; 
	  T0TCR = 0X01;//enable timer
	
 VICVectAddr =0x00;//P0.2 AS L2
}

void EXT1_init (void)

{	
	 PINSEL0 |=0x3 <<6; //EINT1 P0.3 (PINSEL 6 & 7 BITS)
	
   EXTMODE &=0X04; //EDGE	
   EXTPOLAR &=0X00; //RISING( AFTER RELEASING SWITCH)

   VICIntEnable |= 0x00008000; //BIT 15 FOR EINT1
   VICVectCntl0 = 0x2F; //5TH BIT1 FOR ENABLING IRQ &(3,2,1,0 BITS 1 FOR EINT1 ENABLE)
   VICVectAddr0 = (unsigned) EXT1_isr; // Pointer for the ISR
}
int main()
	
{
  PINSEL0 |=0X00;	
	IODIR0 |=0X07;
	IOSET0 =0X07;
	
	EXT1_init();
	FIQ_INIT();
	
	  while(1)			
	   { 			
	    IOCLR0 |=0x01;
	    delay_ms(1000);
			IOSET0 |=0x01; 
      delay_ms(1000);		 
	   }
	
}

