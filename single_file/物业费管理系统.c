#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number = 0;  //全局变量，记录人数 

struct msg {       //定义结构体来储存信息 
	char name[MIN];    //姓名
	char s_number[MIN];  // 身份证号 
	char sex[5];		//性别
	char phone[MIN];		//电话 
	int d_num;		//单元号
	int l_num;		//楼号 
	int h_num; 				//房号
	float h_area;			//平方数 
	float h_area_price;			//每平方物业价格 
	float true_price;			//应交价格 
	char nation[MAX];		//备注信息 
}msg[MAX];

void notice() {    //进入系统的主界面显示提息
	printf("************************************************************************************************\n");
	printf("\t\t\t\t  欢迎使用物业费管理系统\n");
	printf("\t****************************************************************************\n");
	printf("\t\t\t\t     1.新用户信息添加。\n");
	printf("\t\t\t\t     2.修改住户信息。\n");
	printf("\t\t\t\t     3.删除住户信息。\n");
	printf("\t\t\t\t     4.应交物业费自动生成。\n");
	printf("\t\t\t\t     5.缴费功能。\n");
	printf("\t\t\t\t     6.统计。\n");
	printf("\t\t\t\t     7.退出系统。\n");
    printf("*************************************************************************************************\n");
	printf("\t\t\t\t     选择对应编号：(1-7)");
}

void init_msg()   //信息添加函数 
{FILE * fp;
int z;
int i,t,flag=1;
if((fp=fopen("date","wb"))==NULL)
{
	printf("不能打开文件\n");
}
	
	system("cls");
	puts("***************>>>>>>当前模式为信息录入状态<<<<<<*************");
	printf("请输入姓名:");
	scanf("%s",msg[i_number].name);
	for(t=0;t<i_number;t++)
	{
		if(strcmp(msg[t].name,msg[i_number].name) == 0)
		{
			printf("该成员已存在!!!\n");
			flag = 0;
			break;
		}
	}
	if(flag)
	{
		printf("请输入性别:");
		scanf("%s",msg[i_number].sex);
		printf("请输入身份证号:");
		scanf("%s",msg[i_number].s_number);
		printf("请输入联系电话:");
		scanf("%s",msg[i_number].phone);
		printf("请输入楼号:");
		scanf("%d",&msg[i_number].l_num);
		printf("请输入单元号:");
		scanf("%d",&msg[i_number].d_num);
		printf("请输入房号:");
		scanf("%d",&msg[i_number].h_num);
		printf("请输入平米数:");
		scanf("%f",&msg[i_number].h_area);
		printf("请输入每平米价格:");
		scanf("%f",&msg[i_number].h_area_price);
		msg[i_number].true_price = msg[i_number].h_area_price*msg[i_number].h_area;
		printf("请输入备注(无输入无):");
		scanf("%s",msg[i_number].nation);
		fwrite(&msg[i_number],sizeof(struct msg),1,fp);
		fclose(fp);
		i_number++;
	}
	puts("录入成功"); 
}
 
void great_fee()  //自动生成订单 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为用户缴费浏览状态<<<<<<*************");
	puts("姓名\t性别\t身份证号\t\t联系电话\t楼号\t单元号\t房号\t平米数\t每平米物业费\t应交物业费");
	for(i=0;i<i_number;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%s\t%s\t%s\t%s\t%d\t%d\t%d\t%.2f\t%.2f\t%.2f\n",msg[i].name,msg[i].sex,
		msg[i].s_number,msg[i].phone,msg[i].l_num,msg[i].d_num,msg[i].h_num,msg[i].h_area,
		msg[i].h_area_price,msg[i].true_price);
		printf("备注：%s\n\n",msg[i].nation); 
	}	
}

void delet()  //信息删除 
{
	int i,t,flag = 0;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息删除状态<<<<<<*************");
	printf("请输入删除的用户姓名：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].name) == 0)
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
		puts("未找到该用户信息");
	}
}

void pay_fee()	//缴费
{
	int i,flag = 0,cnt;
	float num;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为缴费状态<<<<<<*************");
	printf("请输入姓名：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].name) == 0)
		{
			printf("你当前应交的物业费为：%.2f元。\n是否缴费(1.是 0.否):\n",msg[i].true_price);
			scanf("%d",&cnt);
			if(!cnt) break;
			else
			{
				printf("请输入缴纳金额数：");
				scanf("%f",&num);
				msg[i].true_price-=num;
				puts("缴费成功！");
			}
			flag =1;
			break;
		}
	}
	if(!flag)
	{
		puts("未找到该用户信息");
	}
}

void tongji()	//统计 
{
	int i,t,cnt=0,num[MAX]={0},flag=0;
	struct msg temp,flash[MAX];			//临时储存结构体 
	system("cls");
	puts("***************>>>>>>当前模式为信息统计状态<<<<<<*************\n\n");
	for(i=0;i<i_number;i++)
	{
		num[cnt]=msg[i].l_num;
		cnt++;
		for(t=0;t<cnt;t++)			//判断单元楼数量 
		{
			if(num[t]==num[cnt])
			{
				cnt--;
				break;	
			} 
			else break;	
		}
	}
	for(t=0;t<cnt;t++)					//遍历查找对应单元喽信息 
	{
		flag = 0; 
		for(i=0;i<i_number;i++)
		{
			if(msg[i].l_num==num[t])
			{
				flash[flag]=msg[i];
				flag++;	
			}	
		}	
		for(i=1;i<flag;i++)	
		{
			for(t=0; t<flag-1; t++) 
			{
				if(flash[t+1].true_price>flash[t].true_price)   //由高到低 
				{  
					temp=flash[t+1];
					flash[t+1]=flash[t];
					flash[t]=temp;
				}
			}
		}
		printf("当前楼号：%d\n",num[t]);
		for(i=0;i<flag;i++) printf("用户名：%s 联系电话：%s 房号：%d 拖欠金额：%.2f\n",flash[i].name,
		flash[i].phone,flash[i].h_num,flash[i].true_price);
		puts("\n"); 
	} 
} 

void change()		//修改
{
	int i,flag = 0,cnt;
	char name[MIN]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息修改状态<<<<<<*************");	
	printf("请输入查找的用户姓名：");
	scanf("%s",name);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(name,msg[i].name) == 0)
		{
			flag = 1;
			while(1)
			{
				printf("请输入你要修改的选项：1.电话 2.楼号 3.单元号 4.房号 5.平米数 6.每平米物业价格 0.退出\n");
				scanf("%d",&cnt);
				if(!cnt) break;
				else if(cnt==1)
				{
					printf("请输入修改后的电话:");
					scanf("%s",msg[i].phone);
				}
				else if(cnt==2)
				{
					printf("请输入修改后的楼号:");
					scanf("%d",&msg[i].l_num);
				}
				else if(cnt==3)
				{
					printf("请输入修改后的单元号:");
					scanf("%d",&msg[i].d_num);
				}
				else if(cnt==4)
				{
					printf("请输入修改后的房号:");
					scanf("%d",&msg[i].h_num);
				}
				else if(cnt==5)
				{
					printf("请输入修改后的平米数:");
					scanf("%f",&msg[i].h_area);
					msg[i].true_price = msg[i].h_area_price*msg[i].h_area;
				}
				else if(cnt==6)
				{
					printf("请输入修改后的每平米物业价格:");
					scanf("%f",&msg[i].h_area_price);
					msg[i].true_price = msg[i].h_area_price*msg[i].h_area;
				}
				else puts("输入有误！");
			}
		}
	}
	if(!flag)
	{
		puts("未找到该用户信息");
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
			change();		//修改
		}
		else if(cnt==3)
		{
			delet();  //信息删除
		}
		else if(cnt==4)
		{
			great_fee();  //自动生成订单
		}
		else if(cnt==5)
		{
			pay_fee();	//缴费
		}
		else if(cnt==6)
		{
			tongji();	//统计 	
		}
		else puts("输入有误！"); 
	}
	return 0;
}


