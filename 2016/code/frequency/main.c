#include "msp430g2553.h"

unsigned int Overflow_Cnt=0;
unsigned long Frq=0;

void main()
	{
    	WDTCTL = WDTPW+WDTHOLD;	// Stop watchdog timer
    	//P2SEL |=BIT1;				//选择P2.1第二功能
    	P1DIR |=BIT6;
    	P1DIR&=~BIT0;
    	P1SEL|=BIT0;
    	P1SEL2&=~BIT0;
    	TA0CTL =TASSEL_0+MC_1;		//选择外部时钟，向上计数模式至CCR0
    	TA0CCR0=499;
    	TA0CCTL0 =CCIE;				//使能TACCR0中断
    	WDTCTL=WDT_ADLY_1000;		//WDT定时1S
    	IE1 |=WDTIE;					//开WDT定时中断
    	_EINT();
    	LPM3;
    	//_BIS_SR(LPM3_bits+GIE);

	}
#pragma vector=TIMER0_A0_VECTOR  		//TIMER0_A0_VECTOR 中TIMER0不可少0！！！！（一下午的领悟）
__interrupt void Timer_A(void)
{
	Overflow_Cnt++; 				//溢出次数计数
	P1OUT ^=BIT6;

}
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	Frq=TAR+500*Overflow_Cnt;		//计算频率
	TACTL |=TACLR;					//TAR清零
	Overflow_Cnt=0;					//溢出次数
}
