#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define MAX 50

int Hz_sound[MAX] = {0};
int Hz_time[MAX]={0};
int cnt;   //记录音符个数 

void notice()
{
	puts("********请选择*********");
	puts("");
	puts("********1.输入格式*********");
	puts("********2.播放(无输入则按默认播放)*********");
	puts("********0.退出*********");
}

void init()  //格式输入函数 
{
	int x,y;
	float time;
	int sound[4][7] = {
		{131,147,165,175,196,220,247},
		{262,296,330,349,392,440,449},
		{523,587,659,698,784,880,988},
		{1047,1175,1319,1397,1568,1760,1976}
	};
	puts("请输入(格式：时间间隔 y轴位置 x轴位置 例：0.5 1 1)");
	while(1)
	{
		int k;
		printf("请输入第%d个节奏音符：",cnt+1);
		scanf("%f %d %d",&time,&y,&x);
		Hz_sound[cnt] = sound[y-1][x-1];
		Hz_time[cnt] =time*1000000;
		puts("是否继续输入：1.是  2.否");
		scanf("%d",&k);
		if(k == 2)
		{
			break;				//退出循环 
		} 
		cnt++;
	}
		
}


void sound()
{
	int i;
	if(Hz_sound[0] == 0)
	{
		for (i=0;i<=4000;i+=100)
		{
			Beep(i,100);
			printf("频率:%d\r",i);
			usleep(500000);  //延时0.5秒 
  		}
	}
	else   //输入播放 
	{
		for (i=0;i<=cnt;i++)
		{
			Beep(Hz_sound[i],100);
			usleep(Hz_time[i]);  //延时0.5秒 
  		}
	}
}

void deal()
{
	int k=1,cnt;
	while(k)
	{
		notice();
		printf("请输入对应数字：");
		scanf("%d",&cnt);
		switch(cnt)
		{
			case 0:{
				k = 0;
				break;
			}
			case 1:{
				init();
				break;
			}
			case 2:{
				sound();
				break;
			}
			default :{
				printf("输入有误，请重新输入：");
				break;
			}
		}
		 
	}
}

main()
{
	deal();
	return 0; 
}

