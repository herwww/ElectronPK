//#include  <msp430g2553.h>
#include  <msp430g2231.h>

void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                  // Stop WDT
        BCSCTL1 = CALBC1_1MHZ; // Set range
        DCOCTL = CALDCO_1MHZ; // Set DCO step + modulation
        P1DIR = BIT2;
        //P1OUT = 0x00;
        P1SEL |= BIT2;
        CCR0=2000;
        CCTL1=OUTMOD_7;
        CCR1=100;
        TACTL=TASSEL_2+MC_1;
        //unsigned int n;
        while(1)
        {
          CCR1=1200;
          __delay_cycles(10000000);
          CCR1=1600;
          __delay_cycles(10000000);
          CCR1=1800;
          __delay_cycles(10000000);
        }
}
