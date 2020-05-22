#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX 1000
#define MIN 20

int i_number;		//数据记录 

struct node{
	char number[MIN];		//编号 
	char name[MIN];		//名称 
	struct node *next;//下一个结构体
};

//创建结构体
struct node *listcreat()
{
     struct node *p;
     p=(struct node *)malloc(sizeof(struct node));
     p->next=NULL;
     return p;
}

//添加学生信息
void list_add(struct node *head)
{
    //采取尾插法
    int n;//添加学生数的变量
    struct node *p,*q,*temp=head;
    while(temp->next!=NULL)//这个循环是找到尾结点
        temp=temp->next;
    q=temp; //temp的尾结点就相当于应该一个新的头结点

    printf("请输入你要添加的数量:");
    scanf("%d",&n);
    printf("编号 名称\n");
    while(n--)//尾插法操作
    {
        p=(struct node *)malloc(sizeof(struct node)); //申请空间
        scanf("%s %s",p->number,p->name);
        p->next=q->next;
        q->next=p;
        q=p;
    }
}

//查找学生信息
void list_lookup(struct node *head,int n)
{
    struct node *p;
    p=head->next;
    char stnum[20];
    printf("请输入需要查找的编号:");
    scanf("%s",stnum);
    while(strcmp(p->number,stnum))//采用对链表每个数据进行比较，一样的返回就是0，跳出，并进行输出
    {
        p=p->next;
    }
    if(n==1)
	{
		puts("商品编号\t商品名称");
		printf("%s\t%s\n",p->number,p->name);
	}
	else if(n==2)
	{
		puts("员工编号\t员工名称");
		printf("%s\t%s\n",p->number,p->name);
	}
	else if(n==3)
	{
		puts("会员编号\t会员名称");
		printf("%s\t%s\n",p->number,p->name);
	}
    
}

//找到第一个含有某个值的节点并删除该节点
void list_del(struct node *head)
{
	if (head == NULL) return;
	struct node* RemoveNode = NULL;
 	char name[20];
 	printf("请输入需要删除的编号：");
 	scanf("%s",name);
	//要删除的是头节点
	if (strcmp(head->number,name)==0){
		RemoveNode = head;
		head = head->next;
	}
	//要删除的不是头节点
	else{
		struct node *node = head;
		while (node->next != NULL && strcmp(node->next->number,name)!=0){
			node = node->next;
		}
		if (node->next != NULL && strcmp(node->next->number,name)==0){
			RemoveNode = node->next;
			node->next = node->next->next;
		}
 
	}
	if (RemoveNode != NULL){
//		delete RemoveNode;
		RemoveNode = NULL;
	}
 	puts("删除成功");
}

void init(struct node *head)   //初始导入txt文本文件函数 
{
	FILE*fp;  //定义文本字符指针
	int flag=0;
	if(!(fp=fopen("test.txt","r"))) 
	{
		fp=fopen("test.txt","w+");
		flag=1;
	}
	if(!flag)
	{
		struct node *p,*q,*temp=head;
	    while(temp->next!=NULL)//这个循环是找到尾结点
	        temp=temp->next;
	    q=temp; //temp的尾结点就相当于应该一个新的头结点
		while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中 
		{
			p=(struct node *)malloc(sizeof(struct node)); //申请空间
	        fscanf(fp,"%s\t%s\n",p->number,p->name);
	        p->next=q->next;
	        q->next=p;
	        q=p;
		}
	}
	fclose(fp);   //关闭文本 
	puts("读取成功"); 
} 

void save(struct node *head){   //保存文本函数 
	FILE*fp;
	struct node *p;
    p=head->next;
	fp=fopen("test.txt","w"); // 打开新建的文件遍历写入内存中的自信息 
	while(p)
	{
		fprintf(fp,"%s\t%s\n",p->number,p->name);
		p = p->next;
	}
	fclose(fp);
	puts("保存成功"); 
} 

void deal()		//主处理函数
{
	char cnt;
	int flag = 0;
	struct node *head=listcreat();//创建一个头结点，作为链表的入口
	init(head);
	while(1)
	{
		puts("请选择业务类型(a.服务项目 b.信息管理 空格.退出)");
		cnt = getch();
		switch(cnt)
		{
			case ' ':{
				save(head);
				flag = 1;
				break;
			}
			case 'a':{
				while(1)
				{
					puts("请选择服务类型(a.吹洗 b.吹剪洗 c.烫发 d.染发  空格.退出)");
					cnt = getch();
					if(cnt==' ')
					{
						break;	
					} 
					else if(cnt=='a')
					{
						puts("你选择的服务是吹洗"); 
					}
					else if(cnt=='b')
					{
						puts("你选择的服务是吹剪洗");
					}
					else if(cnt=='c')
					{
						puts("你选择的服务是烫发");
					}
					else if(cnt=='d')
					{
						puts("你选择的服务是染发");
					}
					else
					{
						puts("输入有误！！！");
					}
				}
				break;
			}
			case 'b':{
				while(1)
				{
					puts("请选择功能(a.商品信息 b.员工信息 c.会员信息 空格.退出)");
					cnt = getch();
					if(cnt==' ')
					{
						break;
					}
					else if(cnt=='a')
					{
						while(1)
						{
							puts("请选择(a.添加 b.删减 c.浏览 空格.退出"); 
							cnt = getch();
							if(cnt==' ')
							{
								break;
							}
							else if(cnt=='a')
							{
								list_add(head);
							}
							else if(cnt=='b')
							{
								list_del(head);
							}
							else if(cnt=='c')
							{
								list_lookup(head,1);
							}
							else
							{
								puts("输入有误！！！");
							}
						} 	
					}
					else if(cnt=='b')
					{
						while(1)
						{
							puts("请选择(a.添加 b.删减 c.浏览 空格.退出"); 
							cnt = getch();
							if(cnt==' ')
							{
								break;
							}
							else if(cnt=='a')
							{
								list_add(head);
							}
							else if(cnt=='b')
							{
								list_del(head);
							}
							else if(cnt=='c')
							{
								list_lookup(head,2);
							}
							else
							{
								puts("输入有误！！！");
							}
						}	
					}
					else if(cnt=='c')
					{
						while(1)
						{
							puts("请选择(a.添加 b.删减 c.浏览 空格.退出"); 
							cnt = getch();
							if(cnt==' ')
							{
								break;
							}
							else if(cnt=='a')
							{
								list_add(head);
							}
							else if(cnt=='b')
							{
								list_del(head);
							}
							else if(cnt=='c')
							{
								list_lookup(head,3);
							}
							else
							{
								puts("输入有误！！！");
							}
						} 	
					}
					else
					{
						puts("输入有误！！！");
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
 } 

int main()
{
	deal();
	return 0;
} 
