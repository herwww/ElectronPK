#include "msp430g2553.h"



//���ƣ�delay(int time)
//���ܣ���ʱ����
void delay(int time)
{
  static int i,j;
 
 
  for(i=0;i<100;i++)
    for(j=0;j<100;j++);

}


//���ƣ�InitLed()
//���ܣ���ʼ��LED
void InitLed()
{
  P1DIR |= BIT0+BIT6;//����BIT0��BIT6Ϊ���
  P1OUT &= ~0x01; //����BIT0��BIT6�͵�ƽ
  P1OUT |= 0x40; //����BIT0��BIT6�͵�ƽ
}



//���ƣ�InitIOInterrupt()
//���ܣ���ʼ��P1�İ����ж�����
void InitIOInterrupt()
{
  P1DIR &= ~BIT3;//����P1.3Ϊ����
  P1REN |= BIT3;//����P1.3����
  P1OUT |= BIT3;//������������
  P1IES |= BIT3;//P1.3�жϱ�Եѡ��
  P1IE |= BIT3;//P1.3�ж�ʹ��
  P1IFG = 0x00;//����жϱ�ʶ
}





//���ƣ�PORT1_ISR(void)
//���ܣ������жϵĴ�����
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
 
  delay(100);//Լ100ms
   P1OUT ^= (1<<6);
   P1OUT ^= (1<<0);
   P1IFG = 0;//����жϱ�ʶ

}

int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;
  InitLed();                     //��ʼ��LED�˿�
  InitIOInterrupt();             //��ʼ��IO�ж�
  _EINT();                         //�����ж�
  while(1);
  return 0;
}
