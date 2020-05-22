#include <stdio.h> 
#include <stdlib.h>
//#include<conio.h>
#include <string.h>
#include <math.h>
#define MIN  20 
#define MAX  1000

int i_number = 0;
int cnt;
FILE *fp;

struct zixu
{
    int shu;
    char coll[MIN];
}pxu[100];

struct Node //定义结构体
{       
	char gong[MIN];    //教工号
	char name[MIN];  //姓名 
	char brith[MIN];   //出生日期
	char college[MIN]; //学院
	char adss[MIN];   //住址
	char phone[MIN];      //联系方式 
}LNode[MAX];

void init(void) //文件的初始化读取
{   
	while (!feof(fp)) 
	{
		fscanf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n",LNode[i_number].gong, LNode[i_number].name,
		       LNode[i_number].brith, LNode[i_number].college, LNode[i_number].adss,LNode[i_number].phone);
		i_number++;
	}
} 

void file_save(void) //信息以文件格式录入函数
{
	int i ; 
	fp = fopen("data.txt", "w+");
	for(i=0;i<i_number;i++)
	{ 
		if(i<1)
		{
		}
		else
		{
			fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\n",LNode[i].gong, LNode[i].name,
			LNode[i].brith, LNode[i].college, LNode[i].adss,LNode[i].phone);			
		}			
	}
	fclose(fp);
}

void deal(void);
void save(void)
{
	FILE*Mg;
	int n,i;
	printf("是否另存文件(文件命名格式：英文.txt或其他后缀)：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1)
	{
		char fp_name[MIN];
		printf("请输入保存文件名称：");
		scanf("%s",fp_name);
		Mg = fopen(fp_name,"w+");
		fprintf(fp,"序号\t教师工号\t姓名\t出生日期（年/月）\t学院\t住址\t联系方式\n"); 
		cnt = 1;
		for(i=0; i<i_number; i++)
		{
		    fprintf(fp, "%d\t%s\t%s\t%s\t%s\t%s\t%s\n", cnt, LNode[i].gong,LNode[i].name,LNode[i].brith,LNode[i].college,LNode[i].adss,LNode[i].phone);
			cnt++;			
		}
		fclose(Mg);
		puts("保存成功"); 
	}
	if(n==2)
	{
		deal();
	}
	
} 

void notice(void) //进入系统的主界面显示提息
{
	puts("\t\t\t1.教师信息添加");
	puts("\t\t\t2.教师信息总览");
	puts("\t\t\t3.教师信息修改");
	puts("\t\t\t4.教师信息统计");
	puts("\t\t\t5.教师信息查询");
	puts("\t\t\t6.排课记录删除");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}


void case1(void) 
{    
	int flag,i;
	char wqs[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为课程信息录入状态<<<<<<*************");
	printf("请输入教师工号：");
	scanf("%s",wqs);
	flag=1;            //标识变量，用以判断该信息是否已存在
	for(i=0; i<i_number; i++)  {          //判断输入数据的正误，即文件中是否已存在该信息
		if(strcmp(LNode[i].gong,wqs)==0) {			//利用strcpy(a,b)函数比较字符串a与b是否相等，需调用string.h头文件。下面将多次用到，用法相同不再一一例举 
			flag=0;
		}
		} 
	if(flag==0){ 
		puts("该课程已经存在，如需修改请转到修改命令！");
	} 
	else if(flag==1) {
			strcpy(LNode[i_number].gong,wqs);
			printf("请输入姓名:");
			scanf("%s",&LNode[i_number].name);
			printf("请输入出生日期（年/月） :");
			scanf("%s",&LNode[i_number].brith);
			printf("请输入学院 :");
			scanf("%s",&LNode[i_number].college);
			printf("请输入出住址 :");
			scanf("%s",&LNode[i_number].adss);
			printf("请输入联系方式 :");
			scanf("%s",&LNode[i_number].phone);
			file_save(); 
			i_number++;
		}
	puts("录入成功");
}

void SHOW(void)
{
	system("cls");
	int i,n; 
	printf("序号\t教工号\t姓名\t出生日期（年/月）\t学院\t住址\t联系方式\n");
	cnt = 0; 
	for(i=0; i<i_number; i++){			//遍历数组打印 
	
	    printf("%d\t%s\t%s\t%s\t%s\t%s\t%s\n",cnt,LNode[i].gong,LNode[i].name,LNode[i].brith,LNode[i].college,LNode[i].adss,LNode[i].phone);
		cnt++;
    }
	puts("请输入：1.另存 2.返回主菜单");
	scanf("%d",&n);
	if(n==0){
		deal();
	}
	if(n==1){
		save();
	}	
		   		
} 

void case3(void)
{
	int flag = 0,i,t,n;
	system("cls");
	puts("***************>>>>>>当前模式为教工信息修改状态<<<<<<*************");
	printf("选定修改项：1.修改所有信息 2.修改对应单个项目 0.返回上一层   请键入：");
	scanf("%d",&t);
		if(t==1) 
		{
			n = 0;
			char bj[MIN];
			puts("请输入所需修改的教师工号：");
			scanf("%s",bj);
			for(i=0; i<i_number; i++)//判断输入数据的正误
			{ 
				if(strcmp(LNode[i].gong,bj)==0) //strcpy库函数调用
				{		 
					flag=1;
				}
			    n++;
			} 
			if(flag==0)
		   {
				puts("该教工信息不存在，请检查输入是否有误！");
				case3();
		   }
			else
			{
				puts("请输入修改后的姓名：");
				char num1[MIN];
				scanf("%s",num1);
				strcpy(LNode[n].name,num1);
				puts("请输入修改后的出生日期：");
				char num2[MIN];
				scanf("%s",num2);
				strcpy(LNode[n].brith,num2);
				puts("请输入修改后的学院：");
				char num3[MIN];
				scanf("%s",num3);
				strcpy(LNode[n].college,num3);
				puts("请输入修改后的住址：");
				char num4[MIN];
				scanf("%s",num4);
				strcpy(LNode[n].adss,num4);
				puts("请输入修改后的联系方式：");
				char num5[MIN];
				scanf("%s",num5);
				strcpy(LNode[n].phone,num5);			
			}
		}
		if(t==2)
		{
			n = 0;
			char bj[MIN],num[MIN]; 
			int cut,n;  
			printf("请请输入所需修改的教师工号：");
			scanf("%s",bj);
			for(i=0; i<i_number; i++)//判断输入数据的正误
			{ 
				int n = 0;
				if(strcmp(LNode[i].gong,bj)==0) //strcpy库函数调用
				{		 
					puts("请输入所需修改项：1.姓名 2.出生日期 3.学院 4.住址 5.联系方式 0.返回上一层");
					scanf("%d",&cut); 
					switch(cut) 
					{
						case 0: 
						{
							case3();
							break;
						}
						case 1: 
						{
							puts("请输入修改后的姓名：");
							char num1[MIN];
							scanf("%s",num1);
							strcpy(LNode[n].name,num1);		
							break;
						}
						case 2: 
						{
							puts("请输入修改后的出生日期：");
							char num2[MIN];
							scanf("%s",num2);
							strcpy(LNode[n].brith,num2);
							break;
						}
						case 3: 
						{
							puts("请输入修改后的学院：");
							char num3[MIN];
							scanf("%s",num3);
							strcpy(LNode[n].college,num3);
							break;
						}
						case 4: 
						{
							puts("请输入修改后的住址：");
							char num4[MIN];
							scanf("%s",num4);
							strcpy(LNode[n].adss,num4);
							break;
						}
						case 5: 
						{
							puts("请输入修改后的联系方式：");
							char num5[MIN];
							scanf("%s",num5);
							strcpy(LNode[n].phone,num5);
							break;
						}
					}			
					printf("修改完成"); 
							flag=1;
				}
				n++;
			} 
			if(flag==0)
			{ 
				puts("该教工信息不存在，请检查输入是否有误！");
				case3();
			} 
			if(flag==0)
			{ 
				puts("该教工信息不存在，请检查输入是否有误！");
			} 
		} 
		if(t==0)
		{
			deal();
		}
	file_save();	
}

void PX (void)
{
	int cnt = 0,flag,i,r,t;
    int number = 0;
	struct zixu temp;//用于排序的中间变量
	char wqs[MIN] ;
	system("cls");
	puts("***************>>>>>>当前模式教工信息统计状态<<<<<<*************");
	while(flag)
	{
		puts("请输入需要排序的学院名称：");
		int n;
		scanf("%s",wqs);
		for(i=0; i<i_number; i++)
		{ 
				if(strcmp(LNode[i].college,wqs)==0) //strcpy函数调用 
				{			
					cnt++;
				}
		} 
		pxu[number].shu = cnt;
		strcpy(pxu[number].coll,wqs);
		number++;		
		puts("是否继续录入：  1.是 2.否");
		scanf("%d",&n);
		if(n==1)
		{
			flag = 1;
		} 
		if(n==2)
		{
			flag = 0;
		}
	}
	for(r=1; r<=number; r++) 
	{
		for(t=0; t<=number-r; t++) 
		{
			if(pxu[t+1].shu>pxu[t].shu)//由高到低 
			{ 
				temp = pxu[t+1];
				pxu[t+1]=pxu[t];
				pxu[t]=temp;
			}
		}
	}
	puts("***************>>>>>>排名信息总览<<<<<<*************");
	puts("序号\t学院名称\t教师数量\n");
	int nt=1;
	for(i=0; i<number; i++)
	{	
		printf("%d\t%s\t%d\n",nt,pxu[i].coll,pxu[i].shu);
		nt++;
	}		
}

void case4(void) 
{
	int i,flag=0,k;
	char wqs[MIN];
	k=1;
	system("cls");
	puts("***************>>>>>>当前模式教工查询信息状态<<<<<<*************");
	while(k) 
	{
		int n;
		puts("请输入查询项：  1.输入课教工号查询 2.输入出生日期查询 3.输入学院查询 0.返回主菜单");
		scanf("%d",&n);
		if(n==1)
		{
			printf("请键入教工号： ");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++)	//遍历数组打印 
			{		
				if(strcmp(LNode[i].gong, wqs)==0) 
				{
					cnt = 1; 
					printf("序号\t课程编号\t教工号\t姓名\t出生日期（年/月）\t学院\t住址\t联系方式\n");
					printf("%d\t",cnt);
					printf("%s\t",LNode[i].gong);
					printf("%s\t",LNode[i].name);
					printf("%s\t",LNode[i].brith);
					printf("%s\t",LNode[i].college);
					printf("%s\t",LNode[i].adss);
					printf("%s\n",LNode[i].phone);
					flag=1;
				}
			} 
		}
		if(n==2)
		{
			printf("请键入出生日期： ");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++)//遍历数组打印 
			{			
				if(strcmp(LNode[i].brith, wqs)==0) 
				{
					cnt = 1; 
					printf("序号\t教工号\t姓名\t出生日期（年/月）\t学院\t住址\t联系方式\n");
					printf("%d\t",cnt);
					printf("%s\t",LNode[i].gong);
					printf("%s\t",LNode[i].name);
					printf("%s\t",LNode[i].brith);
					printf("%s\t",LNode[i].college);
					printf("%s\t",LNode[i].adss);
					printf("%s\n",LNode[i].phone);
					cnt++;
					flag=1;
				}
			} 
		}
		if(n==3)
		{
			printf("请键入学院：");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++)
			{			//遍历数组打印 
				if(strcmp(LNode[i].college, wqs)==0) 
				{
					cnt = 1; 
					printf("序号\t教工号\t姓名\t出生日期（年/月）\t学院\t住址\t联系方式\n");
					printf("%d\t",cnt);
					printf("%s\t",LNode[i].gong);
					printf("%s\t",LNode[i].name);
					printf("%s\t",LNode[i].brith);
					printf("%s\t",LNode[i].college);
					printf("%s\t",LNode[i].adss);
					printf("%s\n",LNode[i].phone);
					cnt++;
					flag=1;
				}
		   }
		}
		if(n==0)
		{
			deal();
		} 
		if(flag==0)
		{ 
			puts("未查找到该课程信息，如需录入信息请重新键入命令.");
		} 
		puts("请按照文字提示键入命令："); 
		printf("\t\t1.继续查询\n\t\t0.退出查询\n请键入命令:");
		scanf("%d",&k);
	}
}



void case5(void)
{
	int i,j,flag=0,k;
	char wqs[MIN];
	k=1;
	system("cls");
	while(k) 
	{
		puts("请输入需要删除的教工号："); 
		scanf("%s",wqs);
		for(i=0; i<i_number; i++){ 
			if(strcmp(LNode[i].gong,wqs)==0) //strcpy函数调用 
			{			
				for(i;i<i_number;i++)
				{
					LNode[i] = LNode[i+1];
				} 
				puts("删除完成");
				i_number--;				
				flag=1;	 
			}	
		}
		if(flag==0){ 
			puts("未查找到该课程信息，如需录入信息请重新键入命令.");
		} 
		puts("请按照文字提示键入命令：");
		printf("\t\t1.继续删除\n\t\t0.退出删除\n请键入命令:");
		scanf("%d",&k);
	}
	file_save();
}


void deal(void) //主要处理函数
{
	int k,flag,p;
	p=1;
	flag=1;
	while(flag) 
	{
		if(p>0)
		{ 
			notice();
			scanf("%d",&k);
			system("cls");
			switch(k) 
			{
				case 0: 
				{
					puts("温馨提醒：");
					file_save();
					
					puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
					flag=0;
					
					break;
				}
				case 1: 
				{
					case1();
					break;
				}
				case 2: 
				{
					SHOW();
					break;
				}
				case 3: 
				{
					case3();
					break;
				}
				case 4: 
				{
					PX();
					break;
				}
				case 5: 
				{
					case4();
					break;
				}
				case 6: 
				{
					case5();
					break;
				}
				default: 
				{
					printf("请正确键入命令前数字！\n");
					break;
				}
			} 
		}
		p=1103;
		puts("\n");
	}
}
int main() 
{
	if ((fp = fopen("data.txt", "r+")) == NULL) 
	{
		fp = fopen("data.txt", "w+");
	}
	init();
	printf(" >************欢 迎 您 使 用 课 程 信 息 管 理 系 统************<\n");
	printf(">*************     请键入以下命令行所对应的数字进行操作：     *************<\n");
	printf(">*************   请确定正确退出程序,否则文件将不被正常保存.     *************<\n");
	deal();//处理数据
	return 0;
}
