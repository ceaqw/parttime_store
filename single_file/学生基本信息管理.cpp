#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char s_number[MIN];  // 职工号 
	char name[MIN];    //姓名 
	int age;		//年龄	
	char sex[10];		//性别
	char brith[MIN];		//出生年龄
	char address[MIN];		//地址
	char phone[MIN];		//电话
	char email[MIN];		//E-mail
	char clas[MIN];		//所选科目 
	int point;		//分数 
}msg[MAX];

msg[0] = {1,1,1,1,1,1,1,1,1,1};
void notice() {    //进入系统的主界面显示提息
	printf("************************************************************************************************\n");
	printf("\t\t\t\t  欢迎使用学生信息管理系统\n");
	printf("\t****************************************************************************\n");
	printf("\t\t\t\t     1.录入学生信息。\n");
	printf("\t\t\t\t     2.学生信息浏览。\n");
	printf("\t\t\t\t     3.查询学生信息。\n");
	printf("\t\t\t\t     4.统计。\n");
	printf("\t\t\t\t     5.删除学生信息。\n");
	printf("\t\t\t\t     6.修改学生信息。\n");
	printf("\t\t\t\t     7.退出学生信息系统。\n");
    printf("*************************************************************************************************\n");
	printf("\t\t\t\t     选择对应编号：(1-7)");

}

void init_msg()   //信息添加函数 
{
	int i,n,t,flag=1;
	system("cls");
	puts("***************>>>>>>当前模式为信息录入状态<<<<<<*************");
	printf("请输入录入学生数量：");
	scanf("%d",&n);
	for(i=0;i<n;i++)   //遍历输入职工信息，存入相应的便量中 
	{
		flag=1;
		printf("第%d位：\n",i+1);
		printf("请输入学号:");
		scanf("%s",msg[i_number].s_number);
		for(t=0;t<i_number;t++)
		{
			if(strcmp(msg[t].s_number,msg[i_number].s_number) == 0)
			{
				printf("该成员已存在!!!\n");
				msg[i_number] = msg[t];
				printf("当前已有科目：%s,请输入其他科目:",msg[t].clas);
				scanf("%s",msg[i_number].clas);
				printf("请输入对应成绩:");
				scanf("%d",&msg[i_number].point);
				i_number++;
				flag = 0;
				break;
			}
		}
		if(flag)
		{
			printf("请输入姓名:");
			scanf("%s",msg[i_number].name);
			printf("请输入年龄:");
			scanf("%d",&msg[i_number].age);
			printf("请输入性别:");
			scanf("%s",msg[i_number].sex);
			printf("请输入出生年月:");
			scanf("%s",msg[i_number].brith);
			printf("请输入地址:");
			scanf("%s",msg[i_number].address);
			printf("请输入电话:");
			scanf("%s",msg[i_number].phone);
			printf("请输入email:");
			scanf("%s",msg[i_number].email);
			printf("请输入所修科目:");
			scanf("%s",msg[i_number].clas);
			printf("请输入对应成绩:");
			scanf("%d",&msg[i_number].point);
			i_number++;
		}
		
	} 
	puts("录入成功"); 
}
 
void print()  //信息浏览函数 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为信息浏览状态<<<<<<*************");
	puts("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\temail\t所修科目\t分数");
	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n",msg[i].s_number,msg[i].name,
		msg[i].age,msg[i].sex,msg[i].brith,msg[i].address,msg[i].phone,msg[i].email,
		msg[i].clas,msg[i].point);
	 } 
}

void delet()  //信息删除 
{
	int i,t,flag = 0;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息删除状态<<<<<<*************");
	printf("请输入删除的学号：");
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
		}
	}
	if(flag)
	{
		puts("删除成功");	
	}
	else
	{
		puts("未找到该学生信息");
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
		printf("查找类型：1.学号 2.姓名 3.科目 0.退出\n");
		scanf("%d",&cnt);
		if(cnt==1)
		{
			printf("请输入查找的学号：");
			scanf("%s",name);
			puts("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\temail\t所修科目\t分数");
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n",msg[i].s_number,msg[i].name,
					msg[i].age,msg[i].sex,msg[i].brith,msg[i].address,msg[i].phone,msg[i].email,
					msg[i].clas,msg[i].point);
					flag =1;
				}
			}
		}
		else if(cnt==2)
		{
			printf("请输入查找的姓名：");
			scanf("%s",name);
			puts("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\temail\t所修科目\t分数");
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].name) == 0)
				{
					printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n",msg[i].s_number,msg[i].name,
					msg[i].age,msg[i].sex,msg[i].brith,msg[i].address,msg[i].phone,msg[i].email,
					msg[i].clas,msg[i].point);
					flag =1;
				}
			}
		}
		else if(cnt==3)
		{
			printf("请输入查找的所修科目：");
			scanf("%s",name);
			puts("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\temail\t所修科目\t分数");
			for(i=0;i<i_number;i++)
			{
				if(strcmp(name,msg[i].s_number) == 0)
				{
					printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%d\n",msg[i].s_number,msg[i].name,
					msg[i].age,msg[i].sex,msg[i].brith,msg[i].address,msg[i].phone,msg[i].email,
					msg[i].clas,msg[i].point);
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
		puts("未找到该学生信息");
	}
}

void tongji()	//工资总分统计 
{
	int i,flag=0;
	float cnt,count=0,num=0;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息统计状态<<<<<<*************");
	printf("请选择功能(1.查询某同学所有科目成绩平均分 2.按科目名称统计班级平均分及格率)：");
	scanf("%f",&cnt);
	if(cnt==1)
	{
		cnt=0;
		printf("请输入统计的学生姓名：");
		scanf("%s",name);
		for(i=0;i<i_number;i++)
		{
			if(strcmp(name,msg[i].name) == 0)
			{
				cnt+=msg[i].point;
				count++;
				flag =1;
			}
		}
		printf("%s同学共选修了%.0f门科目，平均成绩为：%.2f\n",name,count,cnt/count);
	}
	else if(cnt==2)
	{
		printf("请输入统计的班级名称：");
		scanf("%s",name);
		for(i=0;i<i_number;i++)
		{
			if(strcmp(name,msg[i].clas) == 0)
			{
				cnt+=msg[i].point;
				count++;
				if(msg[i].point>=60) num++;
				flag =1;
			}
		}
		printf("%s科目班级学生总分为：%.0f，平均成绩为：%.2f，及格率：%.2f\n",name,cnt,cnt/count,num/count);
	}
	if(!flag)
	{
		puts("未找到该学生信息");
	}
} 

void change()		//修改
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息修改状态<<<<<<*************");	
	printf("请输入查找的学号：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].s_number) == 0)
		{
			flag = 1;
			while(1)
			{
				printf("请输入你要修改的选项：1.姓名 2.年龄 3.出生年月 4.地址 5.电话 6.email 0.退出\n");
				scanf("%d",&cnt);
				if(!cnt) break;
				else if(cnt==1)
				{
					printf("请输入修改后的姓名:");
					scanf("%s",msg[i].name);
				}
				else if(cnt==2)
				{
					printf("请输入修改后的年龄:");
					scanf("%d",&msg[i].age);
				}
				else if(cnt==3)
				{
					printf("请输入修改后的出生年月:");
					scanf("%s",msg[i].brith);
				}
				else if(cnt==4)
				{
					printf("请输入修改后的地址:");
					scanf("%s",msg[i].address);
				}
				else if(cnt==5)
				{
					printf("请输入修改后的电话:");
					scanf("%s",msg[i].phone);
				}
				else if(cnt==6)
				{
					printf("请输入修改后的email:");
					scanf("%s",msg[i].email);
				}
				else puts("输入有误！");
			}
		}
	}
	if(!flag)
	{
		puts("未找到该学生信息");
	}
} 

int main() {
 	int cnt;
 	while(1)
 	{
 		notice();
		scanf("%d",&cnt);	
		system("cls");
		if(cnt==7) break;
		else if(cnt==1)
		{
			init_msg();   //信息添加函数 
		}
		else if(cnt==2)
		{
			print();    	//信息浏览函数 
		}
		else if(cnt==3)
		{
			find();    			//信息查找
		}
		else if(cnt==4)
		{
			tongji();			//工资总分统计
		}
		else if(cnt==5)
		{
			delet();		//信息删除 
		}
		else if(cnt==6)
		{
			change();		//修改	
		}
		else puts("输入有误！"); 
	}
	return 0;
}


