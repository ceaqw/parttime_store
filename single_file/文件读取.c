#include<stdio.h>
#include<string.h>
#define MAX 100

int main()
{
	int num = 0;
	char name[20];
	char msg[MAX];
	FILE *fp; 
	printf("请输入你要打开的文件名称：");
	scanf("%s",name);
	fp = fopen(name,"r");
	while(!fp)
	{
		printf("文件不存在！！！请重新输入：");
		scanf("%s",name);
		fp = fopen(name,"r");
	}
	while (!feof(fp))
	{
		fscanf(fp,"%s",msg);
	}
	num = strlen(msg);
	printf("最后一个字符为:\n\n");
	printf("   \n");
	printf(" %c ",msg[num-1]);
	printf("   \n");
	return 0;	
} 
