/*
 * MSP430G2253 ADC_v1.c
 *
 *  Created on: 16 Aug 2013
 *      Author: Ant Scranney
 */
#include  "msp430g2253.h"

unsigned int ADC_value=0;

void ConfigureAdc(void);

void main(void)
{
	{
		WDTCTL = WDTPW + WDTHOLD;		// Stop WDT
	//	BCSCTL1 = CALBC1_1MHZ;			// Set range   DCOCTL = CALDCO_1MHZ;

		BCSCTL1 = CALBC1_1MHZ;//MCLK SMCLK=1MHZ
		DCOCTL = CALDCO_1MHZ;
		//BCSCTL2 &= ~(DIVS_3);			// SMCLK = DCO = 1MHz
		P1SEL |= BIT3;// ADC input pin P1.3
		 TACTL=TASSEL_2+TACLR;//SMCLK,
		    CCR0=2000;//PWM周期
		    CCTL1=OUTMOD_7;
		    CCR1=1000;//384/512=0.75
		    P1DIR|=BIT2;
		    P1SEL|=BIT2;
		    TACTL|=MC_1;//增计数模式
		ConfigureAdc();					// ADC set-up function call
		__enable_interrupt();			// Enable interrupts.


		while(1)
		{
			__delay_cycles(1000);				// Wait for ADC Ref to settle
			ADC10CTL0 |= ENC + ADC10SC;			// Sampling and conversion start
			__bis_SR_register(CPUOFF + GIE);	// Low Power Mode 0 with interrupts enabled
			ADC_value = ADC10MEM;				// Assigns the value held in ADC10MEM to the integer called ADC_value
		if(CCR1>0||CCR1<CCR0){
			if(ADC_value>893)CCR1--;
			else if(ADC_value<880)CCR1++;
		}}

	}
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
	__bic_SR_register_on_exit(CPUOFF);        // Return to active mode }
}

// Function containing ADC set-up
void ConfigureAdc(void)
{

	ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/3
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
	ADC10AE0 |= BIT3;                         // ADC input enable P1.3
}
