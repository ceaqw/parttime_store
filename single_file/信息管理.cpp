#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000

int i_number;  //全局变量，记录人数 
char fp_name[MIN];		//文本名称 

struct _STAFF
{
    char nu[MIN];
	char name[MIN];
	int in;
	int year;
    int month;
    int day;
}data[MAX];

void deal();  //调用功能函数 

void init()   //初始导入txt文本文件函数 
{
	FILE*fp;  //定义文本字符指针 
	int n;
	printf("请输入读取的文件名：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1){
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
			fscanf(fp,"%s %s %d %d %d %d\n",data[i_number].nu,data[i_number].name,&data[i_number].in,
			&data[i_number].year,&data[i_number].month,&data[i_number].day);
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


void notice() {    //进入系统的主界面显示提息
 	printf("\n1.添加节点\n");
    printf("2.输出所有节点\n");
    printf("3.插入节点\n");
    printf("4.修改节点\n");
    printf("5.删除节点\n");
    printf("6.退出系统\n");
    printf("请选择所需操作：\n");

}
 
void case1()
{
//	system("cls");
	puts("***************>>>>>>当前模式为信息添加状态<<<<<<*************");
	printf("请输入商品编号，名称，入库量，年月日:\n");
	scanf("%s %s %d %d %d %d\n",data[i_number].nu,data[i_number].name,&data[i_number].in,
		&data[i_number].year,&data[i_number].month,&data[i_number].day);
	puts("录入成功");
	i_number++;

}
 
void case2()  //信息浏览函数 
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为信息浏览状态<<<<<<*************");
	for(i=0;i<i_number;i++)   
	{
		printf("%s %s %d %d %d %d\n",data[i].nu,data[i].name,data[i].in,
		data[i].year,data[i].month,data[i].day);
	} 
	system("pause");
}

void case3()
{
	int i,num,k;
	printf("请输入你要插入的位置：");
	scanf("%d",&num);
	i_number++;
	for(i=0;i<i_number;i++)
	{
		if(i == num)
		{
			for(k=i_number;k>=i;k--)
			{
				data[k] = data[k-1];
			}
			break;
		}
	}
	printf("请输入商品编号，名称，入库量，年月日:\n");
	scanf("%s %s %d %d %d %d\n",data[num].nu,data[num].name,&data[num].in,
		&data[num].year,&data[num].month,&data[num].day);
	puts("插入成功"); 
}

void case4()
{
	system("cls");
	puts("***************>>>>>>当前模式为信息修改状态<<<<<<*************");
	int i;
	char s[MIN];
	printf("请输入要修改的商品名称：");
	scanf("%s",s);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(s,data[i].name) == 0)
		{
			printf("请输入商品编号，名称，入库量，年月日:\n");
			scanf("%s %s %d %d %d %d\n",data[i].nu,data[i].name,&data[i].in,&data[i].year,&data[i].month,&data[i].day);	
		}
	}
	puts("修改成功"); 
}

void case5()
{
	system("cls");
	puts("***************>>>>>>当前模式为信息删除状态<<<<<<*************");
	int i,k;
	char s[MIN];
	printf("请输入删除的商品编号:");
	scanf("%s",s);
	for(i=0;i<i_number;i++)
	{
		if(strcmp(s,data[i].name) == 0)
		{
			for(k=i;k<i_number;k++)
			{
				data[i] = data[i+1];
			}
			i_number--;
			puts("删除成功");
			break;
		}
		else
		{
			printf("未找到该信息!!!\n");
		}
	}
}

void deal() {//主要处理函数
	int k,flag,p,i;
	p=5;
	flag=1;
	while(flag) {
		if(p>1)
			notice();
			scanf("%d",&k);
			switch(k) {
				case 0: {				//退出 
					puts("温馨提醒：");
					FILE*fp;
					fp = fopen(fp_name,"w"); 
					for(i=0; i<i_number; i++)
					{
					    fprintf(fp,"%s %s %d %d %d %d\n",data[i].nu,data[i].name,&data[i].in,&data[i].year,&data[i].month,&data[i].day);		
					}
					fclose(fp);
					puts("文件已保存"); 
					puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
					flag=0;
					break;
				}
				case 1:
	                case1();		//添加节点 
	                break;
	            case 2:
	                case2();		//浏览节点 
	                break;
	            case 3:
	                case3();		//插入节点 
	                break;
	            case 4:
	                case4();		//修改节点 
	                break;
	            case 5:
	                case5();		//删除节点 
	                break;
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
    printf("商品入库信息管理系统！(正确退出文件即可保存)\n");
    init();
    deal();
    return 0;
}

