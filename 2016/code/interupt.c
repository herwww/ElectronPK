#include "msp430g2553.h"



//名称：delay(int time)
//功能：延时程序
void delay(int time)
{
  static int i,j;
 
 
  for(i=0;i<100;i++)
    for(j=0;j<100;j++);

}


//名称：InitLed()
//功能：初始化LED
void InitLed()
{
  P1DIR |= BIT0+BIT6;//设置BIT0和BIT6为输出
  P1OUT &= ~0x01; //设置BIT0和BIT6低电平
  P1OUT |= 0x40; //设置BIT0和BIT6低电平
}



//名称：InitIOInterrupt()
//功能：初始化P1的按键中断配置
void InitIOInterrupt()
{
  P1DIR &= ~BIT3;//设置P1.3为输入
  P1REN |= BIT3;//启用P1.3电阻
  P1OUT |= BIT3;//设置上拉电阻
  P1IES |= BIT3;//P1.3中断边缘选择
  P1IE |= BIT3;//P1.3中断使能
  P1IFG = 0x00;//清除中断标识
}





//名称：PORT1_ISR(void)
//功能：按键中断的处理函数
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
 
  delay(100);//约100ms
   P1OUT ^= (1<<6);
   P1OUT ^= (1<<0);
   P1IFG = 0;//清除中断标识

}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  InitLed();                     //初始化LED端口
  InitIOInterrupt();             //初始化IO中断
  _EINT();                         //开总中断
  while(1);
  return 0;
}
