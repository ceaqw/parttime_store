#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char s_number[MIN];  // 卡号 
	char name[MIN];    //姓名 
	int mon;		//月份
	float base_money;		//基本工资
	float j_fee;			//津贴 
	float add_money;		//补贴
	float y_money;		//应发工资
	float w_fee; 		//水费
	float e_fee;		//电费
	float s_fee;		//税金
	float t_fee;		//实发工资
}msg[MAX];

void deal();  //调用功能函数 

void init()   //初始导入txt文本文件函数 
{
	FILE*fp;  //定义文本字符指针
	int flag=0;
	if(!(fp=fopen("test.txt","r"))) 
	{
		fp=fopen("test.txt","w+");
		flag = 1;
	}
	if(!flag)
	{
		while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中 
		{
			fscanf(fp,"%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",msg[i_number].s_number,msg[i_number].name,&msg[i_number].mon,
			&msg[i_number].base_money,&msg[i_number].j_fee,&msg[i_number].add_money,&msg[i_number].y_money,
			&msg[i_number].w_fee,&msg[i_number].t_fee,&msg[i_number].s_fee,&msg[i_number].t_fee);
			i_number++;
		}
	}
	fclose(fp);   //关闭文本 
	puts("读取成功"); 
 } 
 
 void save(){   //保存文本函数 
	FILE*fp;
	int i;
	if(!(fp=fopen("test.txt","w"))) // 打开新建的文件遍历写入内存中的自信息 
	{
		fp=fopen("test.txt","w+");
	}  
	for(i=0; i<i_number; i++){
	    fprintf(fp,"%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",msg[i].s_number,msg[i].name,msg[i].mon,
		msg[i].base_money,msg[i].j_fee,msg[i].add_money,msg[i].y_money,
		msg[i].w_fee,msg[i].t_fee,msg[i].s_fee,msg[i].t_fee);			
	}
	fclose(fp);
	puts("保存成功"); 
} 

void notice() {    //进入系统的主界面显示提息
	puts("\t\t\t1.工资信息的输入");
	puts("\t\t\t2.添加工资信息");
	puts("\t\t\t3.浏览职工工资信息");
	puts("\t\t\t4.排序");
	puts("\t\t\t5.查询功能");
	puts("\t\t\t6.统计功能");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void case1()   //信息录入函数 
{
	int i,n,t,flag=0;
	system("cls");
	puts("***************>>>>>>当前模式为信息录入<<<<<<*************");
	printf("请输入录入账户数量：");
	scanf("%d",&n);
	for(i=0;i<n;i++)   //遍历输入信息，存入相应的便量中 
	{
		flag=0;
		printf("第%d位：\n",i+1);
		printf("请输入卡号:");
		scanf("%s",msg[i_number].s_number);
		for(t=0;t<i_number;t++)
		{
			if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
			{
				printf("该成员%d月份工资信息已存在，可输入其其他月份工资信息!!!\n",msg[t].mon);
				flag=msg[t].mon;
				break;
			}
		}
		if(flag)
		{
			strcpy(msg[i_number].name,msg[t].name);
		}
		else
		{
			printf("请输入姓名:");
			scanf("%s",msg[i_number].name);
		}
		printf("请输入月份:");
		scanf("%d",&msg[i_number].mon);
		if(msg[i_number].mon==flag)
		{
			puts("已存在！！");
			i--;
			break;
		}
		printf("请输入基本工资:");
		scanf("%f",&msg[i_number].base_money);
		printf("请输入津贴:");
		scanf("%f",&msg[i_number].j_fee);
		printf("请输入补贴:");
		scanf("%f",&msg[i_number].add_money);
		printf("请输入水费:");
		scanf("%f",&msg[i_number].w_fee);
		printf("请输入电费:");
		scanf("%f",&msg[i_number].e_fee);
		msg[i_number].y_money = msg[i_number].base_money+msg[i_number].j_fee+msg[i_number].add_money;
		if(msg[i_number].y_money<=1500) msg[i_number].s_fee*0.03;
		else if(msg[i_number].y_money>=1500 && msg[i_number].y_money<4500) msg[i_number].s_fee = msg[i_number].base_money*0.1;
		else if(msg[i_number].y_money>=4500 && msg[i_number].y_money<9000) msg[i_number].s_fee = msg[i_number].base_money*0.2;
		else if(msg[i_number].y_money>=9000 && msg[i_number].y_money<35000) msg[i_number].s_fee = msg[i_number].base_money*0.25;
		else if(msg[i_number].y_money>=35000 && msg[i_number].y_money<55000) msg[i_number].s_fee = msg[i_number].base_money*0.30;
		else if(msg[i_number].y_money>=55000 && msg[i_number].y_money<80000) msg[i_number].s_fee = msg[i_number].base_money*0.35;
		else if(msg[i_number].y_money>=80000) msg[i_number].s_fee*0.45;
		msg[i_number].t_fee = msg[i_number].y_money-msg[i_number].e_fee-msg[i_number].w_fee-msg[i_number].s_fee;
		i_number++;
	} 
	puts("录入成功"); 
}

void case2()   //信息添加函数 
{
	int t,flag=0;
	system("cls");
	puts("***************>>>>>>当前模式为信息录入<<<<<<*************");
	printf("请输入卡号:");
	scanf("%s",msg[i_number].s_number);
	for(t=0;t<i_number;t++)
	{
		if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
		{
			printf("该成员%d月份工资信息已存在，可输入其其他月份工资信息!!!\n",msg[t].mon);
			flag=msg[t].mon;
			break;
		}
	}
	if(flag)
	{
		strcpy(msg[i_number].name,msg[t].name);
		flag = 0;
	}
	else
	{
		printf("请输入姓名:");
		scanf("%s",msg[i_number].name);
		printf("请输入月份:");
		scanf("%d",&msg[i_number].mon);
		if(msg[i_number].mon == flag)
		{
			puts("已存在！！！");
			return;
		}
		printf("请输入基本工资:");
		scanf("%f",&msg[i_number].base_money);
		printf("请输入津贴:");
		scanf("%f",&msg[i_number].j_fee);
		printf("请输入补贴:");
		scanf("%f",&msg[i_number].add_money);
		printf("请输入水费:");
		scanf("%f",&msg[i_number].w_fee);
		printf("请输入电费:");
		scanf("%f",&msg[i_number].e_fee);
		msg[i_number].y_money = msg[i_number].base_money+msg[i_number].j_fee+msg[i_number].add_money;
		if(msg[i_number].y_money<=1500) msg[i_number].s_fee*0.03;
		else if(msg[i_number].y_money>=1500 && msg[i_number].y_money<4500) msg[i_number].s_fee = msg[i_number].base_money*0.1;
		else if(msg[i_number].y_money>=4500 && msg[i_number].y_money<9000) msg[i_number].s_fee = msg[i_number].base_money*0.2;
		else if(msg[i_number].y_money>=9000 && msg[i_number].y_money<35000) msg[i_number].s_fee = msg[i_number].base_money*0.25;
		else if(msg[i_number].y_money>=35000 && msg[i_number].y_money<55000) msg[i_number].s_fee = msg[i_number].base_money*0.30;
		else if(msg[i_number].y_money>=55000 && msg[i_number].y_money<80000) msg[i_number].s_fee = msg[i_number].base_money*0.35;
		else if(msg[i_number].y_money>=80000) msg[i_number].s_fee*0.45;
		msg[i_number].t_fee = msg[i_number].y_money-msg[i_number].e_fee-msg[i_number].w_fee-msg[i_number].s_fee;
		i_number++;
		puts("添加成功");
	}
}
 
void case3()  //信息浏览函数 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为信息浏览<<<<<<*************");
	puts("序号\t卡号\t姓名\t月份\t基本工资\t津贴\t补贴\t应发工资\t水费\t电费\t税金\t实发工资");
	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%d\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",i+1,msg[i].s_number,msg[i].name,msg[i].mon,
		msg[i].base_money,msg[i].j_fee,msg[i].add_money,msg[i].y_money,
		msg[i].w_fee,msg[i].t_fee,msg[i].s_fee,msg[i].t_fee);
	 } 
}

void case4()  //排序 
{
	int i,r,t,n;
	struct msg temp;
	system("cls");
	puts("***************>>>>>>当前模式为排序<<<<<<*************");	
	printf("请选择排序方法(1.工资卡号升序 2.实发工资降序 3.姓名字典序排序)"); 
	scanf("%d",&n);
	if(n==1)
	{
		for(r=1; r<i_number; r++) 
		{
			for(t=0; t<i_number-1; t++) 
			{
				if(strcmp(msg[t+1].s_number,msg[t].s_number)>0)   //由高到低 
				{  
					temp=msg[t+1];
					msg[t+1]=msg[t];
					msg[t]=temp;
				}
			}
		}
	} 
	else if(n==2)
	{
		for(r=1; r<i_number; r++) 
		{
			for(t=0; t<i_number-1; t++) 
			{
				if(msg[t+1].t_fee<msg[t].t_fee)   //由低到高 
				{  
					temp=msg[t+1];
					msg[t+1]=msg[t];
					msg[t]=temp;
				}
			}
		}
	}
	else if(n==3)
	{
		for(r=1; r<i_number; r++) 
		{
			for(t=0; t<i_number-1; t++) 
			{
				if(strcmp(msg[t+1].name,msg[t].name)<0)    
				{  
					temp=msg[t+1];
					msg[t+1]=msg[t];
					msg[t]=temp;
				}
			}
		}
	}
	else
	{
		puts("输入有误！！");
	}
	case3();
}

void case5()	//c查询 
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息查找<<<<<<*************");
	while(1)
	{
		printf("查找类型：1.姓名 2.卡号 0.退出\n");
		scanf("%d",&cnt);
		puts("卡号\t姓名\t月份\t基本工资\t津贴\t补贴\t应发工资\t水费\t电费\t税金\t实发工资");
		if(cnt==1)
		{
			printf("请输入查找的姓名：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].name) == 0)
				{
					printf("%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",msg[i].s_number,msg[i].name,msg[i].mon,
					msg[i].base_money,msg[i].j_fee,msg[i].add_money,msg[i].y_money,
					msg[i].w_fee,msg[i].t_fee,msg[i].s_fee,msg[i].t_fee);
					flag =1;
					break;
				}
			}		
		}
		else if(cnt==2)
		{
			printf("请输入查找的卡号：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					printf("%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",msg[i].s_number,msg[i].name,msg[i].mon,
					msg[i].base_money,msg[i].j_fee,msg[i].add_money,msg[i].y_money,
					msg[i].w_fee,msg[i].t_fee,msg[i].s_fee,msg[i].t_fee);
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

void case6()	//统计
{
	int i,cnt[2],flag=0;
	float money;
	char str[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式统计<<<<<<*************");	
	printf("请输入起始月份及结束月份，中间空格隔开：");
	scanf("%d %d",&cnt[0],&cnt[1]);
	printf("请输入需统计的卡号：");
	scanf("%s",str);
	puts("卡号\t姓名\t月份\t基本工资\t津贴\t补贴\t应发工资\t水费\t电费\t税金\t实发工资");
	for(i=0;i<i_number;i++)
	{
		if(strcmp(msg[i].name,str)==0)
		{
			if(msg[i].mon>=cnt[0] && msg[i].mon<=cnt[1])
			{
				printf("%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n",msg[i].s_number,msg[i].name,msg[i].mon,
				msg[i].base_money,msg[i].j_fee,msg[i].add_money,msg[i].y_money,
				msg[i].w_fee,msg[i].t_fee,msg[i].s_fee,msg[i].t_fee);
				money += msg[i].t_fee;			//工资计算 
			}	
			flag = 1;
		}	
	} 
	if(flag)
	{
		printf("统计总实发工资为%.2f元\n",money);
		money = 0;
	}
	else
	{
		puts("未找到！！");
	}
} 

void deal() {//主要处理函数
	int k,flag=0;
	while(1) {
		notice();
		scanf("%d",&k);
		system("cls");
		switch(k) {
			case 0: {
				save();		//保存 
				flag=1;
				break;
			}
			case 1: {
				case1();  //录加 
				break;
			}
			case 2: {
				case2();  //添加
				break;
			}
			case 3: {
				case3();  //浏览 
				break;
			}
			case 4: {
				case4();  //排序
				break;
			}
			case 5: {
				case5();  //查找 
				break;
			}
			case 6: {
				case6();  //统计 
				break;
			}
			default: {
				printf("请正确键入命令前数字！\n");
				break;
			}
		}
		if(flag) break;
	}
}

int main() {
	printf(">*************     请键入以下命令行所对应的数字进行操作：    *************<\n");
	init();
	deal(); 
	return 0;
}


