#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int i_number = 0;		//记录学生人数 

struct admin		//管理员数据结构
{
  char  name[10];  //姓名
  char  password[10]; // 密码
  char  ID[5] ;  //工号
}Admin;


struct student		//学生数据结构：
{
  char  name[10];  //姓名
  char  password[10]; // 密码
  char  ID[5] ;  //学号
  int   time; //累计预约时间  
}Student[40];

int student_see();		//学生信息处理

struct lab
{ 
	int   No;         //编号
	char   name[10];     //姓名
	char   number[10];   //学号 
	int   time_case[9];           //时间段状态
}Lab[7][40];		//40电脑，一周使用情况储存 

void meau()		//菜单显示 
{
	puts(">>>输入下列选项对应的数字选择功能<<<");
	puts("1.管理员查看电脑一周使用情况");
	puts("2.学生查看累计上机时间");
	puts("0.退出"); 
}

void set_init()		//计算机初始化 
{
	int i,j,k;
	for(i = 0;i<7;i++)
	{
		for(j=0;j<40;j++)
		{
			for(k =0;k<9;k++)
			{
				Lab[i][j].time_case[k] = 1;		//初始化位空闲	
			}	
		}	
	}
	
	for(i = 0;i<7;i++)
	{
		for(j=0;j<40;j++)
		{
			Lab[i][j].No = j+1;		//初始化电脑编号		
		}	
	}	
}
 
int student_init()		//学生端预初始化 
{
	int i,flag = 0;
	char key[10]; 
	printf("请输入姓名：");
	scanf("%s",key);
	for(i = 0;i<40;i++)
	{
		if(strcmp(key,Student[i].name) == 0)		//检验是否存在此用户 
		{
			printf("请输入登陆密码：");
			scanf("%s",key);
			if(strcmp(key,Student[i].password) == 0)		//检测密码 
			{
				printf("姓名\t学号\t累计时间\n");
				printf("%s\t%s\t%d\n",Student[i].name,Student[i].ID,Student[i].time);	
			}
		}
		else
		{
			flag = 1;	
		}	
	}
	
	if(flag == 1)
	{
		int cnt;
		printf("是否预约 1.是 2.否\n"); 
		scanf("%d",&cnt);
		if(cnt == 1)
		{
			printf("请输入学号 姓名 密码 预约时间（单位：小时）\n");
			scanf("%s %s %s %d",Student[i_number].ID,Student[i_number].name,Student[i_number].password,&Student[i_number].time);
			i_number++; 
			student_see();
			return 1;
		}
		else if(cnt == 2)
		{
			return 0;
		 } 
	}
	
	return 1;	
} 

int admin_init()  //管理员信息 返回0不通过，返回1通过 ，返回2直接退出 
{
	char key[10];		//密码储存。限9位 
	strcpy(Admin.password,"0");

/*检测是否存在管理员*/
	if(strcmp(Admin.password,"0") == 0)
	{
		printf("请先注册管理员！！！\n");
		printf("请输入管理员姓名："); 
		scanf("%s",Admin.name);
		printf("请输入管理员ID：");
		scanf("%s",Admin.ID);
		printf("请输入管理员登陆密码：");
		scanf("%s",Admin.password);
		printf("再次输入登陆设置密码：");
		scanf("%s",key); 
		while(strcmp(key,Admin.password) != 0)
		{
			printf("输入有误！！！\n");
			printf("再次输入登陆设置密码：");
			scanf("%s",key);
		}
	}
	
	
	printf("请输入登陆用户名：");
	scanf("%s",key);
	if(strcmp(key,Admin.name) == 0)
	{
		printf("请输入登陆密码："); 
		scanf("%s",key);
		if(strcmp(key,Admin.password) == 0)
		{
			return 1;	
		}	
		else
		{
			printf("密码输入有误！！！\n");
			return 0;
		 } 
	 } 
	 else
	 {
	 	printf("不存在此管理员！！！\n");
		return 0; 
	 }	
}

int admin_see()		//管理员信息查看
{
	int i,flag = 0,cnt,num[4];
	printf("1.维护电脑 2.查看实验室使用情况 3.统计某天的使用情况 0.退出\n");
	scanf("%d",&cnt);
	if(cnt == 0)
	{
		return 0;
	}
	else if(cnt == 1)
	{
		printf("请输入所需维护的电脑编号：");
		scanf("%d",&num[0]);
		printf("请输入需要修改状态的时间段（星期1-7 时间段8-16）:");
		scanf("%d %d",&num[1],&num[2]);
		num[2] -= 7; 
		printf("请输入该时间段的状态(1.空闲 2.使用中 3.故障)：\n");
		scanf("%d",&num[3]);
		for(i=0;i<40;i++)
		{
			if(Lab[num[1]-1][i].No == num[0]) 		//找到对应的电脑 
			{
				Lab[num[1]-1][i].time_case[num[2]-1] = num[3];  //修改对应电脑的状态 
				flag = 1;				
			}
		}
		if(flag == 1)
		{
			puts("修改成功");
		}
		else
		{
			puts("未找到！！！");
		}
		return 1;
	}
	else if(cnt == 2)	//看实验室试用情况 
	{

		printf("请输入查看的时间 星期1-7 时间段8-16）：");
		scanf("%d %d",&num[0],&num[1]);
		num[1] -= 7;
		for(i = 0;i<40;i++)
		{
			if(Lab[num[0]-1][i].time_case[num[1]-1] == 1)		//空闲 
			{
				printf("*");
			}
			else if(Lab[num[0]-1][i].time_case[num[1]-1] == 2)	//占用 
			{
				printf("#");
			}
			else if(Lab[num[0]-1][i].time_case[num[1]-1] == 3)		//故障 
			{
				printf("!");
			}
			
			if((i+1)%2 == 0)
			{
				printf(" ");
			} 
			if((i+1)%8 == 0)
			{
				printf("\n"); 
			} 
		}
		puts("\n");
		return 1;
	
	}
	else if(cnt == 3)		//保存某天信息 
	{
		FILE *fp;
		if((fp = fopen("count.txt","w")) == NULL)
		{
			fp = fopen("count.txt","w+");
		}
		int i,j,n;
		printf("请输入需要查看的天数（星期1-7）:");
		scanf("%d",&n);
		for(i = 0;i<9;i++)
		{
			fprintf(fp,"第%d个时间段使用情况\n",i+1);
			for(j=0;j<40;j++)
			{
				if(Lab[n][j].time_case[i] == 1)		//空闲 
				{
					fprintf(fp,"*");
				}
				else if(Lab[n][j].time_case[i] == 2)	//占用 
				{
					fprintf(fp,"#");
				}
				else if(Lab[n][j].time_case[i] == 3)		//故障 
				{
					fprintf(fp,"!");
				}
				
				if((i+1)%2 == 0)
				{
					fprintf(fp," ");
				} 
				if((i+1)%8 == 0)
				{
					fprintf(fp,"\n"); 
				} 
			}
			fputs(fp,"\n");
		}
		fclose(fp);
		 return 1;
	}	
} 

int student_see()		//学生信息处理 
{
	int i,j,num[4],cnt,flag = 0;
	printf("1.预约时间 2.取消预约 3.报告故障 4.查看实验室预约情况 0.退出\n");
	scanf("%d",&cnt);
	if(cnt == 0)
	{
		return 0;
	}
	else if(cnt == 1)		//预约 
	{
		printf("请输入预约选项：\n");
		printf("请输入机号：");
		scanf("%d",&num[0]);
		printf("请输入时间（周几1-7 开始时间段8-16 时间段数8-16 ）:");
		scanf("%d %d %d",&num[1],&num[2],&num[3]);
		num[2] -= 7;
		num[3] -= 7;
		for(j=0;j<40;j++)		
		{
			if(Lab[num[1]-1][j].No == num[0])
			{
				for(i=0;i<num[3];i++)
				{
					if(Lab[num[1]-1][j].time_case[i] != 1)		//检测该机为是否空闲
					{
						flag = 50;
						break; 
					} 
					else
					{
						flag = j;
					}
				}
			}
		}
		
		if(flag == 50)
		{
			puts("预约失败！！！\n");
		}
		else
		{
			printf("请输入学号：");
			scanf("%s",Lab[num[1]-1][flag].number);
			printf("请输入姓名：");
			scanf("%s",Lab[num[1]-1][flag].name);
			for(i=0;i<num[3];i++)
			{
				Lab[num[1]-1][flag].time_case[i] = 2;		//标记被占用 
			}	
		}
		return 1;
	}
	else if(cnt == 2)	//取消 
	{
		printf("请输入取消机号，周几及取消时间段个数数字空格隔开：");
		scanf("%d %d",num[0],num[2],&num[1]);
		for(j=0;j<40;j++)		
		{
			if(Lab[num[1]-1][j].No == num[0])
			{
				for(i=0;i<num[1];i++)
				{
					Lab[num[2]-1][j].time_case[i] = 1;	//标记为空闲 
				}
			}
		}
		return 1;
	}
	else if(cnt == 3)	//报错 
	{
		char key[11];
		char error_msg[1000];
		FILE *fp;
		if((fp = fopen("error.txt","w")) == NULL)
		{
			fp = fopen("error.txt","w+");
		}
		printf("请输入故障机号及时间（时间格式2019/04/19）空格隔开：");
		scanf("%d %s",&num[0],key);
		printf("请输入故障问题：");
		scanf("%s",error_msg);
		fprintf(fp,"%d\t%s\n",num[0],key);
		fprintf(fp,"%s",error_msg);
		fclose(fp);
		return 1;
	}
	else if(cnt == 4)	//查看预约情况 
	{
		printf("请输入查看的时间 星期1-7 时间段8-16）：");
		scanf("%d %d",&num[0],&num[1]);
		num[1] -= 7;
		for(i = 0;i<40;i++)
		{
			if(Lab[num[0]-1][i].time_case[num[1]-1] == 1)		//空闲 
			{
				printf("*");
			}
			else if(Lab[num[0]-1][i].time_case[num[1]-1] == 2)	//占用 
			{
				printf("#");
			}
			else if(Lab[num[0]-1][i].time_case[num[1]-1] == 3)		//故障 
			{
				printf("!");
			}
			
			if((i+1)%2 == 0)
			{
				printf(" ");
			} 
			if((i+1)%8 == 0)
			{
				printf("\n"); 
			} 
		}
		puts("\n");
		return 1;
	}
}

void main()		//主函数 
{
	int i,cnt,flag = 0;
	while(1)
	{
		set_init(); 
		meau();
		printf("请选择：");
		scanf("%d",&cnt);
		switch(cnt)
		{
			case 0:{
				puts("再见！！！"); 
				flag = 1;
				break;
			}
			case 1:{		//管理员 
				while(!flag)		//登陆认证 
				{
					flag =  admin_init();
				}
				flag = 1;
				while(flag)
				{
					flag = admin_see();
				}
				break;
			} 
			case 2:{		//学生 
				flag = student_init();
				if(!flag)
				{
					break;
				}
				else
				{
					while(flag)
					{
						student_see();
					}
				}
				break;
			} 
			default :{
				puts("输入有误！！！"); 
				break;
			} 
		}
		if(flag)
		{
			break;
		}
	}
	system("pause");
	
} 
