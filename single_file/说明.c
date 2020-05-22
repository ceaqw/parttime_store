
/*头文件调用部分*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdlib.h>
#define LEN sizeof(struct node)		//获取结构体大小
#define MAX 147			
/*定义储存结构体*/
struct node
{
	char cc;
	struct node *next;
};
/*链表节点反序对比*/
int judge(struct node *head,int len)
{
	int i=0;
	struct node *top,*p1,*p2;
	top = NULL;
	p1 = head->next;
	for( i = 0 ; i < len/2 ; i++)//循环反序链表节点数据，储存在P2中
	{
		p2 = (struct node *)malloc(LEN);
		p2->cc = p1->cc;
		p2->next = top;
		top = p2;
		p1 = p1->next;
	}
	if(len%2 == 1)
		p1 = p1->next;
		p2 = top;
	for(i = 0 ; i < len/2 ; i++)
	{
		if(p2->cc != p1->cc)	//如果对比中心对称两边的字母不相等，跳出循环
			break;
		top = p2->next;
		p1 = p1->next;
		p2 = top;
	}
	if(!top)
		return 1;		//对称返回1
	else
		return 0;		//不对称返回0
}
/*主函数调用*/
int main()
{
	int n=0,len=0,flag=0;
	char str[MAX];
	struct node *head,*p;
	head = p = (struct node *)malloc(LEN);
	head->next = p->next = NULL;
	printf("请输入一个字符串：\n");
	gets(str);
	len = strlen(str);
	while(n < len && str[0] != '\n')		//将字符串储存在链表P1中
	{
		p = (struct node *)malloc(LEN);
		p->cc = str[n];
		p->next = head->next;
		head->next = p;
		n++;
	}
	flag = judge(head,len);		//调用判断是否对称函数
	if(flag)
		printf("%s是对称的!\n",str);
	else
		printf("%s不是对称的!\n",str);
	system("pause");
	return 0;
}