#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char s_number[MIN];  // 职工号 
	char name[MIN];    //姓名 
	int j_pes;		//基本工资
	int add_pes;		//奖金
	int sum_pes;		//总工资 
	int s_pes;		//税收 
}msg[MAX];

void deal();  //调用功能函数 

void init()   //初始导入txt文本文件函数 
{
	int flag=0;
	FILE*fp;  //定义文本字符指针
	if(!(fp=fopen("test.txt","r"))) 
	{
		fp=fopen("test.txt","w+");
		flag = 1;
	}
	while (!feof(fp) && flag==0)   //如果检测不到文本末尾继续读取，储存到相应的变量中 
	{
		fscanf(fp,"%s\t%s\t%d\t%d\t%d\t%d\n",msg[i_number].s_number,msg[i_number].name,
		&msg[i_number].j_pes,&msg[i_number].add_pes,&msg[i_number].sum_pes,&msg[i_number].s_pes);
		i_number++;
	}
	fclose(fp);   //关闭文本 
	puts("读取成功"); 
 } 
 
 void save(){   //保存文本函数 
	FILE*fp;
	int i;
	fp=fopen("test.txt","w"); // 打开文件遍历写入内存中的自信息 
	for(i=0; i<i_number; i++){
	    fprintf(fp, "%s\t%s\t%d\t%d\t%d\t%d\n",msg[i].s_number,msg[i].name,
		msg[i].j_pes,msg[i].add_pes,msg[i].sum_pes,msg[i].s_pes);				
	}
	fclose(fp);
	puts("保存成功"); 
} 

void addobe()		//初始化
{
	int i;
	for(i=0;i<MAX;i++)
	{
		msg[i].s_pes = 0;	
	}	
} 

void notice() {    //进入系统的主界面显示提息
	printf("************************************************************************************************\n");
	printf("\t\t\t\t  欢迎使用职工管理系统\n");
	printf("\t****************************************************************************\n");
	printf("\t\t\t\t     1.录入职工信息。\n");
	printf("\t\t\t\t     2.增加信息。\n");
	printf("\t\t\t\t     3.查询职工信息。\n");
	printf("\t\t\t\t     4.查询所有员工信息。\n");
	printf("\t\t\t\t     5.删除职工信息。\n");
	printf("\t\t\t\t     6.打印职工信息。\n");
	printf("\t\t\t\t     7.统计。\n");
	printf("\t\t\t\t     8.文件保存到硬盘上。\n");
	printf("\t\t\t\t     9.退出职工信息系统。\n");
    printf("*************************************************************************************************\n");
	printf("\t\t\t\t     选择对应编号：(1-9)");

}

void init_msg()   //信息添加函数 
{
	int i,n,t,flag=1;
	system("cls");
	puts("***************>>>>>>当前模式为信息录入状态<<<<<<*************");
	printf("请输入录入职工数量：");
	scanf("%d",&n);
	for(i=0;i<n;i++)   //遍历输入职工信息，存入相应的便量中 
	{
		flag=1;
		printf("第%d位：\n",i+1);
		printf("请输入职工号:");
		scanf("%s",msg[i_number].s_number);
		for(t=0;t<i_number;t++)
		{
			if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
			{
				printf("该成员已存在!!!");
				flag = 0;
				continue;
			}
		}
		if(flag)
		{
			printf("请输入姓名:");
			scanf("%s",msg[i_number].name);
			printf("请输入基本工资:");
			scanf("%d",&msg[i_number].j_pes);
			printf("请输入奖金:");
			scanf("%d",&msg[i_number].add_pes);
			msg[i_number].sum_pes = msg[i_number].add_pes + msg[i_number].j_pes;
			i_number++;
		}
		
	} 
	puts("录入成功"); 
}
 
void add_msg()   //信息添加函数 
{
	int i,t,flag=1;
	system("cls");
	puts("***************>>>>>>当前模式为信息增加状态<<<<<<*************");
	printf("请输入职工号:");
	scanf("%s",msg[i_number].s_number);
	for(t=0;t<i_number;t++)
	{
		if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
		{
			printf("请输入增加的税收:");
			scanf("%d",&msg[i].s_pes);
			msg[i].sum_pes = msg[i].add_pes + msg[i].j_pes - msg[i].s_pes;
			flag = 0;
			break;
		}
	}
	if(!flag)
	{
		puts("信息添加成功"); 
	} 
	else
	{
		puts("未找到");	
	} 
	
} 

void print()  //信息浏览函数 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为信息浏览状态<<<<<<*************");
	puts("职工号\t姓名\t基本工资\t奖金\t总工资\t税收");
	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%s\t%s\t%d\t%d\t%d\t%d\n",msg[i].s_number,msg[i].name,
		msg[i].j_pes,msg[i].add_pes,msg[i].sum_pes,msg[i].s_pes);
	 } 
}

void delet()  //信息删除 
{
	int i,t,flag = 0;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息删除状态<<<<<<*************");
	printf("请输入删除的职工号：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].s_number) == 0)
		{
			for(t=i;t<i_number;t++)
			{
				msg[i] = msg[i+1];
			}
			i_number--;
			flag =1;
			break;
		}
	}
	if(flag)
	{
		puts("删除成功");	
	}
	else
	{
		puts("未找到该用户信息");
	}
}

void find()	//信息查找
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息查找状态<<<<<<*************");
	while(1)
	{
		printf("查找类型：1.职工号 2.姓名 0.退出\n");
		scanf("%d",&cnt);
		if(cnt==2)
		{
			printf("请输入查找的职工号：");
			scanf("%s",name);
			puts("职工号\t姓名\t基本工资\t奖金\t总工资\t税收");
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					printf("%s\t%s\t%d\t%d\t%d\t%d\n",msg[i].s_number,msg[i].name,
					msg[i].j_pes,msg[i].add_pes,msg[i].sum_pes,msg[i].s_pes);
					flag =1;
					break;
				}
			}
		}
		else if(cnt==1)
		{
			printf("请输入查找的姓名：");
			scanf("%s",name);
			puts("职工号\t姓名\t基本工资\t奖金\t总工资\t税收");
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].name) == 0)
				{
					printf("%s\t%s\t%d\t%d\t%d\t%d\n",msg[i].s_number,msg[i].name,
					msg[i].j_pes,msg[i].add_pes,msg[i].sum_pes,msg[i].s_pes);
					flag =1;
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

void tongji()	//工资总分统计 
{
	int i,flag=0,cnt;
	int point[2]={0}; 
	system("cls");
	puts("***************>>>>>>当前模式为信息统计状态<<<<<<*************");
	printf("请输入你要统计的总工资范围(例：5 500)：\n");
	scanf("%d %d",&point[0],&point[1]);
	puts("符号条件有：");
	puts("职工号\t姓名\t基本工资\t奖金\t总工资\t税收");
	for(i=0;i<i_number;i++)
	{
		if(msg[i].sum_pes>=point[0] && msg[i].sum_pes<=point[1])		//进行链表数据对比 
		{
			printf("%s\t%s\t%d\t%d\t%d\t%d\n",msg[i].s_number,msg[i].name,
			msg[i].j_pes,msg[i].add_pes,msg[i].sum_pes,msg[i].s_pes);
		}
	}
} 

void deal() {//主要处理函数
	int k,flag=1,p=5;
	while(flag) {
		notice();
		scanf("%d",&k);
		system("cls");
		switch(k) {
			case 1: {
				init_msg();  //添加 
				break;
			}
			case 2: {
				add_msg();  //增加 
				break;
			}
			case 3: {
				print();  //查询职工信息 
				break;
			}
			case 4: {
				find();  //查询所有员工信息 
				break;
			}
			case 5: {
				delet();  //删除职工信息 
				break;
			}
			case 6: {
				print();  //打印职工信息 
				break;
			}
			case 7: {
				tongji();  //统计 
				break;
			}
			case 8: {
				save();  //件保存到硬盘上
				break;
			}
			case 9: {
				puts("温馨提醒：");
				save();		//保存 
				puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
				flag=0;
				break;
			}
			default: {
				printf("请正确键入命令前数字！\n");
				break;
			}
		}
		puts("\n");
	}
}

int main() {
	addobe();		//初始化
	init();
	deal(); 
	return 0;
}


