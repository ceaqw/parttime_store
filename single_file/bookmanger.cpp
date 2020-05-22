#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct 
{
	char ISBN[10];		//书号
	char book[30];		//书名
	char author[20];	//作者
	int edition;		//版本号
	char press[50];		//出版社名
	int year;			//出版年	
}Bookinfo; 

int Input(Bookinfo dictList[],int n)		//数据输入 
{
	int i,cnt;
	printf("请输入书籍数量：") ;
	scanf("%d",&cnt);
	cnt = cnt + n; 			//判断储存位置 
	for(i=n;i<cnt;i++)
	{
		printf("第%d本\n",i+1);
		printf("请输入书号：");
		scanf("%s",dictList[i].ISBN);
		printf("请输入书名：");
		scanf("%s",dictList[i].book);
		printf("请输入作者：");
		scanf("%s",dictList[i].author);
		printf("请输入版本号：");
		scanf("%d",&dictList[i].edition);
		printf("请输入出版社名：");
		scanf("%s",dictList[i].press);
		printf("请输入出版年：");
		scanf("%d",&dictList[i].year);
		n++;			//输入完成，总数加一 
	}
	return n; 
}

void Display(Bookinfo dictList[],int n)		//数据输出
{
	int i;
	system("cls");
	puts("***************>>>>>>当前模式为浏览状态<<<<<<*************");
	puts("书号\t书名\t作者\t版本号\t出版社号\t出版年");
	for(i=0;i<n;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%s\t%s\t%s\t%d\t%s\t%d\n",dictList[i].ISBN,dictList[i].book,dictList[i].author,
			dictList[i].edition,dictList[i].press,dictList[i].year);
		if(i+1%10 == 0)
		{
			system("pause");
		}
	} 
	system("pause");
} 

int Delte(Bookinfo dictList[],int n,char *book)		//删除对应书，返回剩余书籍数 
{
	int i,flag = 0;
	for(i=0;i<n;i++)
	{
		if(strcmp(book,dictList[i].book) == 0)
		{
			printf("已找到，是否删除  1.是 2.否");
			scanf("%d",&flag);
			if(flag == 1)		//删除 
			{
				dictList[i] = dictList[i+1];
				n--;
				break;
			} 
			else
			{
				printf("未删除\n");
				break;
			}
		}
	}
	if(flag)
	{
		puts("删除成功");	
		n--;		//书籍数量减一 
	}
	else
	{
		puts("未找到该书籍信息");
	}
	return n;
} 

int Delte_a_record(Bookinfo dictList[],int n)		//输入待删除书名，调用Delte函数
{
 
	char name[10]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息删除状态<<<<<<*************");
	printf("请输入删除的姓名：");
	scanf("%s",name);
	n = Delte(dictList,n,name);		//调用删除书籍函数 
	return n;				//返回剩余书籍数 
} 

void Sort_by_name(Bookinfo dictList[],int n)		//数组按书名升序排列
{
	int i,r,t;
	Bookinfo temp;
	system("cls");
	puts("***************>>>>>>当前模式为书名升序排序状态<<<<<<*************");	
	for(r=1; r<n; r++) 
	{
		for(t=0; t<n-1; t++) 
		{
			if(dictList[t+1].book < dictList[t].book)   //由高到低 
			{  
				temp=dictList[t+1];
				dictList[t+1]=dictList[t];
				dictList[t]=temp;
			}
		}
	}
	puts("书号\t书名\t作者\t版本号\t出版社号\t出版年");
	for(i=0;i<n;i++)    //遍历打印内存中储存的学生信息 
	{
		printf("%s\t%s\t%s\t%d\t%s\t%d\n",dictList[i].ISBN,dictList[i].book,dictList[i].author,
			dictList[i].edition,dictList[i].press,dictList[i].year);
	} 
	puts("排序完成");
	system("pause");
}

int Insert_a_record(Bookinfo dictList[],int n)		//输入待插入信息
{
	int i,r,t;
	Bookinfo temp;
	Bookinfo Dictionary;
	printf("请输入插入的书名：");
	scanf("%s",Dictionary.book);
	printf("请输入插入的书号："); 
	scanf("%s",Dictionary.ISBN);
	printf("请输入插入的作者：");
	scanf("%s",Dictionary.author);
	printf("请输入插入的版本号：");
	scanf("%d",&Dictionary.edition);
	printf("请输入插入的出版社名：");
	scanf("%s",Dictionary.press);
	printf("请输入插入的出版年：");
	scanf("%d",&Dictionary.year);
	dictList[n] = Dictionary;
	n++;
	for(r=1; r<n; r++) 		//进行插入排序 
	{
		for(t=0; t<n; t++) 
		{
			if(dictList[t+1].book > dictList[t].book)   //由高到低 
			{  
				temp=dictList[t+1];
				dictList[t+1]=dictList[t];
				dictList[t]=temp;
			}
		}
	}
	puts("插入完成");
	return n; 
} 

int Qurey(Bookinfo dictList[],int n,char *book)		//查找
{
	int i,flag = 0;
	for(i=0;i<n;i++)
	{
		if(strcmp(book,dictList[i].book) == 0)
		{
			puts("书号\t书名\t作者\t版本号\t出版社号\t出版年");
			printf("%s\t%s\t%s\t%d\t%s\t%d\n",dictList[i].ISBN,dictList[i].book,dictList[i].author,
			dictList[i].edition,dictList[i].press,dictList[i].year);
			flag =1;
			break;
		}
	}
	if(flag)
	{
		puts("查找成功");	
	}
	else
	{
		puts("未找到该用户信息");
	} 
	system("pause");
}

void Qurey_a_record(Bookinfo dictList[],int n)
{

	char name[10]; 
	system("cls");
	puts("***************>>>>>>当前模式为信息查找状态<<<<<<*************");
	printf("请输入查找的书名：");
	scanf("%s",name);
	Qurey(dictList,n,name); 
}

int AddformText(Bookinfo dictList[],int n,char *filename)
{
	FILE*fp;  //定义文本字符指针 
	while(!(fp=fopen(filename,"r")))
	{
		printf("输入有误！请重新输入：");
		scanf("%s",filename);
	}
	while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中 
	{
		fscanf(fp,"%s\t%s\t%s\t%d\t%s\t%d\n",dictList[n].ISBN,dictList[n].book,dictList[n].author,
			dictList[n].edition,dictList[n].press,dictList[n].year);
		n++;
	}
	fclose(fp);   //关闭文本 
	puts("读取成功"); 
	return n; 
}

void WritetoText(Bookinfo dictList[],int n,char *filename)
{
	FILE*fp;
	int i;
	if(!(fp = fopen(filename,"w")))  // 打开新建的文件遍历写入内存中的自信息 
	{
		fp = fopen(filename,"w+");
	}
	for(i=0; i<n; i++){
	    fprintf(fp,"%s\t%s\t%s\t%d\t%s\t%d\n",dictList[i].ISBN,dictList[i].book,dictList[i].author,
			dictList[i].edition,dictList[i].press,dictList[i].year);			
	}
	fclose(fp);
	puts("保存成功"); 
}

void Display_main_meau()		//菜单
{
	Bookinfo dictList[200];				//先设置200本吧 
	int num = 0,cnt;		//记录书籍数量 
	char *name;
	puts("***************************欢迎使用************************");
	puts("-----------------------------------------------------------");
	
	while(1)
	{
		printf("请输入你要选择的功能\n(1.书籍输入 2.信息浏览 3.删除信息 4.插入书籍 5.排序\n 6.查找书籍 7.导入书籍 8.写入文件 0.退出)\n"); 
		scanf("%d",&cnt);
		system("cls"); 
		if(cnt==0)
		{
			printf("谢谢使用\n");
			break;
		}
		else if(cnt==1)		
		{
			num = Input(dictList,0);		//调用输入函数
		}
		else if(cnt==2)
		{
			Display(dictList,num); 		//调用显示函数 
		}
		else if(cnt==3)
		{
			num = Delte_a_record(dictList,num);		//调用删除函数 
		}
		else if(cnt==4)
		{
			num = Insert_a_record(dictList,num);		//调用插入函数 
		}
		else if(cnt==5)
		{
			Sort_by_name(dictList,num);		//调用排序函数 
		}
		else if(cnt==6)
		{
			Qurey_a_record(dictList,num);		//调用查找函数
		}
		else if(cnt==7)
		{
			printf("请输入需要导入的文件名称：");
			scanf("%s",name);
			num = AddformText(dictList,num,name);		//调用导入函数
		}
		else if(cnt==8)
		{
			printf("请输入需要输出的文件名称：");
			scanf("%s",name);
			WritetoText(dictList,num,name); 		//写入文件 
		}
		else
		{
			printf("输入有误！！！\n");
		}
	}	
} 

int main()
{	
	Display_main_meau();
	system("pause");
	return 0;
}
