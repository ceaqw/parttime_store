//定义
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<time.h> 
#define Pquantity 3
#define MAX 1000
#define N 5

int k=0;
//结构体
typedef struct
{
	int Good_num;/*编号*/
	char Good_name[20];/*商品名*/
	char Good_author[20];/*生产商*/
	char Good_time[20];/*入库时间*/
	char Good_out_time[20];/*出库时间*/ 
	char press[20];/*产地*/
	float Good_price;/*价格*/
	int Good_quantity;/*数量*/
	int flag;		//标记 
}Good;

int Read(Good stu[]);//保存商品信息
int Input_Info(Good stu[]);//商品录入
void Output_Info(Good stu[]);//打印信息
void Slect_Info(Good stu[]);//商品核实
void Revise_info(Good stu[]);//修改信息
void Delete_Info (Good stu[]);//删除信息
void denglu();//信息登陆
void SAVE_Info(Good stu[],int sum);//商品录入文件
void Statistics_Info(Good stu[]);	//信息统计 
void Out_Good_Info(Good stu[]);		//商品输出 
char* Time();						//时间 

//系统主界面
int main()
{
	//定义要显示的显示界面
	Good stu[20];
	int choice,k,sum;//k为输入商品数，sum为文件商品数
	printf("\n\n\n");
	puts("       *****************************************      ");
	puts("       *                                       *      ");
	puts("       *                                       *      ");
	puts("       *      超市仓库管理系统                 *      ");
	puts("       *                                       *      ");
	puts("       *      请登录                           *      ");
	puts("       *****************************************      ");
	// 登陆界面
	denglu();
	sum=Read(stu);
	if(sum==0)//如果文件空，则录入，不为空
	{
		printf("第一先录入基本库存信息！按Enter键后进入——\n");
		getchar();
		sum= Input_Info(stu);
	}

	do
	{
		system("cls");
		printf("\n\n\n********超市仓库管理系统********\n\n");
		printf("          1.创建商品信息\n\n");
		printf("          2.打印商品信息\n\n");
        printf("          3.修改商品信息\n\n");
        printf("          4.删除商品信息\n\n");
		printf("          5.信息统计\n\n");
		printf("          6.商品出库管理\n\n");
        printf("          0.退出系统\n\n");
        printf("           请选择（0~6）：\n\n");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: k=Input_Info(stu);break;  
			case 2: Output_Info(stu);break;  
			case 3: Revise_info(stu);break;  
			case 4: Delete_Info(stu);break;  
			case 5: Statistics_Info(stu);break;	
			case 6: Out_Good_Info(stu);break;		
			case 0: break;
		}
	}while(choice!=0);
	SAVE_Info(stu,sum);//保存到结构体数组中*/
	return 0;
}

char* Time()
{
	char str[2][20]={"0"};
	time_t timep,Tim;
    struct tm *p;
    time(&timep);
    p =  localtime(&timep); //此函数获得的tm结构体的时间，是已经进行过时区转化为本地时间     
    int Year = 1900 + p->tm_year;
    int Month = 1 + p->tm_mon;
    int Day = p->tm_mday;
    int Hour = p->tm_hour;
    int Minute = p->tm_min;
	itoa(Year,str[0],10);
	itoa(Month,str[1],10);
	strcat(str[0],":");
	strcat(str[0],str[1]);
	itoa(Day,str[1],10);
	strcat(str[0],":");
	strcat(str[0],str[1]);
	itoa(Hour,str[1],10);
	strcat(str[0],":");
	strcat(str[0],str[1]);
	itoa(Minute,str[1],10);
	strcat(str[0],":");
	strcat(str[0],str[1]);
	return str[0];
}

//保存商品信息
int Read(Good stu[])
{
	FILE *fp;
	int i=0;
	if((fp=fopen("stu.txt","rt"))==NULL)
	{
		printf("\n\n------文件不存在!请手动创建");
		return 0;
	}
	while(!feof(fp))//读入文件内容，每读一行并写到一个结构体
	{
		fread(&stu[i],sizeof(Good),1,fp);
		if(stu[i].Good_num==0)
		break;
		else
		i++;
	}
	fclose(fp);
	return i;
}

void SAVE_Info(Good stu[],int sum)//将结构体中的商品信息保存到有关文件
{
	FILE*fp;
	int i;
	if((fp=fopen("stu.txt","wb"))==NULL)//打开文件
	{
		printf("写文件错误！\n");
		return;
	}
	for(i=0;i<sum;i++)//将结构体中的每个结构体都写入文件
	{
		if(fwrite(&stu[i],sizeof(Good),1,fp)!=1)
		{
			printf("写入文件错误!\n");
		}
	}
	fclose(fp);
}

//商品信息的录入
int Input_Info(Good stu[])
{  
   int i,x,n,flag=0,choice;
   for(i=0;i<MAX;i++)
   {
	   system("cls");
	   printf("\n\n      录入商品(最多%d)\n",MAX);
	   printf("-----------------------------------------------\n");
	   printf("\n         第%d个",k+1);
	   printf("\n 请输入商品的编号：");
	   scanf("%d",&stu[k].Good_num);
	   for(n=0;n<k;n++)
	   {
	   		if(stu[k].Good_num==stu[n].Good_num)
	   		{
	   			printf("此商品已存在！！！\n");
				printf("\n编号：%d,名称：%s,入库时间：%s,出库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[n].Good_num,stu[n].Good_name,stu[n].Good_time,stu[n].Good_out_time,stu[n].Good_author,stu[n].press,stu[n].Good_price,stu[n].Good_quantity);
				printf("是否修改：(1.是 0.否)\n");
				if(getch()=='1')
				{
						printf("\n编号：%d,名称：%s,入库时间：%s,出库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[n].Good_num,stu[n].Good_name,stu[n].Good_time,stu[n].Good_out_time,stu[n].Good_author,stu[n].press,stu[n].Good_price,stu[n].Good_quantity);
						while(1)
						{
							printf("\n\n\n ********请输入您想要修改的数据********\n\n");//根据提示显示需要修改的信息
							printf("        1.编号\n\n");
							printf("        2.名称\n\n");
							printf("        3.生产商\n\n");
							printf("        4.产地\n\n");
							printf("        5 .价格\n\n");
							printf("        6.数量\n\n");
							printf("     7.入库时间\n\n"); 
							printf("	 8.出库时间\n\n");
							printf("		0.退出\n\n");
							printf("        请选择(1~8):");
							scanf("%d",&choice);
							switch(choice)
							{
							case 0:{
								flag = 1;
								break;
							}
							case 1:{
								printf("\n   请输入您修改的新编号");
							     scanf("%d",&stu[n].Good_num);
								break;
							        } 
							case 2:{
								printf("\n  请输入您修改的新名称");
								  scanf("%s",stu[n].Good_name);
								break;
								   }
							case 3:{
								   printf("\n 请输入您修改的新生产商");
								     scanf("%s",stu[n].Good_author);
								   break;
								   }
							
							case 4:{
								   printf("\n 请输入您修改的新产地");
								   scanf("%s",stu[n].press);
								   break;
								   }
							case 5:{
								   printf("\n  请输入您修改的新价格");
								   scanf("%f",&stu[n].Good_price);
								   break;
								   }
							case 6:{
								   printf("\n  请输入您改的新数量");
								   scanf("%d",&stu[n].Good_quantity);
								   break;
								   }
							case 7:{
									printf("\n  请输入您改的新时间");
									scanf("%s",stu[n].Good_time);
									break;
									}
							case 8:{
									printf("\n  请输入您改的新时间");
									scanf("%s",stu[n].Good_out_time);
									break;
									}
							}
							if(flag)
							{
								break;
							}
						}
						 printf("\n编号：%d,名称：%s,入库时间：%s,出库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[n].Good_num,stu[n].Good_name,stu[n].Good_time,stu[n].Good_out_time,stu[n].Good_author,stu[n].press,stu[n].Good_price,stu[n].Good_quantity);
						 printf("修改完成！\n");
					}
					flag=1;
				}
		}
	   if(!flag)
	   {
	   	   flag=0;
	       printf("\n 请输入商品的名称：");
		   scanf("%s",stu[k].Good_name);
		   printf("\n 请输入商品的生产商：");
		   scanf("%s",stu[k].Good_author);
		   printf("\n 请输入商品的产地：");
		   scanf("%s",stu[k].press);
		   printf("\n 请输入商品的价格：");
		   scanf("%f",&stu[k].Good_price);
	       printf("\n 请输入商品的数量：");
		   scanf("%d",&stu[k].Good_quantity);
//		   printf("\n 请输入商品的入库时间：");
		   strcpy(stu[k].Good_time,Time());
//		   printf("\n 请输入商品的出库时间：");
		   strcpy(stu[k].Good_out_time,"未知");
		   k++; 
	   }
	   flag = 0; 
	   printf("\n 请按1返回菜单或按0继续创建");
	   scanf("%d",&x);
	    if(x)
			break;
   }
    return k;
}

/*打印信息*/
void Output_Info(Good stu[])
{
	int i;
	system("cls");
	for (i=0;i<k;i++)
	{
		printf("\n编号：%d,名称：%s,入库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[i].Good_num,stu[i].Good_name,stu[i].Good_time,stu[i].Good_author,stu[i].press,stu[i].Good_price,stu[i].Good_quantity);	
	}
	printf("按任意键加Enter键返回主菜单！");
	scanf("%d",&i);
	getchar();
}

/*修改商品信息*/
void Revise_info(Good stu[])
{
	int Good_num,i,choice;
	system("cls");
	printf("\n\n\n    请输入要修改的商品编号");
	scanf("%d",&Good_num);
	for(i=0;i<k;i++)
	{
		if(Good_num==stu[i].Good_num)//找到则显示
		{
			printf("\n编号：%d,名称：%s,入库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[i].Good_num,stu[i].Good_name,stu[i].Good_time,stu[i].Good_author,stu[i].press,stu[i].Good_price,stu[i].Good_quantity);
		printf("\n\n\n ********请输入您想要修改的数据********\n\n");//根据提示显示需要修改的信息
		printf("        1.编号\n\n");
		printf("        2.名称\n\n");
		printf("        3.生产商\n\n");
		printf("        4.产地\n\n");
		printf("        5 .价格\n\n");
		printf("        6.数量\n\n");
		printf("        7.入库时间\n\n"); 
		printf("		8.出库时间\n\n");
		printf("        请选择(1~8):");
		scanf("%d",&choice);
		switch(choice)
		{
		case 1:{
			printf("\n   请输入您修改的新编号");
		     scanf("%d",&stu[i].Good_num);
			break;
		
		        } 
		case 2:{
			printf("\n  请输入您修改的新名称");
			  scanf("%s",stu[i].Good_name);
			break;
			   }
		case 3:{
			   printf("\n 请输入您修改的新生产商");
			     scanf("%s",stu[i].Good_author);
			   break;
			   }
		
		case 4:{
			   printf("\n 请输入您修改的新产地");
			   scanf("%s",stu[i].press);
			   break;
			   }
		case 5:{
			   printf("\n  请输入您修改的新价格");
			   scanf("%f",&stu[i].Good_price);
			   break;
			   }
		case 6:{
			   printf("\n  请输入您改的新数量");
			   scanf("%d",&stu[i].Good_quantity);
			   break;
			   }
		case 7:{
				printf("\n  请输入您改的新时间");
				scanf("%s",stu[i].Good_time);
				break;
				}
		case 8:{
				printf("\n  请输入您改的新时间");
				scanf("%s",stu[i].Good_out_time);
				break;
				}
		}
		
	}
 printf("\n编号：%d,名称：%s,入库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[i].Good_num,stu[i].Good_name,stu[i].Good_time,stu[i].Good_author,stu[i].press,stu[i].Good_price,stu[i].Good_quantity);
 printf("按任意键返回主菜单！");
 system("pause");
 break;}
}

//删除商品信息

void Delete_Info (Good stu[])
{
	int i,j;
	char Stuname2[20];
	system("cls");
	printf("请输入商品名称");
	scanf("%s",Stuname2);
	printf("\n");
	for(i=0;i<k;i++)
		if(strcmp(stu[i].Good_name,Stuname2)==0)
			for(j=0;j<20;j++)
				stu[i].Good_name[j]=stu[i+1].Good_name[j];
	k--;
	printf("删除成功\n");
	printf("按任意键返回主菜单！");
	system("pause");

}


void denglu()
//首先验证登陆，只有正确输入密码才能使用该系统
{
	char userName[10];
	char userPWD[10];
	int i,sum;
	for (i = 1; i < 4; i++)
	{
		printf("\n        请输入你的用户名");
		scanf("%s",userName);
		printf("\n        请输入你的密码");
		scanf("%s",userPWD);
		if (strcmp(userName,"root") == 0 && strcmp(userPWD, "123654") == 0)
		{
		
			printf("\n        *登陆成功，显示主菜单*");
			return;
		}


	
        else
		{
			if (i < 3)
			{
				printf("用户名密码错误，请重新输入");
			}
			else
			{
				printf("连续三次输错用户名或密码，退出系统。");
				exit(1);
			}
		}
	}
}


void Out_Good_Info(Good stu[])
{
	int i,cnt; 
	printf("请输入出库的商品编号：");
	scanf("%d",&cnt);
	for(i=0;i<k;i++)
	{
		if(cnt==stu[i].Good_num)
		{
			printf("请输入出库数量(最多%d件):",stu[i].Good_quantity);
			scanf("%d",&cnt);
			stu[i].Good_quantity-=cnt;
			strcpy(stu[i].Good_out_time,Time());
			stu[i].flag=1; 
			puts("操作完成");
		}
	}
}

void Statistics_Info(Good stu[])
{
	char str[15];
	char cnt;
	int i,num;
	system("cls");
	puts("当前为信息统计状态");
	while(1)
	{
		puts("请选择你要实现的功能(1.产品名称查询 2.入库时间查询 3.产品编号查询 4. 查看商品历史纪录 0.退出)：");
		cnt = getch();
		if(cnt=='0')
		{
			break;
		}
		else if(cnt=='1')
		{
			printf("请输入你要查询的产品名称：");
			scanf("%s",str);
			for(i=0;i<k;i++)
			{
				if(strcmp(stu[i].Good_name,str)==0)
				{
					printf("\n编号：%d,名称：%s,入库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[i].Good_num,stu[i].Good_name,stu[i].Good_time,stu[i].Good_author,stu[i].press,stu[i].Good_price,stu[i].Good_quantity);	
				}	
			}			
		}
		else if(cnt=='2')
		{
			printf("请输入你要查询的入库时间：");
			scanf("%s",str);
			for(i=0;i<k;i++)
			{
				if(strcmp(stu[i].Good_time,str)==0)
				{
					printf("\n编号：%d,名称：%s,入库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[i].Good_num,stu[i].Good_name,stu[i].Good_out_time,stu[i].Good_author,stu[i].press,stu[i].Good_price,stu[i].Good_quantity);
				}	
			}
		}
		else if(cnt=='3')
		{
			printf("请输入你要查询的产品编号：");
			scanf("%d",&num);
			for(i=0;i<k;i++)
			{
				if(stu[i].Good_num==num)
				{
					printf("\n编号：%d,名称：%s,入库时间：%s,生产商：%s,产地：%s,价格%.2f,数量：%d\n",stu[i].Good_num,stu[i].Good_name,stu[i].Good_time,stu[i].Good_author,stu[i].press,stu[i].Good_price,stu[i].Good_quantity);
				}	
			}
		}
		else if(cnt=='4')
		{
			printf("请输入选择项(1.出库记录查看 2.入库记录查看)\n");
			cnt = getch();
			if(cnt=='1')
			{
				for(i=0;i<k;i++)
				{
					if(stu[i].flag==1)
					{
						printf("\n商品名称：%s，商品编号：%d,出库时间:%s\n",stu[i].Good_name,stu[i].Good_num,stu[i].Good_out_time);
					}
				}
			}
			else if(cnt=='2')
			{
				for(i=0;i<k;i++)
				{
					if(stu[i].flag!=1)
					{
						printf("\n商品名称：%s，商品编号：%d,入库时间:%s\n",stu[i].Good_name,stu[i].Good_num,stu[i].Good_time);
					}
				}
			}		
		} 
		else
		{
			puts("输入有误！！！");
		}
	} 	
}
           
