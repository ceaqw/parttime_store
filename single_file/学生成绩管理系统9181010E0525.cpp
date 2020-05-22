#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number;  //全局变量，记录人数 

struct LNode {       //定义结构体来储存信息 
	char s_number[MIN];  // 学号 
	char name[MIN];    //姓名 
	int chinese; //语文 
	int  math;  //数学 
	int english;   //英文
	float sreve; //平均分 
	int sum; 
}LNode[MAX];

void deal();  //调用功能函数 

void init()   //初始导入txt文本文件函数 
{
	FILE*fp;  //定义文本字符指针 
	int n;
	printf("请输入读取的文件名：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1){
		char fp_name[MIN];
		printf("请输入文件名：");
		scanf("%s",fp_name);
		while((fp=fopen(fp_name,"r"))==NULL)
		{ 
			printf("文件不存在，请重新输入：");
			scanf("%s",fp_name);	 
		}	
	//	fp = fopen(fp_name,"r"); // 以读取的格式打开文本 
		while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中 
		{
			fscanf(fp, "%s\t%s\t%d\t%d\t%d\t%f\t%d\n",LNode[i_number].s_number,LNode[i_number].name,&LNode[i_number].chinese,
			&LNode[i_number].math,&LNode[i_number].english,&LNode[i_number].sreve,&LNode[i_number].sum);
			i_number++;
		}
		fclose(fp);   //关闭文本 
		puts("读取成功"); 
		deal();
	}
	if(n==2){
		deal();
	}
 } 
 
 void save(){   //保存文本函数 
	FILE*fp;
	int n,i;
	printf("是否另存文件(文件命名格式：英文.txt或其他后缀)：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1){
		char fp_name[MIN];
		printf("请输入保存文件名称：");
		scanf("%s",fp_name);
		fp = fopen(fp_name,"w");  // 打开新建的文件遍历写入内存中的自信息 
		for(i=0; i<i_number; i++){
		    fprintf(fp, "%s\t%s\t%d\t%d\t%d\t%.2f\t%d\n",LNode[i].s_number,LNode[i].name,LNode[i].chinese,
			LNode[i].math,LNode[i].english,LNode[i].sreve,LNode[i].sum);			
		}
		fclose(fp);
		puts("保存成功"); 
	}	
} 

void notice() {    //进入系统的主界面显示提息
	puts("\t\t\t1.成绩信息添加");
	puts("\t\t\t2.信息浏览");
	puts("\t\t\t3.平均分分布统计"); 
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
		scanf("%s",LNode[i_number].s_number);
		for(t=0;t<i_number;t++)
		{
			if(strcmp(LNode[t].s_number,LNode[i_number].s_number) == 0)
			{
				printf("该学生已存在");
				continue;
			}
		}
		printf("请输入姓名:");
		scanf("%s",LNode[i_number].name);
		printf("请输入语文成绩:");
	//	scanf("%d",&LNode[i_number].chinese);
		while(!scanf("%d",&LNode[i_number].chinese))
		{
			fflush(stdin);
			printf("输入有误，只能输入数字，请重新输入：");
		}
		printf("请输入数学成绩:");
		while(!scanf("%d",&LNode[i_number].math))
		{
			fflush(stdin);
			printf("输入有误，只能输入数字，请重新输入：");
		}	
		printf("请输入英文成绩:");
		while(!scanf("%d",&LNode[i_number].english))
		{
			fflush(stdin);
			printf("输入有误，只能输入数字，请重新输入：");
		}
		LNode[i_number].sreve = (float)(LNode[i_number].math + LNode[i_number].english + LNode[i_number].chinese)/3;
		LNode[i_number].sum = LNode[i_number].math + LNode[i_number].english + LNode[i_number].chinese;
		i_number++;
	} 
	puts("录入成功"); 
	deal();
}
 
void case2()  //信息浏览函数 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为课程信息添加状态<<<<<<*************");
	puts("序号\t学号\t\t姓名\t语文\t数学\t英文\t总分\t平均分");
	for(i=0;i<i_number;i++)    //打印内存中储存的学生信息 
	{
		if(strlen(LNode[i].s_number)<8)
		{
			printf("%d\t%s\t\t%s\t%d\t%d\t%d\t%d\t%.2f\n",i+1,LNode[i].s_number,LNode[i].name,LNode[i].chinese,
			LNode[i].math,LNode[i].english,LNode[i].sum,LNode[i].sreve);
		}
		else
		{
			printf("%d\t%s\t%s\t%d\t%d\t%d\t%d\t%.2f\n",i+1,LNode[i].s_number,LNode[i].name,LNode[i].chinese,
			LNode[i].math,LNode[i].english,LNode[i].sum,LNode[i].sreve);
		}
	 } 
	save(); 
}

void case3()  //信息可视化统计函数 
{
	int i,n,score[5]={0};
	for(i=0;i<i_number;i++)  //检测每个成员的平均成绩的区间 
	{
		if(LNode[i].sreve < 60)
		{
			score[0]++;
		}
		else if(LNode[i].sreve < 70)
		{
			score[1]++;
		}
		else if(LNode[i].sreve < 80)
		{
			score[2]++;
		}
		else if(LNode[i].sreve < 90)
		{
			score[3]++;
		}
		else
		{
			score[4]++;
		}
	}
	
	for(i=0;i<5;i++)   //循环将各个区间的人数划分为5个等级 
	{
		score[i] = score[i]%6;
	}
	
	puts("*********************************************");  //图形打印 
	for(i=0;i<5;i++)
	{
		for(n=0;n<5;n++)
		{
			if(score[n] >= 5-i)
			{
				printf("I\t");
			}
			else
			{
				printf("\t");
			}
		}
		printf("\n");
	}
	puts("<60\t<70\t<80\t<90\t≦100"); 
	puts("*********************************************");	
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
					puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
					flag=0;
					break;
				}
				case 1: {
					case1();
					break;
				}
				case 2: {
					case2();
					break;
				}
				case 3: {
					case3();
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
	printf(" >************欢 迎 您 使 用 南 京 理 工 大 学 学 生 成 绩 管 理 系 统************<\n");
	printf(">*************     请键入以下命令行所对应的数字进行操作：    *************<\n");
	init();
	return 0;
}

