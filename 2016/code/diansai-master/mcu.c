#include <msp430g2553.h>

//HD7279各个管脚定义
#define SET_CS P1OUT |= BIT4;
#define CLR_CS P1OUT &= ~BIT4;

#define SET_CLK P1OUT |= BIT5;
#define CLR_CLK P1OUT &= ~BIT5;

#define DATA_IN P1DIR &= ~BIT6; //data脚设为输入
#define DATA_OUT P1DIR |= BIT7; //data脚设为输出
#define SET_DATA P1OUT |= BIT7;
#define CLR_DATA P1OUT &= ~BIT7;
//#define DATA P1IN&BIT2;

//#define KEY P1IN&BIT3;


//HD7279基本命令定义
#define CMD_RESET 0xa4 //复位（清除）指令
#define CMD_TEST 0xbf //测试指令
#define RTL_UNCYL 0xa1 //左移指令
#define RTR_UNCYL 0xa0 //右移指令
#define RTL_CYCLE 0xa3 //循环左移指令
#define RTR_CYCLE 0xa2 //循环右移指令
#define DECODE0 0x80 //方式0译码
#define DECODE1 0xc8 //方式1译码
#define UNDECODE 0x90 //不带小数点显示
#define BLINKCTL 0x88 //闪烁控制
#define ACTCTL 0x98 //消隐控制
#define SEGON 0xe0 //段点亮指令
#define SEGOFF 0xc0 //段关闭指令
#define CMD_READ 0x15 //读取键盘数据指令

//long_delay short_delay 延时57us 延时11us
#define long_delay()  __delay_cycles(70);
#define short_delay() __delay_cycles(30);

#define uchar char
#define uint unsigned int

uint key_code[]={61,36,37,38,44,45,46,52,53,54};  //键盘上的数字键对应的键值，在程序中要进行处理，要转化为0~9
//数组中为数字0~9对应的键值，顺序不能变

uchar key=0;   //按下的键码


//向HD7279中发送指令  相当于写纯指令
void send_byte(uchar com)
{
	uchar i=0;
	CLR_CS;
	long_delay();
	for (i=0;i<8;i++)  //发送命令，高位在前   一个时钟脉冲，送一位数据到7279中
	{
		if (com&0x80)  //高位在前
		{
			SET_DATA;
		}
		else
		{
			CLR_DATA;
		}
		SET_CLK;  //发生脉冲
		short_delay();
		CLR_CLK;
		short_delay();
		com <<= 1;   //左移一位
	}
	CLR_DATA;
	SET_CS;//houjia
}

//向HD7279中发送带数据指令
void send_byte_date(uchar com)
{
	uchar i=0;
	long_delay();
	for (i=0;i<8;i++)  //发送命令，高位在前   一个时钟脉冲，送一位数据到7279中
	{
		if (com&0x80)  //高位在前
		{
			SET_DATA;
		}
		else
		{
			CLR_DATA;
		}
		SET_CLK;  //发生脉冲
		short_delay();
		CLR_CLK;
		short_delay();
		com <<= 1;   //左移一位
	}
	CLR_DATA;
}


void send_32byte_date(unsigned long com)
{
	uchar i=0;
	long_delay();
	for (i=0;i<32;i++)  //发送命令，高位在前   一个时钟脉冲，送一位数据到7279中
	{
		if (com&0x80000000)  //高位在前
		{
			SET_DATA;
		}
		else
		{
			CLR_DATA;
		}
		SET_CLK;  //发生脉冲
		short_delay();
		CLR_CLK;
		short_delay();
		com <<= 1;   //左移一位
	}
	CLR_DATA;
}

//向7279中写入带数据的指令
void wr_data_com(uchar com,uchar data)  //先发送指令，再发送数据
{
	SET_CLK;  //发生脉冲
	short_delay();
	CLR_CLK;
	short_delay();
	CLR_CS;
	send_byte_date (com);    //发送指令
	__delay_cycles(200);
	send_byte_date (data); //发送数据
	SET_CS;
}

void wr_data32_com(uchar com,unsigned long data)  //先发送指令，再发送数据
{
	SET_CLK;  //发生脉冲
	short_delay();
	CLR_CLK;
	short_delay();
	CLR_CS;
	send_byte_date (com);    //发送指令
	__delay_cycles(200);
	send_32byte_date (data); //发送数据
	SET_CS;
}

//从7279中读取数据
uchar receive_byte(void)
{
unsigned char i, in_byte;
	//SET_DATA; // set to input mode
	long_delay();
	for (i=0;i<8;i++)
	{
		SET_CLK;
		short_delay();
		in_byte = in_byte<<1;
		//DATA_IN;  //把DATA脚设为输入 DATA为P12
		if (P1IN&BIT6)
		{
			in_byte=in_byte|0x01;
		}
		CLR_CLK;
		short_delay();
	}
//DATA_OUT;  //把data设为输出
//CLR_DATA;
	return (in_byte);
}


//从7279中读取数据
unsigned long receive_byte32(void)
{
unsigned char i;
unsigned long in_byte;
	//SET_DATA; // set to input mode
	long_delay();
	for (i=0;i<32;i++)
	{
		SET_CLK;
		short_delay();
		in_byte = in_byte<<1;
		//DATA_IN;  //把DATA脚设为输入 DATA为P12
		if (P1IN&BIT6)
		{
			in_byte=in_byte|0x01;
		}
		CLR_CLK;
		short_delay();
	}
//DATA_OUT;  //把data设为输出
//CLR_DATA;
	return (in_byte);
}

unsigned long re_data32_com(uchar com)
{
	unsigned long a;
	SET_CLK;  //发生脉冲
	short_delay();
	CLR_CLK;
	short_delay();
	CLR_CS;
	send_byte_date (com);    //发送指令
	__delay_cycles(200);
	a=receive_byte32();
	SET_CS;
	return a;
}




void hd7279_init()
{
  //delay_ms(50);  //上电延迟
	__delay_cycles(1500);
  P1DIR |= BIT4+BIT5+BIT7;   //单片机相关IO口设置
  P1DIR &= ~BIT6;  //6为输入
  //send_byte(CMD_RESET);  //输入清除指令
  _delay_cycles(1500);
}

void ADC10_init(void)
{
	ADC10CTL0 &= ~ENC;
	ADC10CTL0 = ADC10IE + ADC10ON + REFON + ADC10SHT_3 + SREF_0;
	ADC10CTL1 = CONSEQ_0 + ADC10SSEL_0 + ADC10DIV_3 + SHS_0 + INCH_0;
	__delay_cycles(30000);
	ADC10CTL0 |= ENC;
	_enable_interrupts();
	ADC10CTL0 |= ENC+ ADC10SC;
}


void show(float num)
{
	if(num<10)
	{
		num=num*100;
		wr_data_com(0,(int)num%10);
		wr_data_com(1,(int)num%100/10);
	    wr_data_com(2,(int)num/100|0x10);
	    wr_data_com(3,0);
	}
	else if(num>=10 &&num<100)
	{
		num=num*10;
		wr_data_com(0,(int)num%10);
		wr_data_com(1,(int)num%100/10|0x10);
		wr_data_com(2,(int)num/100);
		wr_data_com(3,0);
	}
	else
	{
		wr_data_com(0,(int)num%10);
		wr_data_com(1,(int)num%100/10);
		wr_data_com(2,(int)num/100);
		wr_data_com(3,0);
	}
}

void show_fre(float num)
{
	if(num<1000)
	{
		wr_data_com(0,(int)num%10);
		wr_data_com(1,(int)num%100/10);
		wr_data_com(2,(int)num/100);
		wr_data_com(3,1);
	}
	else
	{
		num=num/10;
		wr_data_com(0,(int)num%10);
		wr_data_com(1,(int)num%100/10);
		wr_data_com(2,(int)num/100|0x10);
	    wr_data_com(3,2);
	}
}



long float fre;
int temp;
int admax=0;
int admin=1024;
int flag_main;
long a;
float dianya;
long long pingjun;
long long pingjun_fengzhi;
int admax_pingjun;
void main(void)
   {

    unsigned char s1[] ={"key:"};

    WDTCTL = WDTPW+WDTHOLD; // 停止看门狗定时器

    BCSCTL1 = CALBC1_16MHZ;  //设定cpu时钟DCO频率为12MHz
    DCOCTL = CALDCO_16MHZ;


    P1REN |= BIT3;				        //启用P1.3内部上下拉电阻
	P1OUT |= BIT3;			            //将电阻设置为上拉
	//-----配置P1.3中断参数-----
	P1DI                                                                     R &= ~BIT3;                    // P1.3设为输入(可省略)
	P1IES |= BIT3;                          // P1.3设为下降沿中断
	P1IE  |= BIT3 ;                          // 允许P1.3中断

    hd7279_init();

    ADC10_init();
    _enable_interrupts();
    ADC10CTL0 |= ENC + ADC10SC;
    int i=0;

     while(1)
     {
    	 if(flag_main!=2)
    	     __delay_cycles(11000);
    	 else
    		 __delay_cycles(110000);
    		 i=i+1;
    		 pingjun+=temp;
    		 pingjun_fengzhi+=admax;
    		 admax=0;
    		 if(i==100)
    		 {
    			 i=0;
				 dianya=pingjun/100;
				 pingjun=0;
				 admax_pingjun=pingjun_fengzhi/100;
				 pingjun_fengzhi=0;
				 if(flag_main==0) //直流电压
				 {
					 dianya=0.1389*dianya-1.2217;
					// dianya=3.56*dianya/1024*41.33*1.08;
					/* if(dianya<20)
						 dianya=dianya*1.366;
					 else if(dianya<35)
	                        dianya=dianya*1.03*1.047*1.06*1.095;
					 else if(dianya<40)
					 	   dianya=dianya*1.03*1.047*1.06*1.07;
					 else if(dianya<50)
					 	 dianya=dianya*1.03*1.047*1.06;
					 else if(dianya<70)
					 	 dianya=dianya*1.03*1.047;
					 else if(dianya<90)
						 dianya=dianya*1.03;*/
					 show(dianya);
				 }
				 if(flag_main==1) //直流电流
				 {
					 //dianya=3.56*temp/1024;
					 //show(dianya);
					 dianya=dianya*0.0521-0.8495;
					 show(dianya);
					 /*wr_data_com(0,(int)dianya%10);
					 wr_data_com(1,(int)dianya%100/10);
					 wr_data_com(2,(int)dianya%1000/100);
					 wr_data_com(3,(int)dianya/1000);*/
				 }
				 if(flag_main==2) //交流电压
				 {
					 admax_pingjun=0.0992*admax_pingjun+5.6169-0.5;
					/* wr_data_com(0,(int)dianya%10);
					 					 wr_data_com(1,(int)admax_pingjun%100/10);
					 					 wr_data_com(2,(int)admax_pingjun%1000/100);
					 					 wr_data_com(3,(int)admax_pingjun/1000);*/
					 show(admax_pingjun);

				 }
				 if(flag_main==3)//交流频率
				 {
					 a=re_data32_com(0xbf);
					 fre=48000000/a;
					 show_fre(fre);
				 }


				 //__delay_cycles(11000000);
         }
     }


}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	int Push_Key;
	Push_Key=P1IFG&(~P1DIR);
	//-----延时一段时间，避开机械抖动区域-----
	__delay_cycles(10000);							//消抖延时
	//----判断按键状态是否与延时前一致-----
	if((P1IN&Push_Key)==0) 	                	//如果该次按键确实有效
   {
		if(flag_main==0)
		{
			flag_main=1;
		}
		else if(flag_main==1)
		{
			flag_main=2;
		}
		else if(flag_main==2)
		{
			flag_main=3;
		}
		else if(flag_main==3)
		{
			flag_main=0;
		}
   }

   	P1IFG=0;                            			//退出中断前必须手动清除IO口中断标志
}


#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR_HOOK(void)
{

    temp = ADC10MEM;							//读取AD采样值
    if(temp>admax)
        admax=temp;
    if(temp<admin)
    	admin=temp;
    ADC10CTL0 |= ENC+ ADC10SC;
}
