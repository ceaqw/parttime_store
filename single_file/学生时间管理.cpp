#include <stdio.h> 
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000


int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char s_number[MIN];  // 学号 
	char name[MIN];    //姓名 
	char sex[MIN];		//性别 
	char f_time_thing[MAX];		//上午时间安排 
	char l_time_thing[MAX];		//下午时间安排 
	char other_time_thing[MAX];		//其他特殊安排 
}msg[MAX];


void notice() {    //进入系统的主界面显示提息
	puts("\t\t\t1.学生时间信息添加");
	puts("\t\t\t2.信息浏览");
	puts("\t\t\t3.修改");
	puts("\t\t\t4.查询");
	puts("\t\t\t5.排序查看");
	puts("\t\t\t6.读取文档");
	puts("\t\t\t7.保存文档");
	puts("\t\t\t8.统计");
	puts("\t\t\t9.插入函数");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void init()   //初始导入dat文本文件函数 
{
	FILE*fp;  //定义文本字符指针 
	int n;
	char fp_name[MIN];
	printf("请输入读取的文件名：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1){
		printf("请输入文件名(例：test.dat)：");
		scanf("%s",fp_name);
		while((fp=fopen(fp_name,"rb"))==NULL)
		{ 
			printf("文件不存在，请重新输入：");
			scanf("%s",fp_name);	 
		}	
	//	fp = fopen(fp_name,"r"); // 以读取的格式打开文本 
		while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中 
		{
			fscanf(fp,"%s\t%s\t%s\t%s\t%s\t%s\n",msg[i_number].s_number,msg[i_number].name,msg[i_number].sex,
			msg[i_number].f_time_thing,msg[i_number].l_time_thing,msg[i_number].other_time_thing);
			i_number++;
		}
		fclose(fp);   //关闭文本 
		puts("读取成功"); 
	}
 } 

// 保存信息函数
void save()   //初始导入dat文本文件函数 
{
	FILE*fp;  //定义文本字符指针 
	int i; 
	char fp_name[MIN];
	printf("请输入导出的文件名(例：test.dat)：");
	scanf("%s",fp_name);
	if((fp=fopen(fp_name,"wb"))==NULL)
	{ 
		fp=fopen(fp_name,"wb+");	 
	}	
	for(i=0;i<i_number;i++)   //如果检测不到文本末尾继续读取，储存到相应的变量中 
	{
		fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\n",msg[i].s_number,msg[i].name,msg[i].sex,
			msg[i].f_time_thing,msg[i].l_time_thing,msg[i].other_time_thing);
	}
	fclose(fp);   //关闭文本 
	puts("保存成功"); 
 } 

// 添加信息函数
void add_msg()   //信息添加函数 
{
	int i,n,t,flag=0;
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
				flag = 1; 
				continue;
			}
		}
		if(!flag)
		{
			printf("请输入姓名:");
			scanf("%s",msg[i_number].name);
			printf("请输入性别:");
			scanf("%s",msg[i_number].sex);
			printf("请输入上午安排(没有安排就输入无):");
			scanf("%s",msg[i_number].f_time_thing);
			printf("请输入下午安排(没有安排就输入无):");
			scanf("%s",msg[i_number].l_time_thing);
			printf("请输入特殊时间安排(没有安排就输入无):");
			scanf("%s",msg[i_number].other_time_thing);
			i_number++;
		}
	} 
	puts("录入成功"); 
}

void Print()  //信息浏览函数 
{
	int i;
	char str[20] = {0};

	system("cls");
	puts("***************>>>>>>当前模式为信息浏览状态<<<<<<*************");

	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("序号：%d\t学号：%s\t姓名：%s\t性别：%s\n",i+1,msg[i].s_number,msg[i].name,msg[i].sex);
		printf("上午安排：%s\n",msg[i].f_time_thing);
		printf("下午安排：%s\n",msg[i].l_time_thing);
		printf("特殊时间安排：%s\n\n",msg[i].other_time_thing);
	}

	puts("-----------------------------------------------------------------");
	seek:
	printf("是否查询相关人员信息(1.是 0.否)：");
	scanf("%d", &i);
	if (!i) return;
	else if (i == 1)
	{
		printf("请选择查询依据(1.姓名 2.学号)：");
		scanf("%d", &i);

		if(i == 1)
		{
			printf("请输入你要查找的姓名：");
			scanf("%s", str);

			for (int j = 0; j < i_number; ++j)
			{
				if (strcmp(str, msg[j].name) == 0)
				{
					printf("序号：%d\t学号：%s\t姓名：%s\t性别：%s\n",i+1,msg[i].s_number,msg[i].name,msg[i].sex);
					printf("上午安排：%s\n",msg[i].f_time_thing);
					printf("下午安排：%s\n",msg[i].l_time_thing);
					printf("特殊时间安排：%s\n\n",msg[i].other_time_thing);				
				}
			}
		}
		else if (i == 2)
		{
			printf("请输入你要查找的学号：");
			scanf("%s", str);

			for (int j = 0; j < i_number; ++j)
			{
				if (strcmp(str, msg[j].s_number) == 0)
				{
					printf("序号：%d\t学号：%s\t姓名：%s\t性别：%s\n",i+1,msg[i].s_number,msg[i].name,msg[i].sex);
					printf("上午安排：%s\n",msg[i].f_time_thing);
					printf("下午安排：%s\n",msg[i].l_time_thing);
					printf("特殊时间安排：%s\n\n",msg[i].other_time_thing);				
				}
			}
		}
		else
		{
			puts("输入有误！！！");
		}
	}
	else
	{
		puts("输入有误！！，请重试：");
		goto seek;
	} 
}


// 插入
void Insert()
{
	int num = 0;
	int flag = 0;
	struct msg temp;
	puts("\t\t\t插入数据函数");
	
	fla:
	printf("请输入插入位置：");
	scanf("%d", &num);

	if (num<0 || num>i_number)
	{
		printf("输入无效！！请重试！！\n");
		goto fla;
	}

	printf("请输入学号:");
	scanf("%s",temp.s_number);
	for(int t=0;t<i_number;t++)
	{
		if(strcmp(msg[t].s_number,temp.s_number) == 0)
		{
			printf("该成员已存在!!!");
			flag = 1; 
			continue;
		}
	}
	if(!flag)
	{
		printf("请输入姓名:");
		scanf("%s",temp.name);
		printf("请输入性别:");
		scanf("%s",temp.sex);
		printf("请输入上午安排(没有安排就输入无):");
		scanf("%s",temp.f_time_thing);
		printf("请输入下午安排(没有安排就输入无):");
		scanf("%s",temp.l_time_thing);
		printf("请输入特殊时间安排(没有安排就输入无):");
		scanf("%s",temp.other_time_thing);
		i_number++;
	}


	for (int i = i_number; i > i_number-num; --i)
	{
		msg[i] = msg[i-1];
	}

	msg[num] = temp;

	puts("插入成功！！");
}

void Change()	//信息修改
{
	int i,flag = 0,cnt;
	char name[MIN];
	system("cls");
	puts("***************>>>>>>当前模式为信息修改状态<<<<<<*************");
	printf("请输入需修改的学号：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].s_number) == 0)
		{
			while(1)
			{
				printf("请选择修改项(1.姓名 2.性别 3.上午安排 4.下午安排 5.特殊时间安排 0.退出)：");
				scanf("%d",&cnt);
				if(cnt==0)
				{
					flag = 1;
					break;	
				} 
				else if(cnt==1)
				{
					printf("姓名为：%s\n",msg[i].name); 
					printf("请输入修改后的姓名：");
					scanf("%s",msg[i].name);
				}
				else if(cnt==2)
				{
					printf("性别为：%s\n",msg[i].sex); 
					printf("请输入修改后的性别：");
					scanf("%s",msg[i].sex);
				}
				else if(cnt==3)
				{
					printf("上午安排为：%s\n",msg[i].f_time_thing); 
					printf("请输入修改后的上午安排：");
					scanf("%s",msg[i].f_time_thing);
				}
				else if(cnt==4)
				{
					printf("下午安排为：%s\n",msg[i].l_time_thing); 
					printf("请输入修改后的下午安排：");
					scanf("%s",msg[i].l_time_thing);
				}
				else if(cnt==5)
				{
					printf("特殊时间安排为：%s\n",msg[i].other_time_thing); 
					printf("请输入修改后的特殊时间安排：");
					scanf("%s",msg[i].other_time_thing);
				}
			}
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

void Find()	//信息查找
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息查找状态<<<<<<*************");
	while(1)
	{
		printf("查找类型：1.学号查询 2.姓名查询 0.退出\n");
		scanf("%d",&cnt);
		if(cnt==1)
		{
			system("cls");
			printf("请输入查找的学号：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					printf("学号：%s\t姓名：%s\t性别：%s\n",msg[i].s_number,msg[i].name,msg[i].sex);
					printf("上午安排：%s\n",msg[i].f_time_thing);
					printf("下午安排：%s\n",msg[i].l_time_thing);
					printf("特殊时间安排：%s\n\n",msg[i].other_time_thing);
					flag =1;
				}
			}		
		}
		else if(cnt==2)
		{
			system("cls");
			printf("请输入查找的姓名：");
			scanf("%s",name);
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].name) == 0)
				{
					printf("学号：%s\t姓名：%s\t性别：%s\n",msg[i].s_number,msg[i].name,msg[i].sex);
					printf("上午安排：%s\n",msg[i].f_time_thing);
					printf("下午安排：%s\n",msg[i].l_time_thing);
					printf("特殊时间安排：%s\n\n",msg[i].other_time_thing);
					flag =1;
				}
			}
		}
		else if(cnt==0)
		{
			flag = 1; 
			break;
		}	
	}
	if(!flag)
	{
		puts("未找到该用户信息");	
	}
}

// 统计函数
void Statistics()
{
	int num;
	char str[2][20] = {0};

	while(1)
	{
		printf("请选择统计项 1.统计人数 2.统计学号范围 0.退出\n");
		printf("请输入：");
		scanf("%d", &num);

		if (!num)
		{
			break;
		}
		else if (num == 1)
		{
			int number = 0;
			for (int i = 0; i < i_number; ++i)
			{
				if (strcmp("女", msg[i].sex) == 0)
				{
					number++;
				}
			}

			printf("女生人数：%d,男生人数：%d \n", number, i_number-number);
		}	
		else if (num == 2)
		{
			printf("你输入其实范围:\n");
			printf("示例(311 456)  ");

			scanf("%s%s", str[0], str[1]);

			for (int i = 0; i < i_number; ++i)
			{
				if (strcmp(str[0],msg[i].s_number)<0 && strcmp(str[1], msg[i].s_number)>0)
				{
					printf("学号：%s\t姓名：%s\t性别：%s\n",msg[i].s_number,msg[i].name,msg[i].sex);
					printf("上午安排：%s\n",msg[i].f_time_thing);
					printf("下午安排：%s\n",msg[i].l_time_thing);
					printf("特殊时间安排：%s\n\n",msg[i].other_time_thing);	
				}
			}
		}
	}
	
}



void Sort()		//排序
{
	
	int i,r,t;
	struct msg temp;
	system("cls");
	puts("***************>>>>>>当前模式为学号升序排序<<<<<<*************");	
	for(r=1; r<i_number; r++) 
	{
		for(t=0; t<i_number-1; t++) 
		{
			if(strcmp(msg[t+1].s_number,msg[t].s_number)<0)   //由高到低 
			{  
				temp=msg[t+1];
				msg[t+1]=msg[t];
				msg[t]=temp;
			}
		}
	}	
	puts("排序结果为：");
	Print();
} 

void deal() {//主要处理函数
	int k,flag=1;
	flag=1;
	// puts("欢迎使用学生时间管理系统");
	while(flag) 
	{
		notice();
		scanf("%d",&k);
		system("cls");
		switch(k) 
		{
			case 0: {
				puts("程序已推出：");
				flag=0;
				break;
			}
			case 1: {
				add_msg();  //添加 
				break;
			}
			case 2: {
				Print();  //浏览 
				break;
			}
			case 3: {
				Change();  //修改 
				break;
			}
			case 4: {
				Find();  //查询
				break;
			}
			case 5: {
				Sort();  //排序 
				break;
			}
			case 6: {
				init();  //读取 
				break;
			}
			case 7: {
				save();  //保存 
				break;
			}
			case 8: {

				Statistics();
				break;
			}
			case 9: {
				Insert();
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

int main()
{
	
	puts("-----------------欢迎使用学生时间管理系统-------------");

	printf("正在进入系统");

	for (int i = 0; i < 6; ++i)
	{
		printf(".");
		Sleep(500);
	}



	puts("");
	deal(); 
	system("pause");
	return 0;
}
