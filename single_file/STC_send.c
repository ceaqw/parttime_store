#include "reg52.h"                         //此文件中定义了单片机的一些特殊功能寄存器
#include "string.h"
#include "intrins.h"
#define Data_SIZE  //数据长度
  
typedef unsigned  int u16 ;          //对数据类型进行声明定义
typedef unsigned char u8;
unsigned char key_1[Data_SIZE] = {0x5A,0xA5,0x06,0x83,0x00,0x00,0x01,0x00,0x01};
unsigned char key_2[Data_SIZE] = {0x5A,0xA5,0x06,0x83,0x00,0x00,0x01,0x00,0x02};
unsigned char receiveDatas[9];
unsigned char Countor1,Countor2; //设置全局变量，储存定时器 T1 中断次数
unsigned int con1,con2; 
sbit con_1 = P0^1;   //继电器控制接口1
sbit con_2 = P0^2;  //继电器接口2
int i=0;
void Receive(char x);
void result();
void delay500ms();
void delay1s();
sfr WDT_CONTR=0xe1;
/*******************************************************************************
* 函数名         :UsartInit()
* 函数功能                   :设置串口
* 输入           : 无
* 输出                  : 无
*******************************************************************************/
void UsartInit()
{
        SCON=0X50;                        //设置为工作方式1
        TMOD=0X21;                        //设置计数器工作方式2
        PCON=0X80;                        //波特率加倍
        TH1=0XFa;
        TL1=0XFF;
        ES=1;                                                //打开接收中断
        EA=1;                                                //打开总中断
        TR1=1;                                                        //打开计数器
		ET0=1; //定时器 T0 中断允许
		TH0=(65536-46083)/256; //定时器 T0 的高 8 位赋初值
		TL0=(65536-46083)%256; //定时器 T0 的高 8 位赋初值
		TR0=1; //启动定时器 T0  
        WDT_CONTR=0x35;                        
}

/*******************************************************************************
* 函 数 名       : main
* 函数功能                 : 主函数
* 输    入       : 无
* 输    出             : 无
*******************************************************************************/
void main()
{        
        UsartInit();  //        串口初始化
		con_1 = 1;	   //控制器初始化
		con_2 = 1;
		
        while(1)
        {
         delay500ms();
         WDT_CONTR=0x35;
        };                
}

/*******************************************************************************
* 函数名         : Usart() interrupt 4
* 函数功能                  : 串口通信中断函数
* 输入           : 无
* 输出                  : 无
*******************************************************************************/
void Usart() interrupt 4
{
        u8 receiveData;
        receiveData=SBUF;//出去接收到的数据
        Receive(receiveData);

        RI = 0;//清除接收中断标志位
        SBUF=receiveData;//将接收到的数据放入到发送寄存器
        while(!TI);                         //等待发送数据完成
        TI=0;                                                 //清除发送完成标志位
}

/**************************************************
将接收到的字符存入字符串
***************************************************/
void Receive(unsigned char x)
  {
  receiveDatas[i]=x;
  i++;
    if(i==9){
    i=0;
	result();
  
   }
  }

/*******************************************
对比字符串执行命令
*********************************/
void result()
{
    if(receiveDatas[8] == key_1[8])
    {
		con_1 = ~con_1;
        delay500ms();
        WDT_CONTR=0x35;        
    }
    if(receiveDatas[8] == key_2[8])
    {
		con_2 = ~con_2;
        delay500ms();
       	WDT_CONTR=0x35;        
    }
}  


/***********************************************
定时器中断
*****************************************/
void Time0(void) interrupt 1 using 0
{
	if(con_1==0)
	{
		Countor1++; //Countor1 自加 1累计满 2 次，即计时满 100ms
		if(Countor1 == 20)		//满一秒 
		{
			con1++; 
			Countor1 = 0;
		} 
	}
	if(con_2==0)
	{
		Countor2++;
		if(Countor2 == 20)		//满一秒 
		{
			con2++; 
			Countor2 = 0;
		} 
	}
	if(con1 == 1800) //若满30分钟 1800
	{
		if(con_1==0)
		{
			con_1 = 1;	//关闭
			WDT_CONTR=0x35;
		}
		con1=0; //将 con 清 0，重新从 0 开始计数
	}
	if(con2 == 1800) //若满30分钟 1800
	{
		if(con_2==0)
		{
			con_2 = 1;	//关闭
			WDT_CONTR=0x35;
		}
		con2 = 0; //将 con 清 0，重新从 0 开始计数
	}
	TH0=(65536-46083)/256; //定时器 T0 的高 8 位重新赋初值
	TL0=(65536-46083)%256; //定时器 T0 的高 8 位重新赋初值
}


/*********************************************
延时0.5秒  和延时1秒 9秒
*******************************************/
void delay1s()   //误差 -0.000000000227us
{
    unsigned char a,b,c;
    for(c=13;c>0;c--)
        for(b=247;b>0;b--)
            for(a=142;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}


void delay500ms()   //误差 -0.000000000114us
{
    unsigned char a,b,c;
    for(c=98;c>0;c--)
        for(b=127;b>0;b--)
            for(a=17;a>0;a--);
    _nop_();  //if Keil,require use intrins.h
}

