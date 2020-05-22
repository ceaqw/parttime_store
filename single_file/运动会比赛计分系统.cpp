/*有问题请咨询技术qq2487689294
	创建日期：2019/1/11/  
	作者：ceaqw*/ 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MIN  20 
#define MAX  1000	


int i_number = 0;
int i_num = 0;

FILE *fp;

struct MS {       //定义结构体
	char sch[MIN];    //学校
	char sor[MIN];  //运动项目 
	char mes[MIN];   //运动员相关信息 
	char res[MIN]; //比赛成绩 
}MSE[MAX];

struct MSG {       //定义结构体       副本，临时转换使用 
	char sch[MIN];    //学校
	char sor[MIN];  //运动项目 
	char mes[MIN];   //运动员相关信息 
	char res[MIN]; //比赛成绩 
}MSEG[MAX];

struct MSG temp;//用于排序的中间变量

void deal(); 

void read_ms() {   //文件读取
	while (!feof(fp)) {
		fscanf(fp, "%s\t%s\t%s\t%s\n",MSE[i_number].sch, MSE[i_number].sor,
		       MSE[i_number].mes, MSE[i_number].res);
		i_number++;
	}
}

void save_ms() {   //信息以文件格式录入函数
	fprintf(fp, "%s\t%s\t%s\t%s\n",MSE[i_number].sch, MSE[i_number].sor,
		       MSE[i_number].mes, MSE[i_number].res);
}

void show() {   //进入系统的主界面显示提息
	puts("\t\t\t1.运动会成员信息输入");
	puts("\t\t\t2.查询成绩");
	puts("\t\t\t3.项目排名查询");
	printf("\t\t\t0. 溜了溜了^_^···\n");
}

void input() {    
	int flag,i;
	puts("***************>>>>>>当前模式为信息录入状态<<<<<<*************");
	printf("请输入运动员相关信息（姓名/出生年/月/日/或其他信息）：");
	flag=1;            
	scanf("%s",&MSE[i_number].mes);
	for(i=0; i<i_number; i++)  {          //判断输入数据的正误，即文件中是否已存在该信息
		if(strcmp(MSE[i].mes,MSE[i_number].mes)==0) {			//利用strcpy(a,b)函数比较字符串a与b是否相等，需调用string.h头文件。下面将多次用到，用法相同不再一一例举 
			flag=0;
		}
		} 
	if(flag==0){ 
		puts("该运动员信息已经存在，如需修改请转到修改命令！");
	} 
	else if(flag==1) {
			printf("请输入学校:");
			scanf("%s",MSE[i_number].sch);
			printf("请输入项目:");
			scanf("%s",MSE[i_number].sor);
			printf("请输入成绩:");
			scanf("%s",MSE[i_number].res);
			save_ms();
			i_number++;
			puts("录入成功"); 
		}
}

void refer(){
	int i,flag=0,k;
	char wqs[MIN];
	k=1;
	puts("***************>>>>>>当前模式为查询信息状态<<<<<<*************");
	while(k) {
		int n,cnt=1;
		puts("请输入查询项：  1.查询学校成绩 2.查询项目成绩结果 3.查询运动员比赛结果");
		scanf("%d",&n);
		if(n==1){
			printf("请键学校名称： ");
			scanf("%s",wqs);
			printf("序号\t学校\t项目\t运动员\t成绩\n");
			for(i=0; i<i_number; i++){			//遍历数组打印 
				if(strcmp(MSE[i].sch, wqs)==0) {
					printf("%d\t",cnt);
					printf("%s\t",MSE[i].sch);
					printf("%s\t",MSE[i].sor);
					printf("%s\t",MSE[i].mes);
					printf("%s\n",MSE[i].res);
					flag=1;
					cnt++;
				}
			} 
		}
		if(n==2){
			printf("请键入运动项目名称： ");
			scanf("%s",wqs);
			printf("序号\t学校\t项目\t运动员\t成绩\n");
			for(i=0; i<i_number; i++){			//遍历数组打印 
				if(strcmp(MSE[i].sor, wqs)==0) {	
					printf("%d\t",cnt);
					printf("%s\t",MSE[i].sch);
					printf("%s\t",MSE[i].sor);
					printf("%s\t",MSE[i].mes);
					printf("%s\n",MSE[i].res);
					cnt++;
					flag=1;
				}
			} 
		}
		if(n==3){
			printf("请键入运动员相关信息：");
			scanf("%s",wqs);
			printf("序号\t学校\t项目\t运动员\t成绩\n");
			for(i=0; i<i_number; i++){			//遍历数组打印 
				if(strcmp(MSE[i].mes, wqs)==0) {	
					printf("%d\t",cnt);
					printf("%s\t",MSE[i].sch);
					printf("%s\t",MSE[i].sor);
					printf("%s\t",MSE[i].mes);
					printf("%s\n",MSE[i].res);
					cnt++;
					flag=1;
				}
		   }
		}
		if(flag==0){ 
			puts("未查找到该信息，如需录入信息请重新键入命令.");
		} 
		puts("请按照文字提示键入命令："); 
		printf("\t\t1.继续查询\n\t\t0.退出查询\n请键入命令:");
		scanf("%d",&k);
	}
} 

void statistics(){
	int i,flag=0,k;
	char wqs[MIN];
	k=1;
	puts("***************>>>>>>当前模式为信息统计状态<<<<<<*************");
	while(k) {
		int n,cnt=1;
		puts("请输入统计项：  1.按学校汇总 2.按比赛项目汇总");
		scanf("%d",&n);
		if(n==1){
			puts("请输入学校名称：");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++){
				if(strcmp(MSE[i].sch,wqs) == 0){
					strcpy(MSEG[i_num].sch,	MSE[i].sch);
					strcpy(MSEG[i_num].sor,MSE[i].sor);
					strcpy(MSEG[i_num].mes,MSE[i].mes);
					strcpy(MSEG[i_num].res,MSE[i].res);
					i_num++;
				} 
			} 
			int r,t;
			for(r=1; r<i_num; r++) {
				for(t=0; t<i_num-r; t++) {
					if(strcmp(MSEG[t+1].res, MSEG[t].res)>0) {  //成绩由高到低
						temp=MSEG[t+1];
						MSEG[t+1]=MSEG[t];
						MSEG[t]=temp;
					}
				}
			}			
			printf("排名\t学校\t项目\t运动员\t成绩\n");
			for(i=0; i<i_num; i++){			//遍历数组打印 
					printf("%d\t",cnt);
					printf("%s\t",MSEG[i].sch);
					printf("%s\t",MSEG[i].sor);
					printf("%s\t",MSEG[i].mes);
					printf("%s\n",MSEG[i].res);
					flag=1;
					cnt++;
			} 
		}
		if(n==2){
			puts("请输入项目名称：");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++){
				if(strcmp(MSE[i].sor,wqs) == 0){
					strcpy(MSEG[i_num].sch,	MSE[i].sch);
					strcpy(MSEG[i_num].sor,MSE[i].sor);
					strcpy(MSEG[i_num].mes,MSE[i].mes);
					strcpy(MSEG[i_num].res,MSE[i].res);
					i_num++;
				} 
			} 
			int r,t;
			for(r=1; r<i_num; r++) {
				for(t=0; t<i_num-r; t++) {
					if(strcmp(MSEG[t+1].res, MSEG[t].res)>0) {  //成绩由高到低
						temp=MSEG[t+1];
						MSEG[t+1]=MSEG[t];
						MSEG[t]=temp;
					}
				}
			}	
			printf("排名\t学校\t项目\t运动员\t成绩\t得分\n");
			int sec = 7;
			for(i=0; i<5; i++){			//遍历数组打印 
				if(strcmp(MSE[i].sor, wqs)==0) {	
					printf("%d\t",cnt);
					printf("%s\t",MSE[i].sch);
					printf("%s\t",MSE[i].sor);
					printf("%s\t",MSE[i].mes);
					printf("%s\t",MSE[i].res);
					printf("%d\n",sec);
					sec-=2;
					cnt++;
					flag=1;
				}
			} 
		}
		if(flag==0){ 
			puts("未查找到该信息，如需录入信息请重新键入命令.");
		} 
		puts("请按照文字提示键入命令："); 
		printf("\t\t1.继续查询\n\t\t0.退出查询\n请键入命令:");
		scanf("%d",&k);
	}
} 

void deal(){    //主要处理函数
	int flag,p;
	p=1;
	flag=1;
	while(flag) {
		int k=0; 
		show();	
		printf("请键入对应数字：");
		scanf("%d",&k);
		
		switch(k) {
			
			case 0: {
				puts("温馨提醒：");
				puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
				puts("\t*******成绩基本信息已保存在当前目录文件 result.txt 下********");
				fclose(fp);
				flag=0;
				break;
			}
			case 1: {
				input();
				break;
			}
			case 2: {
				refer();
				break;
			}
			case 3: {
				statistics();
				break;
			}
			default: {
				printf("请正确键入命令前数字！\n");
				break;
			}
    		puts("\n");
		} 
	}
}

int main() { 
	if ((fp = fopen("result.txt", "r+")) == NULL) {
		fp = fopen("result.txt", "w+");
	}
	read_ms();
	printf(" >************欢 迎 您 使 用 C E A Q W 成 绩 统 计 系 统************<\n");
	printf(">*************     请键入以下命令行所对应的数字进行操作：     *************<\n");
	printf(">*************   请确定正确退出程序,否则文件将不被正常保存.     *************<\n");
	deal();  //处理数据
	return 0;
}
