#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char s_number[MIN];  // 学号 
	char name[MIN];    //姓名 
	int chinese; //语文 
	int  math;  //数学 
	int english;   //英文
	float sreve; //平均分 
	int sum;   //总分 
}msg[MAX];

void deal();  //调用功能函数 

void init()   //初始导入txt文本文件函数 
{
	FILE*fp;  //定义文本字符指针
	if(!(fp=fopen("student.txt","r"))) 
	{
		fp=fopen("student.txt","r+");
	}
	while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中 
	{
		fscanf(fp,"%s\t%s\t%d\t%d\t%d\t%f\t%d\n",msg[i_number].s_number,msg[i_number].name,&msg[i_number].chinese,
		&msg[i_number].math,&msg[i_number].english,&msg[i_number].sreve,&msg[i_number].sum);
		i_number++;
	}
	fclose(fp);   //关闭文本 
	puts("读取成功"); 
 } 
 
 void save(){   //保存文本函数 
	FILE*fp;
	int i;
	if(!(fp=fopen("student.txt","w"))) // 打开新建的文件遍历写入内存中的自信息 
	{
		fp=fopen("student.txt","w+");
	}  
	for(i=0; i<i_number; i++){
	    fprintf(fp, "%s\t%s\t%d\t%d\t%d\t%.2f\t%d\n",msg[i].s_number,msg[i].name,msg[i].chinese,
		msg[i].math,msg[i].english,msg[i].sreve,msg[i].sum);			
	}
	fclose(fp);
	puts("保存成功"); 
} 

void notice() {    //进入系统的主界面显示提息
	puts("\t\t\t1.成绩信息添加");
	puts("\t\t\t2.信息浏览");
	puts("\t\t\t3.删除");
	puts("\t\t\t4.修改");
	puts("\t\t\t5.查找");
	puts("\t\t\t6.排序"); 
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void case1()   //信息添加函数 
{
	int i,n,t;
	system("cls");
	puts("***************>>>>>>当前模式为课程信息添加状态<<<<<<*************");
	printf("请输入录入学生数量：");
	scanf("%d",&n);
	for(i=0;i<n;i++)   //遍历输入学生信息，存入相应的便量中 
	{
		printf("第%d位：\n",i+1);
		printf("请输入学号:");
		scanf("%s",msg[i_number].s_number);
		for(t=0;t<i_number;t++)
		{
			if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
			{
				printf("该成员已存在!!!");
				continue;
			}
		}
		printf("请输入姓名:");
		scanf("%s",msg[i_number].name);
		printf("请输入语文成绩:");
		while(!scanf("%d",&msg[i_number].chinese))
		{
			fflush(stdin);
			printf("输入有误，只能输入数字，请重新输入：");
		}
		printf("请输入数学成绩:");
		while(!scanf("%d",&msg[i_number].math))
		{
			fflush(stdin);
			printf("输入有误，只能输入数字，请重新输入：");
		}	
		printf("请输入英文成绩:");
		while(!scanf("%d",&msg[i_number].english))
		{
			fflush(stdin);
			printf("输入有误，只能输入数字，请重新输入：");
		}
		msg[i_number].sreve = (float)(msg[i_number].math + msg[i_number].english + msg[i_number].chinese)/3;
		msg[i_number].sum = msg[i_number].math + msg[i_number].english + msg[i_number].chinese;
		i_number++;
	} 
	puts("录入成功"); 
}
 
void case2()  //信息浏览函数 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为信息添加状态<<<<<<*************");
	puts("序号\t学号\t姓名\t语文\t数学\t英文\t总分\t平均分");
	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",i+1,msg[i].s_number,msg[i].name,msg[i].chinese,
			msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
	 } 
}

void case3()  //信息删除 
{
	int i,flag = 0;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息删除状态<<<<<<*************");
	printf("请输入删除的姓名：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].name) == 0)
		{
			msg[i] = msg[i+1];
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

void case4()	//信息修改
{
	int i,flag = 0;
	char name[MIN];
	system("cls");
	puts("***************>>>>>>当前模式为信息修改状态<<<<<<*************");
	printf("请输入需修改的姓名：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].name) == 0)
		{
			printf("请输入修改后的姓名:");
			scanf("%s",msg[i_number].name);
			printf("请输入修改后的语文成绩:");
			while(!scanf("%d",&msg[i_number].chinese))
			{
				fflush(stdin);
				printf("输入有误，只能输入数字，请重新输入：");
			}
			printf("请输入修改后的数学成绩:");
			while(!scanf("%d",&msg[i_number].math))
			{
				fflush(stdin);
				printf("输入有误，只能输入数字，请重新输入：");
			}	
			printf("请输入修改后的英文成绩:");
			while(!scanf("%d",&msg[i_number].english))
			{
				fflush(stdin);
				printf("输入有误，只能输入数字，请重新输入：");
			}
			msg[i_number].sreve = (float)(msg[i_number].math + msg[i_number].english + msg[i_number].chinese)/3;
			msg[i_number].sum = msg[i_number].math + msg[i_number].english + msg[i_number].chinese; 
			flag =1;
			break;
		}
	}
	if(flag)
	{
		puts("修改成功");	
	}
	else
	{
		puts("未找到该用户信息");
	}
} 

void case5()	//信息查找
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息查找状态<<<<<<*************");
	while(1)
	{
		printf("查找类型：1.姓名 2.学号 3.分数区间 0.退出\n");
		scanf("%d",&cnt);
		if(cnt==1)
		{
			printf("请输入查找的姓名：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].name) == 0)
				{
					puts("学号\t姓名\t语文\t数学\t英文\t总分\t平均分");
					printf("%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",msg[i].s_number,msg[i].name,msg[i].chinese,
					msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
					flag =1;
					break;
				}
			}		
		}
		else if(cnt==2)
		{
			printf("请输入查找的学号：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					puts("学号\t姓名\t语文\t数学\t英文\t总分\t平均分");
					printf("%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",msg[i].s_number,msg[i].name,msg[i].chinese,
					msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
					flag =1;
					break;
				}
			}
		}
		else if(cnt==3)
		{
			int grades[2] = {0};
			cnt=0;
			printf("请输入查找的分数范围及科目(例：语文 25 50)：");
			scanf("%s %d %d",name,grades[0],grades[1]);
			puts("序号\t学号\t姓名\t语文\t数学\t英文\t总分\t平均分");
			for(i=0;i<i_number;i++)
			{
				if(strcmp("语文",name)==0)
				{
					if(msg[i].chinese>grades[0] && msg[i].chinese<grades[1])
					{
						printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",cnt+1,msg[i].s_number,msg[i].name,msg[i].chinese,
						msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
						flag =1;
						cnt++;
					}	
				}
				else if(strcmp("数学",name)==0)
				{
					if(msg[i].math>grades[0] && msg[i].math<grades[1])
					{
						printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",cnt+1,msg[i].s_number,msg[i].name,msg[i].chinese,
						msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
						flag =1;
						cnt++;
					}
				}
				else if(strcmp("英文",name)==0)
				{
					if(msg[i].english>grades[0] && msg[i].english<grades[1])
					{
						printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",cnt+1,msg[i].s_number,msg[i].name,msg[i].chinese,
						msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
						flag =1;
						cnt++;
					}
				}
			}
		}
		else if(cnt==0)
		{
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

void case6()	//成绩总分排序
{
	int i,r,t;
	struct msg temp;
	system("cls");
	puts("***************>>>>>>当前模式为成绩排序状态<<<<<<*************");	
	for(r=1; r<i_number; r++) 
	{
		for(t=0; t<i_number-1; t++) 
		{
			if(msg[t+1].sum > msg[t].sum)   //由高到低 
			{  
				temp=msg[t+1];
				msg[t+1]=msg[t];
				msg[t]=temp;
			}
		}
	}
	puts("序号\t学号\t姓名\t语文\t数学\t英文\t总分\t平均分");
	for(i=0;i<i_number-1;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",i+1,msg[i].s_number,msg[i].name,msg[i].chinese,
			msg[i].math,msg[i].english,msg[i].sum,msg[i].sreve);
	} 
} 

void deal() {//主要处理函数
	int k,flag,p;
	p=5;
	flag=1;
	while(flag) {
		if(p>1)
			notice();
			scanf("%d",&k);
			system("cls");
			switch(k) {
				case 0: {
					puts("温馨提醒：");
					save();		//保存 
					puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
					flag=0;
					break;
				}
				case 1: {
					case1();  //添加 
					break;
				}
				case 2: {
					case2();  //浏览 
					break;
				}
				case 3: {
					case3();  //删除 
					break;
				}
				case 4: {
					case4();  //修改 
					break;
				}
				case 5: {
					case5();  //查找 
					break;
				}
				case 6: {
					case6();  //排序 
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
	printf(" >************欢 迎 您 使 用 学 生 成 绩 管 理 系 统************<\n");
	printf(">*************     请键入以下命令行所对应的数字进行操作：    *************<\n");
	init();
	deal(); 
	return 0;
}


