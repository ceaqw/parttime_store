#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000
#define MIN 20

int i_number[6] = {0}; 

typedef struct Msg{
	char name[MIN];		//姓名 
	char password[MIN];		//密码
	int flag;
}msg;

typedef struct ListNode
{
    char name[MIN];		//姓名 
	char password[MIN];		//密码
	int flag;
    struct ListNode* next;    //指向结构体的指针
}ListNode;

struct Earth_msg{		//块地信息 
	char earth_number[MIN];		//块地编号
	char earth_sort[MIN];		//土壤种类
	char earth_form[MIN];		//土壤形状
	int earth_area;		//土壤面积
	char earth_location[MIN];		//位置 
}earth_msg[MAX]; 

struct Crop_msg{		//作物信息 
	char crop_number[MIN];		//作物编号 
	char crop_name[MIN];		//作物名称 
	int crop_plant_time;		//作物种植时间 
	int crop_gain_time;				//作物收货时间 
	char crop_character[MIN];		//作物特性 
}crop_msg[MAX]; 

struct Plant_msg{			//种植信息 
	char plant_number[MIN];		//种植记录编号 
	char plant_earth_number[MIN];		//作物地块编号 
	char plant_crop_number[MIN];		//作物种类编号 
	int plant_num;		//数量 
	char plant_action_time[MIN];		//实施时间 
	char plant_action_peo[MIN];		//实施人 
	char plant_check_peo[MIN];		//检查人 
}plant_msg[MAX];

struct Run_msg{			// 经营信息 
	char run_number[MIN];			//经营记录编号 
	char run_sort[MIN];			//经营种类 
	char run_action_time[MIN];		//实施时间 
	char run_action_peo[MIN];		//实施人 
	char run_check_peo[MIN];		//监察人 
}run_msg[MAX]; 

struct Pick_msg{			//采摘 
	char pick_number[MIN];		//采摘编号 
	char pick_earth_number[MIN];		//地块编号 
	char pick_crop_number[MIN];		//作物类型 
	int pick_num;		//数量 
	char pick_action_time[MIN];			//实施时间 
	char pick_action_peo[MIN];		//实施人 
	char pick_check_peo[MIN];		//检查人 
}pick_msg[MAX];		

struct Finance_msg{		//财务 
	int finance_gain;
	int finance_cast; 
}finance_msg; 

//传二级指针,ppHead是一个二级指针，是第一个指向第一个节点的指针的地址
int Buy_node(ListNode* pHead,msg x)
{
 //   ListNode *node = (ListNode*)malloc(sizeof(ListNode));
 	ListNode *node;
	node = pHead; 
    strcpy(node->name,x.name);
    strcpy(node->password,x.password);
    node->flag = x.flag;
    node->next = NULL;// 否则会变成随机值
    return x.flag;
}

//在链表中查找数据(x)，初步想法是遍历一遍链表，对比查找，找到则返回节点指针,没找到返回NULL
int Find(ListNode* pHead, char *x,int n)
{
	int i;
    ListNode *cur = pHead;
    if(n==1)
    {
    	for(i=0;i<i_number[0];i++)
	    {
	        if (strcmp(x,cur->name)==0)
	        {
	            return cur->flag;
	        }
	        else
	        {
	        	cur = cur->next;	
			}
	    }
	}
    else if(n==2)
    {
    	for(i=0;i<i_number[0];i++)
	    {
	        if (strcmp(x,cur->password)==0)
	        {
	            return cur->flag;
	        }
	        else
	        {
	        	 cur = cur->next;	
			}
	    }
	}
    return 0;
}


int login(ListNode* pHead)		//登陆,返回1登陆成功0失败 
{
	int num;
	char str[2][MIN],cnt;
	int i,flag=0;
	printf("请输入用户名：");
	scanf("%s",str[0]); 
	if(num = Find(pHead,str[0],1))
	{
		flag=1;
		printf("请输入密码：");
		scanf("%s",str[1]);
		while(!Find(pHead,str[1],2))
		{
			printf("密码输入有误！！！是否重新输入(1.是 0.否)\n");
			cnt = getch();
			if(cnt=='0')
			{
				exit(0);
			}
			printf("请输入：");
			scanf("%s",str[1]);
		}
	}
	if(flag)
	{
		return num;
	}
} 
 
int signin(ListNode* pHead)			//注册
{
	msg num;
	printf("请输入用户名：");
	scanf("%s",num.name);	
	printf("请输入登陆密码：");
	scanf("%s",num.password);
	printf("请输入身份类型(1.管理员 2.操作员 3.财务人员)：");
	scanf("%d",&num.flag);
	i_number[0]++;
	puts("注册成功！");
	return Buy_node(pHead,num);
} 

int DelItem(ListNode *L,char *str)
//在单链表中删除数据元素
{
    int i = 1;
    ListNode *p,*q;
    p = L;
    if(L->next==NULL) /*L为空表，无结点可删除*/
    {
        printf("The linklist is empty!\n");
        return 0;
    }
    while(p->next!=NULL && strcmp(p->name,str)!=0)
    {
        p = p->next;
    }

    q = p->next;
    p->next = p->next->next;
    free(q);
    return 1;

}


void Administrator_set(ListNode* pHead)		//管理员操作界面
{
	ListNode* MSG;
	MSG = pHead;
	int i,t;
	char cnt,str[MIN];
	while(1)
	{
		printf("请选择功能(1.增加用户 2.删除用户 3.修改用户 4.查找用户 0.退出):\n");
		cnt = getch();
		if(cnt=='1')
		{
			signin(pHead);				
		}
		else if(cnt=='2')
		{
			printf("请输入你要删除的用户名:");
			scanf("%s",str);
			DelItem(pHead,str);
		}
		else if(cnt=='3')
		{
			printf("请输入你要修改的用户名:");
			scanf("%s",str);	
			for(i=0;i<i_number[0];i++)
			{
				if(strcmp(str,MSG->name)==0)
				{
					while(1)
					{
						printf("请输入你要修改的信息(1.名称 2.密码 3.标识 0.退出)：\n");
						cnt = getch();
						if(cnt=='0')
						{
							break;
						}
						else if(cnt=='1')
						{
							printf("请输入修改后的名称：");
							scanf("%s",MSG->name);								
						}
						else if(cnt=='2')
						{
							printf("请输入修改后的密码：");
							scanf("%s",MSG->password);	
						}
						else if(cnt=='3')
						{
							printf("请输入修改后的标志：");
							scanf("%d",&MSG->flag);
						} 
					}
				}
				MSG = MSG->next;	
			}
		}
		else if(cnt=='4')
		{
			printf("请输入你要查找的用户名：");
			scanf("%s",str);
			for(i=0;i<i_number[0];i++)
			{
				if(strcmp(str,MSG->name)==0)
				{
					printf("用户\t密码\t身份\n");
					printf("%s\t%s\t",MSG->name,MSG->password);
					if(MSG->flag==1)
					{
						printf("管理员\n");	
					}
					else if(MSG->flag==2)
					{
						printf("操作员\n");	
					}
					else if(MSG->flag==3)
					{
						printf("财务员\n");	
					}	
				}
				MSG = MSG->next;	
			}	
		} 
		else if(cnt=='0')
		{
			break;	
		}	
	}	
} 

void operate_set()			//操作员
{
	int i,t,flag=1;
	char cnt,str[MIN];
	while(1)
	{
		printf("请选择功能(1.块地管理 2.作物管理 3.种植管理 4.采摘 0.退出):\n");
		cnt = getch();
		if(cnt=='0')
		{
			break;	
		}
		else if(cnt=='1')
		{
			while(1)
			{
				printf("请选择功能项(1.增 2.浏览 3.删 4.修改 5.查询 0.退出)：\n");
				cnt = getch();
				if(cnt=='0')
				{
					break;
				}
				else if(cnt=='1')
				{
					printf("请输入地块编号：");
					scanf("%s",earth_msg[i_number[1]].earth_number);
					for(i=0;i<i_number[1];i++)
					{
						if(!strcmp(earth_msg[i_number[1]].earth_number,earth_msg[i].earth_number))		//检测已存在 
						{
							flag=0;
						}
					}
					if(flag)
					{
						printf("请输入土壤类型：");
						scanf("%s",earth_msg[i_number[1]].earth_sort);
						printf("请输入形状：");
						scanf("%s",earth_msg[i_number[1]].earth_form);
						printf("请输入面积：");
						scanf("%d",&earth_msg[i_number[1]].earth_area);
						printf("请输入位置：");
						scanf("%s",earth_msg[i_number[1]].earth_location);
						i_number[1]++;
						puts("录入成功");
					}
					else
					{
						puts("已存在！！");
						flag=1;
					}
				}
				else if(cnt=='2')
				{
					printf("地块编号\t土壤种类\t形状\t面积\t位置\n");
					for(i=0;i<i_number[1];i++)
					{
						printf("%s\t%s\t%s\t%d\t%s\n",earth_msg[i].earth_number,earth_msg[i].earth_sort,earth_msg[i].earth_form,
						earth_msg[i].earth_area,earth_msg[i].earth_location);
					}
				}
				else if(cnt=='3')
				{
					printf("请输入需要删除的作物编号：");
					scanf("%s",str);
					for(i=0;i<i_number[1];i++)
					{
						if(strcmp(str,earth_msg[i].earth_number)==0)
						{
							for(t=i;t<i_number[1];t++)
							{
								earth_msg[t] = earth_msg[t+1];
							}
							i_number[1]--; 
						}
					}
				}
				else if(cnt=='4')
				{
					printf("请输入你要修改的编号：");
					scanf("%s",str);
					for(i=0;i<i_number[1];i++)
					{
						if(strcmp(str,earth_msg[i].earth_number)==0)
						{
							while(1)
							{
								printf("请输入所需修改项(1.土壤种类 2.形状 3.面积 4.位置 0.退出)："); 
								cnt = getch();
								if(cnt=='0')
								{
									break;				
								}
								else if(cnt=='1')
								{
									printf("请输入修改后的信息：");
									scanf("%s",earth_msg[i].earth_sort);
								}
								else if(cnt=='2')
								{
									printf("请输入修改后的信息：");
									scanf("%s",earth_msg[i].earth_form);
								}
								else if(cnt=='3')
								{
									printf("请输入修改后的信息：");
									scanf("%s",earth_msg[i].earth_area);
								}
								else if(cnt=='4')
								{
									printf("请输入修改后的信息：");
									scanf("%s",earth_msg[i].earth_location);
								}
							}	
						}
					}
				}
				else if(cnt=='5')
				{
					printf("请输入你要查询的作物编号：");
					scanf("%s",str);
					printf("地块编号\t土壤种类\t形状\t面积\t位置\n");
					for(i=0;i<i_number[1];i++)
					{
						if(strcmp(str,earth_msg[i].earth_number)==0)
						{
							printf("%s\t%s\t%s\t%d\t%s\n",earth_msg[i].earth_number,earth_msg[i].earth_sort,earth_msg[i].earth_form,
							earth_msg[i].earth_area,earth_msg[i].earth_location);
						}
					}
				}
			}	
		}
		else if(cnt=='2')
		{
			while(1)
			{
				printf("请选择功能项(1.增 2.浏览 3.删 4.修改 5.查询 0.退出)：\n");
				cnt = getch();
				if(cnt=='0')
				{
					break;
				}
				else if(cnt=='1')
				{
					printf("请输入作物种类编号：");
					scanf("%s",crop_msg[i_number[2]].crop_number);
					for(i=0;i<i_number[2];i++)
					{
						if(!strcmp(crop_msg[i_number[2]].crop_number,crop_msg[i].crop_number))		//检测已存在 
						{
							flag=0;
						}
					}
					if(flag)
					{
						printf("请输入作物名称：");
						scanf("%s",crop_msg[i_number[2]].crop_name);
						printf("请输入种植月份：");
						scanf("%d",&crop_msg[i_number[2]].crop_plant_time);
						printf("请输入收获月份：");
						scanf("%d",&crop_msg[i_number[2]].crop_gain_time);
						printf("请输入作物特性(喜光、避光、嬉水、干旱)：");
						scanf("%s",crop_msg[i_number[2]].crop_character);
						i_number[2]++;
						puts("录入成功");
					}
					else
					{
						puts("已存在！！");
						flag=1;
					}
				}
				else if(cnt=='2')
				{
					printf("作物种类编号\t作物名称\t种植月份\t收获月份\t作物特性\n");
					for(i=0;i<i_number[2];i++)
					{
						printf("%s\t%s\t%d\t%d\t%s\n",crop_msg[i].crop_number,crop_msg[i].crop_name,crop_msg[i].crop_plant_time,
						crop_msg[i].crop_gain_time,crop_msg[i].crop_character);
					}
				}
				else if(cnt=='3')
				{
					printf("请输入需要删除的作物种类编号：");
					scanf("%s",str);
					for(i=0;i<i_number[2];i++)
					{
						if(strcmp(str,crop_msg[i].crop_number)==0)
						{
							for(t=i;t<i_number[2];t++)
							{
								crop_msg[t] = crop_msg[t+1];
							}
							i_number[2]--; 
						}
					}
				}
				else if(cnt=='4')
				{
					printf("请输入你要修改的编号：");
					scanf("%s",str);
					for(i=0;i<i_number[2];i++)
					{
						if(strcmp(str,crop_msg[i].crop_number)==0)
						{
							while(1)
							{
								printf("请输入所需修改项(1.作物名称 2.种植月份 3.收获月份 4.作物特性 0.退出)："); 
								cnt = getch();
								if(cnt=='0')
								{
									break;				
								}
								else if(cnt=='1')
								{
									printf("请输入修改后的信息：");
									scanf("%s",crop_msg[i].crop_name);
								}
								else if(cnt=='2')
								{
									printf("请输入修改后的信息：");
									scanf("%d",&crop_msg[i].crop_plant_time);
								}
								else if(cnt=='3')
								{
									printf("请输入修改后的信息：");
									scanf("%d",&crop_msg[i].crop_gain_time);
								}
								else if(cnt=='4')
								{
									printf("请输入修改后的信息(喜光、避光、嬉水、干旱)：");
									scanf("%s",crop_msg[i].crop_character);
								}
							}	
						}
					}
				}
				else if(cnt=='5')
				{
					printf("请输入你要查询的作物编号：");
					scanf("%s",str);
					printf("作物种类编号\t作物名称\t种植月份\t收获月份\t作物特性\n");
					for(i=0;i<i_number[2];i++)
					{
						if(strcmp(str,crop_msg[i].crop_number)==0)
						{
							printf("%s\t%s\t%s\t%d\t%s\n",crop_msg[i].crop_number,crop_msg[i].crop_name,crop_msg[i].crop_plant_time,
							crop_msg[i].crop_gain_time,crop_msg[i].crop_character);
						}
					}
				}
			}	
		}
		else if(cnt=='3')
		{
			while(1)
			{
				printf("请选择功能项(1.增 2.浏览 3.删 4.修改 5.查询 0.退出)：\n");
				cnt = getch();
				if(cnt=='0')
				{
					break;
				}
				else if(cnt=='1')
				{
					printf("请输入种植记录编号：");
					scanf("%s",plant_msg[i_number[3]].plant_number);
					for(i=0;i<i_number[3];i++)
					{
						if(!strcmp(plant_msg[i_number[3]].plant_number,plant_msg[i].plant_number))		//检测已存在 
						{
							flag=0;
						}
					}
					if(flag)
					{
						printf("请输入地块名称：");
						scanf("%s",plant_msg[i_number[3]].plant_earth_number);
						printf("请输入作物种植编号：");
						scanf("%s",plant_msg[i_number[3]].plant_crop_number);
						printf("请输入数量：");
						scanf("%d",&plant_msg[i_number[3]].plant_num);
						printf("请输入实施时间：");
						scanf("%s",plant_msg[i_number[3]].plant_action_time);
						printf("请输入实施人：");
						scanf("%s",plant_msg[i_number[3]].plant_action_peo);
						printf("请输入检查人：");
						scanf("%s",plant_msg[i_number[3]].plant_check_peo);
						i_number[3]++;
						puts("录入成功");
					}
					else
					{
						puts("已存在！！");
						flag=1;
					}
				}
				else if(cnt=='2')
				{
					printf("种植记录编号\t地块名称\t作物种植编号\t数量\t实施时间\t实施人\t检查人\n");
					for(i=0;i<i_number[3];i++)
					{
						printf("%s\t%s\t%s\t%d\t%s\t%s\t%s\n",plant_msg[i].plant_number,plant_msg[i].plant_earth_number,plant_msg[i].plant_crop_number,
						plant_msg[i].plant_num,plant_msg[i].plant_action_time,plant_msg[i].plant_action_peo,plant_msg[i].plant_check_peo);
					}
				}
				else if(cnt=='3')
				{
					printf("请输入需要删除的种植记录编号：");
					scanf("%s",str);
					for(i=0;i<i_number[3];i++)
					{
						if(strcmp(str,plant_msg[i].plant_number)==0)
						{
							for(t=i;t<i_number[3];t++)
							{
								plant_msg[t] = plant_msg[t+1];
							}
							i_number[3]--; 
						}
					}
				}
				else if(cnt=='4')
				{
					printf("请输入你要修改的种植记录编号：");
					scanf("%s",str);
					for(i=0;i<i_number[3];i++)
					{
						if(strcmp(str,plant_msg[i].plant_number)==0)
						{
							while(1)
							{
								printf("请输入所需修改项(1.地块名称 2.数量 3.实施时间 4.实施人 5.检查人 0.退出)："); 
								cnt = getch();
								if(cnt=='0')
								{
									break;				
								}
								else if(cnt=='1')
								{
									printf("请输入修改后的信息：");
									scanf("%s",plant_msg[i].plant_earth_number);
								}
								else if(cnt=='2')
								{
									printf("请输入修改后的信息：");
									scanf("%d",&plant_msg[i].plant_num);
								}
								else if(cnt=='3')
								{
									printf("请输入修改后的信息：");
									scanf("%s",plant_msg[i].plant_action_time);
								}
								else if(cnt=='4')
								{
									printf("请输入修改后的信息：");
									scanf("%s",plant_msg[i].plant_action_peo);
								}
								else if(cnt=='4')
								{
									printf("请输入修改后的信息：");
									scanf("%s",plant_msg[i].plant_check_peo);
								}
							}	
						}
					}
				}
				else if(cnt=='5')
				{
					printf("请输入你要查询的作物种植编号：");
					scanf("%s",str);
					printf("种植记录编号\t地块名称\t作物种植编号\t数量\t实施时间\t实施人\t检查人\n");
					for(i=0;i<i_number[3];i++)
					{
						if(strcmp(str,plant_msg[i].plant_number)==0)
						{
							printf("%s\t%s\t%s\t%d\t%s\t%s\t%s\n",plant_msg[i].plant_number,plant_msg[i].plant_earth_number,plant_msg[i].plant_crop_number,
							plant_msg[i].plant_num,plant_msg[i].plant_action_time,plant_msg[i].plant_action_peo,plant_msg[i].plant_check_peo);
						}
					}
				}
			}		
		}
		else if(cnt=='4')
		{
			while(1)
			{
				printf("请选择功能项(1.增 2.浏览 3.删 4.修改 5.查询 0.退出)：\n");
				cnt = getch();
				if(cnt=='0')
				{
					break;
				}
				else if(cnt=='1')
				{
					printf("请输入采摘记录编号：");
					scanf("%s",pick_msg[i_number[4]].pick_number);
					for(i=0;i<i_number[4];i++)
					{
						if(!strcmp(pick_msg[i_number[4]].pick_number,pick_msg[i].pick_number))		//检测已存在 
						{
							flag=0;
						}
					}
					if(flag)
					{
						printf("请输入采摘地块：");
						scanf("%s",pick_msg[i_number[4]].pick_earth_number);
						printf("请输入采摘作物种类：");
						scanf("%s",pick_msg[i_number[4]].pick_crop_number);
						printf("请输入数量：");
						scanf("%d",&pick_msg[i_number[4]].pick_num);
						printf("请输入实施时间：");
						scanf("%s",pick_msg[i_number[4]].pick_action_time);
						printf("请输入实施人：");
						scanf("%s",pick_msg[i_number[4]].pick_action_peo);
						printf("请输入检查人：");
						scanf("%s",pick_msg[i_number[4]].pick_check_peo);
						i_number[4]++;
						puts("录入成功");
					}
					else
					{
						puts("已存在！！");
						flag=1;
					}
				}
				else if(cnt=='2')
				{
					printf("采摘记录编号\t采摘地块\t采摘作物种类\t数量\t实施时间\t实施人\t检查人\n");
					for(i=0;i<i_number[4];i++)
					{
						printf("%s\t%s\t%s\t%d\t%s\t%s\t%s\n",pick_msg[i].pick_number,pick_msg[i].pick_earth_number,pick_msg[i].pick_crop_number,
						pick_msg[i].pick_num,pick_msg[i].pick_action_time,pick_msg[i].pick_action_peo,pick_msg[i].pick_check_peo);
					}
				}
				else if(cnt=='3')
				{
					printf("请输入需要删除的采摘记录编号：");
					scanf("%s",str);
					for(i=0;i<i_number[4];i++)
					{
						if(strcmp(str,pick_msg[i].pick_number)==0)
						{
							for(t=i;t<i_number[4];t++)
							{
								pick_msg[t] = pick_msg[t+1];
							}
							i_number[4]--; 
						}
					}
				}
				else if(cnt=='4')
				{
					printf("请输入你要修改的采摘记录编号：");
					scanf("%s",str);
					for(i=0;i<i_number[4];i++)
					{
						if(strcmp(str,pick_msg[i].pick_number)==0)
						{
							while(1)
							{
								printf("请输入所需修改项(1.采摘地块 2.采摘作物种类 3.数量 4.实施时间 5.实施人 6.检查人 0.退出)："); 
								cnt = getch();
								if(cnt=='0')
								{
									break;				
								}
								else if(cnt=='1')
								{
									printf("请输入修改后的信息：");
									scanf("%s",pick_msg[i].pick_earth_number);
								}
								else if(cnt=='2')
								{
									printf("请输入修改后的信息：");
									scanf("%s",pick_msg[i].pick_crop_number);
								}
								else if(cnt=='3')
								{
									printf("请输入修改后的信息：");
									scanf("%d",&pick_msg[i].pick_num);
								}
								else if(cnt=='4')
								{
									printf("请输入修改后的信息：");
									scanf("%s",pick_msg[i].pick_action_time);
								}
								else if(cnt=='5')
								{
									printf("请输入修改后的信息：");
									scanf("%s",pick_msg[i].pick_action_peo);
								}
								else if(cnt=='6')
								{
									printf("请输入修改后的信息：");
									scanf("%s",pick_msg[i].pick_check_peo);
								}
							}	
						}
					}
				}
				else if(cnt=='5')
				{
					printf("请输入你要查询的采摘记录编号：");
					scanf("%s",str);
					printf("采摘记录编号\t采摘地块\t采摘作物种类\t数量\t实施时间\t实施人\t检查人\n");
					for(i=0;i<i_number[4];i++)
					{
						if(strcmp(str,pick_msg[i].pick_number)==0)
						{
							printf("%s\t%s\t%s\t%d\t%s\t%s\t%s\n",pick_msg[i].pick_number,pick_msg[i].pick_earth_number,pick_msg[i].pick_crop_number,
							pick_msg[i].pick_num,pick_msg[i].pick_action_time,pick_msg[i].pick_action_peo,pick_msg[i].pick_check_peo);
						}
					}
				}
			}	
		} 
	}
} 

void finance_set()			//财务管理
{
	int i,t;
	char cnt,str[MIN];
	while(1)
	{
		printf("请选择功能(1.经营管理 2.财务管理 0.退出):\n");
		cnt = getch();
		if(cnt=='0')
		{
			break;	
		}
		else if(cnt=='1')
		{
			while(1)
			{
				printf("请选择功能项(1.增 2.浏览 3.删 4.修改 5.查询 0.退出)：\n");
				cnt = getch();
				if(cnt=='0')
				{
					break;
				}
				else if(cnt=='1')
				{
					printf("请输入经营记录编号：");
					scanf("%s",run_msg[i_number[5]].run_number);
					printf("请输入经营种类(施肥、浇水、除草)：");
					scanf("%s",run_msg[i_number[5]].run_sort);
					printf("请输入实施时间：");
					scanf("%s",run_msg[i_number[5]].run_action_time);
					printf("请输入实施人：");
					scanf("%s",run_msg[i_number[5]].run_action_peo);
					printf("请输入检查人：");
					scanf("%s",run_msg[i_number[5]].run_check_peo);
					i_number[5]++;
					puts("录入成功");
				}
				else if(cnt=='2')
				{
					printf("经营记录编号\t经营种类\t实施时间\t实施人\t检查人\n");
					for(i=0;i<i_number[5];i++)
					{
						printf("%s\t%s\t%s\t%s\t%s\n",run_msg[i].run_number,run_msg[i].run_sort,run_msg[i].run_action_time,
						run_msg[i].run_action_peo,run_msg[i].run_check_peo);
					}
				}
				else if(cnt=='3')
				{
					printf("请输入需要删除的经营记录编号：");
					scanf("%s",str);
					for(i=0;i<i_number[5];i++)
					{
						if(strcmp(str,run_msg[i].run_number)==0)
						{
							for(t=i;t<i_number[5];t++)
							{
								run_msg[t] = run_msg[t+1];
							}
							i_number[5]--; 
						}
					}
				}
				else if(cnt=='4')
				{
					printf("请输入你要修改的经营记录编号：");
					scanf("%s",str);
					for(i=0;i<i_number[5];i++)
					{
						if(strcmp(str,run_msg[i].run_number)==0)
						{
							while(1)
							{
								printf("请输入所需修改项(1.经营种类 2.实施时间 3实施人 4.检查人 0.退出)："); 
								cnt = getch();
								if(cnt=='0')
								{
									break;				
								}
								else if(cnt=='1')
								{
									printf("请输入修改后的信息：");
									scanf("%s",run_msg[i].run_sort);
								}
								else if(cnt=='2')
								{
									printf("请输入修改后的信息：");
									scanf("%d",&run_msg[i].run_action_time);
								}
								else if(cnt=='3')
								{
									printf("请输入修改后的信息：");
									scanf("%s",run_msg[i].run_action_peo);
								}
								else if(cnt=='4')
								{
									printf("请输入修改后的信息：");
									scanf("%s",run_msg[i].run_check_peo);
								}
							}	
						}
					}
				}
				else if(cnt=='5')
				{
					printf("请输入你要查询的经营记录编号：");
					scanf("%s",str);
					printf("经营记录编号\t经营种类\t实施时间\t实施人\t检查人\n");
					for(i=0;i<i_number[5];i++)
					{
						if(strcmp(str,run_msg[i].run_number)==0)
						{
							printf("%s\t%s\t%s\t%s\t%s\n",run_msg[i].run_number,run_msg[i].run_sort,run_msg[i].run_action_time,
							run_msg[i].run_action_peo,run_msg[i].run_check_peo);
						}
					}
				}
			}
		} 
		else if(cnt=='2')
		{
			while(1)
			{
				printf("1.收入查询与修改 2.支出查询与修改 0.退出\n");
				cnt = getch();
				if(cnt=='0')
				{
					break;
				}
				else if(cnt=='1')
				{
					printf("当前收入为%d元，是否修改(1.是 0.否)：\n",finance_msg.finance_gain);
					cnt=getch();
					if(cnt=='1')
					{
						printf("请输入修改后的金额：");
						scanf("%d",&finance_msg.finance_gain);
						puts("修改成功！");
					}
				}
				else if(cnt=='2')
				{
					printf("当前支出为%d元，是否修改(1.是 0.否)：\n",finance_msg.finance_cast);
					cnt=getch();
					if(cnt=='1')
					{
						printf("请输入修改后的金额：");
						scanf("%d",&finance_msg.finance_cast);
						puts("修改成功！");
					}
				} 
			}
		} 
	}
 } 

int main()
{
	ListNode* list = (ListNode*)malloc(sizeof(ListNode));
	int cnt;
	char chr,st;
	printf("欢迎使用！！！\n");
	cnt=login(list);
	while(1)
	{
		system("cls");
		printf("空格键退出，q键切换其他账户，其他键继续\n");
		chr = getch();
		if(chr==' ')
		{
			break;
		}
		else if(chr=='q' || chr=='Q')
		{
			cnt=login(list);
		}
		if(cnt==0)			//不存在此账户，注册 
		{
			printf("未注册\n");
			printf("是否注册(1.是 0.否)\n");
			st = getch();
			if(st=='1')
			{
				cnt = signin(list);
			} 
			else if(st=='0')
			{
				printf("谢谢使用！！！\n");
				return 0;
			}
		}
		else if(cnt==1)		//管理员
		{
			Administrator_set(list);
		}
		else if(cnt==2)			//操作员 
		{
			operate_set();
		}
		else if(cnt==3)		//财务管理 
		{
			finance_set();
		}  
	}
	return 0;
}
