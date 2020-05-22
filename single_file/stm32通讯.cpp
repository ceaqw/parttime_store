#include "stm32f10x.h"
 
//以下的MOSI接口对应你的CSQ 
 
//以下内容需按实际更改
#define SPI_TX_OFF {GPIOA->CRL&=0x0FFFFFFF;GPIOA->CRL|=0x40000000;}//把PA7（MOSI）配置成开漏--输入模式
#define SPI_TX_ON  {GPIOA->CRL&=0x0FFFFFFF;GPIOA->CRL|=0xB0000000;}//把PA7（MOSI）配置成推挽--输出模式（50MHz）
 
#define GPIO_CS_Pin_Name  GPIO_Pin_0
#define GPIO_CS_Pin_Type  GPIOB

#define SPI_CS_ENABLE  GPIO_ResetBits(GPIO_CS_Pin_Type, GPIO_CS_Pin_Name)       //片选脚电平拉低
#define SPI_CS_DISABLE GPIO_SetBits(GPIO_CS_Pin_Type, GPIO_CS_Pin_Name)         //片选脚电平拉高
//#define INDEX_ENABLE   GPIO_SetBits(GPIOA, GPIO_Pin_4)/* for incremental signal index */
 
//#define CLK_H GPIO_SetBits(GPIOA, GPIO_Pin_5)                 //时钟脚PA5电平拉高
//#define CLK_L GPIO_ResetBits(GPIOA, GPIO_Pin_5)               //时钟脚PA5电平拉低
 
//#define DATA_H	GPIO_SetBits(GPIOA, GPIO_Pin_6)                         //PA6（MISO）电平拉高
//#define DATA_L	GPIO_ResetBits(GPIOA, GPIO_Pin_6)                       //PA6（MISO）电平拉低
//#define READ_DATA	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
 
/* 执行SPI命令，执行TLE5012 */
#define READ_STATUS					0x8001			//8000
#define READ_ANGLE_VALUE		0x8021			//8020
#define READ_SPEED_VALUE		0x8031			//8030
 
#define WRITE_MOD1_VALUE		0x5060							//0_1010_0_000110_0001
#define MOD1_VALUE	0x0001
 
#define WRITE_MOD2_VALUE		0x5080							//0_1010_0_001000_0001
#define MOD2_VALUE	0x0801
 
#define WRITE_MOD3_VALUE		0x5091							//0_1010_0_001001_0001
#define MOD3_VALUE	0x0000
 
#define WRITE_MOD4_VALUE		0x50E0							//0_1010_0_001110_0001
#define MOD4_VALUE	0x0098				//9bit 512
 
#define WRITE_IFAB_VALUE		0x50B1
#define IFAB_VALUE 0x000D
/* 功能模式 */
#define REFERESH_ANGLE		0
 
void SPI5012B_Init(void);
void SPI_SendData16(uint16_t SendData);
uint16_t SPI_ReadData16(void);
uint16_t ReadAngle(void);
uint16_t ReadSpeed(void);
uint16_t ReadValue(uint16_t u16Value);
uint16_t SPIx_ReadWriteByte(uint16_t byte);
uint16_t TlE5012W_Reg(uint16_t Reg_CMD, uint16_t Reg_Data);

void SPI5012B_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1,ENABLE );
	
	//以下二句，在它处声明，请增加使用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);//使JTDO、JTDI、JTCK 当成普通IO口进行操作
	//GPIOB0 当成普通IO口进行操作
	
	/*SPI: NSS,SCK,MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //PA5--CLK--复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//GPIO_StructInit(&GPIO_InitStructure);
	
	/* Configure PA6 as encoder input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6--MISO--输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* PA7*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //PA7--MOSI--推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_CS_Pin_Name; //PB0--CS--推挽输出
	GPIO_Init(GPIO_CS_Pin_Type, &GPIO_InitStructure);
	
	/**********SPI****************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //SPI1--双线全双工！！
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}

uint16_t SPIx_ReadWriteByte(uint16_t byte)
{
	uint16_t retry = 0;
	while( (SPI1->SR&1<<1) == 0 )//发送缓冲区非空
	{
		if( ++retry > 200 )
			return 0;//延迟一段时间后返回
	}
	SPI1->DR = byte;     //发送数据
	
	retry = 0;
	while( (SPI1->SR&1<<0) == 0 ) //接收缓冲区为空
	{
		if( ++retry > 200 )
			return 0;//延迟一段时间后返回
	}
	return SPI1->DR;          //读一下缓冲区，清标志
}
 
 
//得到 0~359 度
uint16_t ReadAngle(void)
{
	return ( ReadValue(READ_ANGLE_VALUE) * 360 / 0x10000 );
}
 
//得到角速度
uint16_t ReadSpeed(void)
{
	return ReadValue(READ_SPEED_VALUE);
}
 
 
uint16_t ReadValue(uint16_t u16RegValue)
{
	uint16_t u16Data;
 
	SPI_CS_ENABLE;
	
	SPIx_ReadWriteByte(u16RegValue);
  	SPI_TX_OFF;
	
	u16Data = ( SPIx_ReadWriteByte(0xffff) & 0x7FFF ) << 1;//0x12/0xff*100k
	
	SPI_CS_DISABLE;
  	SPI_TX_ON;
	
	return(u16Data);
}
