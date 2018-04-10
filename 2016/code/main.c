#include  <msp430g2553.h>
#include "display.h"
void main(void)
{
	WDTCTL = WDTPW + WDTHOLD;                  // Stop WDT
        BCSCTL1 = CALBC1_1MHZ; // Set range
        DCOCTL = CALDCO_1MHZ; // Set DCO step + modulation
		init_display_number();
		display_number(4);
};

