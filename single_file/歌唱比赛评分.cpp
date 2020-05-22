#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 

int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char s_number[MIN];  // 参赛号 
	char name[MIN];    //姓名 
	int point[10];		//得分
	float sre_point;		//最终得分 
}msg[MIN];

void meau() {    //进入系统的主界面显示提息
	puts("\t\t\t1.评分添加");
	puts("\t\t\t2.评分浏览");
	puts("\t\t\t3.查找");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void case1()   //信息添加函数 
{
	int i,r,t;
	int flag=0; 
	system("cls");
	for(i=0;i<10;i++)   //遍历输入学生信息，存入相应的便量中 
	{
		flag=0;
		printf("第%d位：\n",i+1);
		printf("请输入选手号:");
		scanf("%s",msg[i_number].s_number);
		for(t=0;t<i_number;t++)
		{
			if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
			{
				printf("该成员已存在!!!");
				flag = 1; 
				break;
			}
		}
		if(!flag)
		{
			printf("请输入姓名:");
			scanf("%s",msg[i_number].name);
			for(i=0;i<10;i++)
			{
				printf("请输入第%d个评委的打分：",i+1);
				scanf("%d",&msg[i_number].point[i]);
			}
			for(r=1; r<10; r++) 
			{
				for(t=0; t<9; t++) 
				{
					if(msg[i_number].point[t+1] > msg[i_number].point[t])   //由高到低 
					{  
						flag=msg[i_number].point[t+1];
						msg[i_number].point[t+1]=msg[i_number].point[t];
						msg[i_number].point[t]=flag;
					}
				}
			}
			msg[i_number].sre_point = 0;
			for(i=1;i<9;i++)
			{
				msg[i_number].sre_point+=msg[i_number].point[i];
			}
			msg[i_number].sre_point /= 8;
			i_number++;
		}
	} 
	puts("录入成功"); 
}

void case2()  //信息浏览函数 
{
	int i,r,t;
	struct msg temp;
	system("cls");
	for(r=1; r<i_number; r++) 
	{
		for(t=0; t<i_number-1; t++) 
		{
			if(msg[t+1].sre_point > msg[t].sre_point)   //由高到低 
			{  
				temp=msg[t+1];
				msg[t+1]=msg[t];
				msg[t]=temp;
			}
		}
	}
	puts("排名\t选手号\t姓名\t最终评分");
	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%d\t%s\t%s\t%.2f\n",i+1,msg[i].s_number,msg[i].name,msg[i].sre_point);
	 } 
}

void case3()	//信息查找
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	while(1)
	{
		printf("查找类型：1.姓名 2.学号 0.退出\n");
		scanf("%d",&cnt);
		if(cnt==1)
		{
			printf("请输入查找的姓名：");
			scanf("%s",name);
			puts("排名\t选手号\t姓名\t最终评分");
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].name) == 0)
				{
					printf("%d\t%s\t%s\t%.2f\n",i+1,msg[i].s_number,msg[i].name,msg[i].sre_point);
					flag =1;
					break;
				}
			}		
		}
		else if(cnt==2)
		{
			printf("请输入查找的选手号：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					printf("%d\t%s\t%s\t%.2f\n",i+1,msg[i].s_number,msg[i].name,msg[i].sre_point);
					flag =1;
					break;
				}
			}
		}
		else if(cnt==0)
		{
			break;
		}
		
	}
	if(!flag)
	{
		puts("未找到该用户信息");	
	}
} 

int main()
{
	int cnt;
	printf("欢迎使用\n");
	while(1)
	{
		meau();
		scanf("%d",&cnt);
		if(cnt==0)
		{
			printf("谢谢使用！！\n");
			break;	
		}	
		else if(cnt==1)
		{
			case1();
		}
		else if(cnt==2)
		{
			case2();
		}
		else if(cnt==3)
		{
			case3();
		}
		else
		{
			printf("输入有误！！\n");
		}
	}	
	return 0;
} 
