#include  <msp430g2553.h>
#include "display.h"
//#include  <msp430g2231.h>

/*
void display(int* data){
	//显示带小数点的四位数
	//0:10^(-x)
	//1:个位
	//2:十位
	//3:百位
	//4:千位
	//enable
	int i=0;
	P1OUT|=BIT0;
	for(i=0;i<5;i++){
		P1OUT&=0xf8;
		//P1OUT&=~BIT0;
		P1OUT|=(data[i]&0x01)<<0;
		P1OUT|=((data[i]>>1)&0x01)<<1;
		P1OUT|=((data[i]>>2)&0x01)<<2;
		//P1OUT|=BIT0;
		__delay_cycles(1000000);
	}
}
*/
void init_display_number(){
	P1DIR|=BIT3+BIT4+BIT5;
	P1OUT&=0xc7;//set 0;
	P1DIR|=BIT0;
}
void display_number(int number){
	P1OUT&=0xc7;
	P1OUT|=(number&0x01)<<3;
	P1OUT|=((number>>1)&0x01)<<4;
	P1OUT|=((number>>2)&0x01)<<5;

}
