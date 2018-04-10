//#include<msp430g2553.h>
#include<msp430g2231.h>
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  	volatile int i=-1;
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = CALBC1_1MHZ;//MCLK SMCLK=1MHZ
    TACTL=TASSEL_2+TACLR;//SMCLK,
    CCR0=2000;//PWM周期
    CCTL1=OUTMOD_7;
    CCR1=1000;//384/512=0.75
    P1DIR|=BIT2;
    P1SEL|=BIT2;
    TACTL|=MC_1;//增计数模式
    while(1)
    {
		if(i==2) i=0;
		else i+=1;

		switch(i)
		{
			case 0:
				CCR1 = 880;
				break;
			case 1:
				CCR1 = 1450;
				break;
			case 2:
				CCR1 = 2000;
				break;
		}

		__delay_cycles(10000000);
    }
  return 0;
}
