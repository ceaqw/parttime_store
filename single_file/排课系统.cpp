/*有问题请咨询技术qq2487689294
	创建日期：2019/1/7/  
	作者：ceaqw*/ 

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MIN  20 
#define MAX  1000

int i_number;
int cnt;

struct LNode {       //定义结构体
	char num[MIN];  // 课程编号 
	char gong[MIN];    //教工号 
	char time[MIN];   //课程学期 
}LNode[MAX];

void deal();
void save(){
	FILE*fp;
	int n,i;
	printf("是否另存文件(文件命名格式：英文.txt或其他后缀)：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1){
		char fp_name[MIN];
		printf("请输入保存文件名称：");
		scanf("%s",fp_name);
		fp = fopen(fp_name,"w+");
		fprintf(fp,"记录编号\t课程编号\t教师工号\t上课时间\n"); 
		for(i=0; i<i_number; i++){
			cnt = 1;
		    fprintf(fp, "%d\t%s\t%s\t%s\t%s\n", cnt, LNode[i].num,LNode[i].gong,LNode[i].time);
			cnt++;			
		}
		fclose(fp);
		puts("保存成功"); 
	}
	if(n==2){
		deal();
	}
		
} 

void notice() {//进入系统的主界面显示提息
	puts("\t\t\t1.排课记录添加");
	puts("\t\t\t2.任课教师信息查询");
	puts("\t\t\t3.教师认课情况统计");
	puts("\t\t\t4.排课记录删除");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void find_g() {
	int i,flag=0,k;
	char wqs[MIN];
	k=1;
	system("cls");
	puts("***************>>>>>>当前模式教工查询信息状态<<<<<<*************");
	while(k) {
		int n;
		puts("请输入查询项：  1.输入课程编号查询综合信息  2.输入学期查询有关课程。") ;
		scanf("%d",&n);
		if(n==1){
			printf("请键入课程编号： ");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++)			//遍历数组打印 
				if(strcmp(LNode[i].num, wqs)==0) {
					cnt = 1; 
					printf("序号\t课程编号\t教工号\t上课时间\n");
					printf("%d\t",cnt);
					printf("%s\t",LNode[i].num);
					printf("%s\t",LNode[i].gong);
					printf("%s\n",LNode[i].time);
					cnt++;
					flag=1;
				}
		   }
		if(n==2){
			printf("请键入上课时间： ");
			scanf("%s",wqs);
			for(i=0; i<i_number; i++)			//遍历数组打印 
				if(strcmp(LNode[i].time, wqs)==0) {
					cnt = 1; 
					printf("序号\t课程编号\t上课时间\n");
					printf("%d\t",cnt);
					printf("%s\t",LNode[i].num);
					printf("%s\n",LNode[i].time);
					cnt++;
					flag=1;
				}
		}
		if(flag==0)
			puts("未查找到该课程信息，如需录入信息请重新键入命令.");
		puts("请按照文字提示键入命令："); 
		printf("\t\t1.继续查询\n\t\t0.退出查询\n请键入命令:");
		scanf("%d",&k);
	}
}

void find_t() {
	int i,flag=0,k;
	char wqs[MIN];
	k=1;
	system("cls");
	puts("***************>>>>>>当前模式教工任课信息统计状态<<<<<<*************");
	while(k) {
		printf("请键入教工号： ");
		scanf("%s",wqs);
		for(i=0; i<i_number; i++)			//遍历数组打印 
			cnt =1;
			if(strcmp(LNode[i].num,wqs)==0) {
				printf("%d",cnt);
				printf("%-16s",LNode[i].num);
				printf("%-4s\n",LNode[i].gong);
				printf("%-4s\n",LNode[i].time);
				cnt++;
				flag=1;
			}
		if(flag==0)
			puts("未查找到该课程信息，如需录入信息请重新键入命令.");
		puts("请按照文字提示键入命令：");
		printf("\t\t1.继续查询\n\t\t0.退出查询\n请键入命令:");
		scanf("%d",&k);
	}
}

void case1() {    
	int flag,i;
	system("cls");
	puts("***************>>>>>>当前模式为课程信息录入状态<<<<<<*************");
	printf("请输入课程编号：");
	flag=1;            //标识变量，用以判断该信息是否已存在
	scanf("%s",&LNode[i_number].num);
	for(i=0; i<i_number; i++)  {          //判断输入数据的正误，即文件中是否已存在该信息
		if(strcmp(LNode[i].num,LNode[i_number].num)==0) {			//利用strcpy(a,b)函数比较字符串a与b是否相等，需调用string.h头文件。下面将多次用到，用法相同不再一一例举 
			flag=0;
			break;
		}
		} 
	if(flag==0)
		puts("该课程已经存在，如需修改请转到修改命令！");
	else if(flag==1) {
	//		scanf("%s",LNode[i_number].num);
			printf("请输入教工号:");
			scanf("%s",LNode[i_number].gong);
			printf("请输入上课时间:");
			scanf("%s",LNode[i_number].time);
			i_number++;
		}
	puts("录入成功"); 
}

void case4(){
	int i,j,flag=0,k;
	char wqs[MIN];
	k=1;
	system("cls");
	while(k) {
		puts("请输入需要删除的课程号："); 
		scanf("%s",wqs);
		for(i=0; i<i_number; i++)
			if(strcmp(LNode[i].num,wqs)==0) {			//strcpy函数调用 
				LNode[i] = LNode[i++];					//运用指针移位，指向下一个指针地址从而达到删除效果 
				i_number--;							//成员减一 
				puts("删除完成"); 
				flag=1;
			}
		if(flag==0)
			puts("未查找到该课程信息，如需录入信息请重新键入命令.");
		puts("请按照文字提示键入命令：");
		printf("\t\t1.继续删除\n\t\t0.退出删除\n请键入命令:");
		scanf("%d",&k);
	}
	save();
} 

void deal() {//主要处理函数
	int k,flag,p;
	p=1;
	notice();
	flag=1;
	while(flag) {
		if(p>1)
			notice();
			scanf("%d",&k);
			system("cls");
			switch(k) {
				case 0: {
					puts("温馨提醒：");
					puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
					flag=0;
					break;
				}
				case 1: {
					case1();
					break;
				}
				case 2: {
					find_g();
					break;
				}
				case 3: {
					find_t();
					break;
				}
				case 4: {
					case4();
					break;
				}
				default: {
					printf("请正确键入命令前数字！\n");
					break;
				}
		}
		p=1103;
		puts("\n");
	}
}
int main() {
	printf(" >************欢 迎 您 使 用 课 程 信 息 管 理 系 统************<\n");
	printf(">*************     请键入以下命令行所对应的数字进行操作：     *************<\n");
	printf(">*************   请确定正确退出程序,否则文件将不被正常保存.     *************<\n");
	deal();//处理数据
	return 0;
}
