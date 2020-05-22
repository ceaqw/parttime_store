/*如对您的要求有误或其他问题
请咨询qq:2487689294
谢谢合作
date：2019/02/24*/ 

#include <stdio.h>
#include <string.h>
#define MIN 5
#define MAX 20

int number,flash = 1;

union marritalState{
	char marring[MIN];
	int marryFlag;//婚姻状况标志，1.表示未婚 2.表示已婚 3.表示离婚
}; 

struct LNode{      //储存用户信息 
	char name[MIN];
	char sex[MIN];
	char age[MIN];
	union marritalState marrital;
	char date[MAX];
	char mname[MIN];
	char num_child[MIN]; 
} LNode[MAX];

void msg_input(){     //信息录入函数 
	int tep,i;
	puts("\t···信息录入界面···\t\n");
	puts("请输入录入信息人数：");
	scanf("%d",&tep);
	for(i=0;i<tep;i++){
		puts("姓名：");
		scanf("%s",LNode[i].name); 
		puts("性别：");
		scanf("%s",LNode[i].sex);
		puts("年龄：");
		scanf("%s",LNode[i].age);
		puts("婚姻状态，请输入：");
		puts("1.未婚"); 
		puts("2.已婚"); 
		puts("3.离婚"); 
		scanf("%d",&LNode[i].marrital.marryFlag);
		if(LNode[i].marrital.marryFlag == 1){
			strcpy(LNode[i].marrital.marring,"未婚");
		}
		else if(LNode[i].marrital.marryFlag == 2){
			strcpy(LNode[i].marrital.marring,"已婚");	
		}
		else if(LNode[i].marrital.marryFlag == 3){
			strcpy(LNode[i].marrital.marring,"离婚");	
		}
		puts("结婚/离婚日期：");
		scanf("%s",LNode[i].date);
		puts("配偶姓名：");
		scanf("%s",LNode[i].mname);
		puts("孩子数：");
		scanf("%s",LNode[i].num_child);
		puts("录入成功");
		number++; 
	}	
}

void msg_up_input(){     //信息添加函数 
	puts("\t···信息添加录入界面···\t\n");
	puts("姓名：");
	scanf("%s",LNode[number].name); 
	puts("性别：");
	scanf("%s",LNode[number].sex);
	puts("年龄：");
	scanf("%s",LNode[number].age);
	puts("婚姻状态，请输入：");
		puts("1.未婚"); 
		puts("2.已婚"); 
		puts("3.离婚"); 
		scanf("%d",&LNode[number].marrital.marryFlag);
		if(LNode[number].marrital.marryFlag == 1){
			strcpy(LNode[number].marrital.marring,"未婚");
		}
		else if(LNode[number].marrital.marryFlag == 2){
			strcpy(LNode[number].marrital.marring,"已婚");	
		}
		else if(LNode[number].marrital.marryFlag == 3){
			strcpy(LNode[number].marrital.marring,"离婚");	
		}
	puts("结婚/离婚日期：");
	scanf("%s",LNode[number].date);
	puts("配偶姓名：");
	scanf("%s",LNode[number].mname);
	puts("孩子数：");
	scanf("%s",LNode[number].num_child);
	puts("录入成功");
	number++; 
}

void msg_output(){     //信息输出浏览函数 
	int i,cnt=1;
	puts("\t···信息浏览界面···\t\n");
	printf("\t\t\t\t\t··员工婚姻信息表··\t\t\t\t\t\n");
	printf("员工编号\t姓名\t\t性别\t\t年龄\t\t婚姻状态\t\t结婚/离婚日期\t\t配偶姓名\t孩子数\n");	 
	for(i=0;i<number;i++){
		printf(" %d\t\t",cnt);
		printf("%s\t\t%s\t\t%s\t\t",LNode[i].name,LNode[i].sex,LNode[i].age);
		printf("%s\t\t\t%s\t\t%s\t\t%s\n",LNode[i].marrital.marring,LNode[i].date,LNode[i].mname,LNode[i].num_child);
		cnt++;
	}
}

void meau(){     //主菜单函数 
	int tep; 
	printf("\t·····欢迎使用·····\t\n");
	printf("\t··请输入下列选项的数字··\t\n");
	printf("		1.信息录入：\n");
	printf("		2.成员添加：\n"); 
	printf("		3.信息浏览：\n");
	printf("		4.  退出    \n");
	scanf("%d",&tep);
	if(tep == 1){
		msg_input();
	}
	else if(tep == 2){
		msg_up_input();
	}
	else if(tep == 3){
		msg_output();
	}
	else if(tep == 4){
		puts("·····谢谢使用·····\n");
		flash = 2;
	} 
	else{
		printf("错误代码，请重新输入："); 
	} 
} 

int main(){
	while(flash == 1){    
		meau();
	}
}
