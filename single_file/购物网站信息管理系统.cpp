/*有问题请咨询技术qq2487689294
	创建日期：2019/1/7/  
	作者：ceaqw*/ 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define MIN_C 50   

int i_number;
FILE *fp;
void deal();
struct things {
	char name[MIN_C];  //商品名称 
	char price[MIN_C];//商品价格 
	char volume[MIN_C];//商品销量 
	char cerdit[MIN_C];	//商品信誉 
} DATA[MIN_C];

struct things temp;//用于排序的中间变量

void save(){
	int n,i;
	FILE *fp;
	printf("是否另存文件(文件命名格式：英文.txt或其他后缀)：1.是   2.否(将返回主菜单)\n") ;
	scanf("%d",&n);
	if(n==1){
		char fp_name[MIN_C];
		printf("请输入保存文件名称：");
		scanf("%s",fp_name);
		fp = fopen(fp_name,"w+");
		fprintf(fp,"名称\t价格\t销量\t信誉"); 
		for(i=0; i<i_number; i++){
		    fprintf(fp, "%s\t%s\t%s\t%s\n", DATA[i].name, DATA[i].price,DATA[i].volume,DATA[i].cerdit);			
		}
		fclose(fp);
		puts("保存成功"); 
	}
	if(n==2){
		deal();
	}
		
} 

void notice() {//进入系统的主界面显示提息
	puts("\t\t\t1.商品信息录入");
	puts("\t\t\t2.商品信息查询");
	puts("\t\t\t3.商品信息修改");
	puts("\t\t\t4.商品信息删除");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void find() {//商品查找 利用strcmp()函数
	int i,flag=0,k;
	char wqs[MIN_C];
	k=1;
	system("cls");
	puts("***************>>>>>>当前模式商品查询信息状态<<<<<<*************");
	while(k) {
		printf("请键入商品名称： ");
		scanf("%s",wqs);
		for(i=0; i<i_number; i++)			//遍历数组打印 
			if(strcmp(DATA[i].name,wqs)==0) {
				printf("%-18s",DATA[i].name);
				printf("%-16s",DATA[i].price);
				printf("%-4s",DATA[i].volume);
				printf("%-6s\n",DATA[i].cerdit);
				flag=1;
			}
		if(flag==0)
			puts("未查找到该商品信息，如需录入信息请重新键入命令.");
		puts("请按照文字提示键入命令：");
		printf("\t\t1.继续查询\n\t\t0.退出查询\n请键入命令:");
		scanf("%d",&k);
	}
}

int case1() {     //商品信息的录入
	int flag,i;
	system("cls");
	puts("***************>>>>>>当前模式为商品信息录入状态<<<<<<*************");
	printf("请输入商品名称：");
	flag=1;            //标识变量，用以判断该信息是否已存在
	scanf("%s",&DATA[i_number].name);
	for(i=0; i<i_number; i++)  {          //判断输入数据的正误，即文件中是否已存在该信息
		if(strcmp(DATA[i].name,DATA[i_number].name)==0) {			//利用strcpy(a,b)函数比较字符串a与b是否相等，需调用string.h头文件。下面将多次用到，用法相同不再一一例举 
			flag=0;
			break;
		}
		} 
	if(flag==0)
		puts("该商品已经存在，如需修改请转到修改命令！");
	else if(flag==1) {
			printf("请输入商品价格：");
			scanf("%s",DATA[i_number].price);
			printf("请输入商品销量：");
			scanf("%s",DATA[i_number].volume);
			printf("请输入商品信誉：");
			scanf("%s",DATA[i_number].cerdit);
			i_number++;
		}
	save();
	return i_number;
}

void sort() {      //销量排序 
	int r,t;
	system("cls");
	puts("***************>>>>>>当前模式以学号排序后浏览状态<<<<<<*************");
	for(r=1; r<i_number; r++) {
		for(t=0; t<i_number; t++) {
			if(strcmp(DATA[t+1].cerdit, DATA[t].cerdit)<0) {//销量由低到高
				temp=DATA[t+1];
				DATA[t+1]=DATA[t];
				DATA[t]=temp;
			}
		}
	}
}

void sort1() {      //销量从大到小排序 
	int r,t;
	system("cls");
	puts("***************>>>>>>当前模式以学号排序后浏览状态<<<<<<*************");
	for(r=1; r<i_number; r++) {
		for(t=0; t<i_number; t++) {
			if(strcmp(DATA[t+1].cerdit, DATA[t].cerdit)>0) {//销量由低到高
				temp=DATA[t+1];
				DATA[t+1]=DATA[t];
				DATA[t]=temp;
			}
		}
	}
}

void case2(){
	int v;
	printf("提示：1,输入名称查询。 0.返回上一层请键入：");
	scanf("%d",&v);
	if(v==1) {
		int i;
		printf("提示：1.销量由高到低查看。2.信誉由高到低查看 3.查询单个商品信息 0.返回上一层请键入：");
		scanf("%d",&i);
		if(i==1){
			int n;
			sort();
			for(n=1;n<=i_number;n++){
				printf("%d   ",n);
				printf("%-18s",DATA[i].name);
				printf("%-16s",DATA[i].price);
				printf("%-4s",DATA[i].volume);
				printf("%-6s",DATA[i].cerdit);
			}
			save();
		} 
		else if(i==2){
			int n;
			sort1();
			for(n=1;n<=i_number;n++){
				printf("%d   ",n);
				printf("%-18s",DATA[i].name);
				printf("%-16s",DATA[i].price);
				printf("%-4s",DATA[i].volume);
				printf("%-6s",DATA[i].cerdit);
			}
			save();
		}
		else if(i==3){
			find();
			save();
		}
		else {
			case2();
		}
	}
	if(v==0){
		deal();					//返回主菜单 
	}	
}

void case3(){
	int flag = 0,i,t;
	system("cls");
	puts("***************>>>>>>当前模式为商品信息修改状态<<<<<<*************");
	printf("选定修改项：1.修改销量信息.    0.返回上一层   请键入：");
	scanf("%d",&t);
		if(t==1) {
			char bj[MIN_C],num[MIN_C];   
			printf("请输入需要修改的商品名称：");
			scanf("%s",bj);
			printf("请输入修改后的商品销量");
			scanf("%s",num);
			for(i=0; i<i_number; i++) //判断输入数据的正误
				if(strcmp(DATA[i].name,bj)==0) {		//strcpy库函数调用 
					strcpy(num,DATA[i].cerdit);
					flag=1;
					break;
				}
				printf("修改完成"); 
				save();
			if(flag==0)
				puts("该商品不存在，请检查输入是否有误！");
		}
		if(t==0){
			deal();
		}	
}

void case4(){
	int i,j,flag=0,k;
	char wqs[MIN_C];
	k=1;
	system("cls");
	while(k) {
		puts("请输入需要删除的商品名称："); 
		scanf("%s",wqs);
		for(i=0; i<i_number; i++)
			if(strcmp(DATA[i].name,wqs)==0) {			//strcpy函数调用 
				DATA[i] = DATA[i++];					//运用指针移位，指向下一个指针地址从而达到删除效果 
				i_number --;							//成员减一 
				puts("删除完成"); 
				flag=1;
			}
		save(); 
		if(flag==0)
			puts("未查找到该同学信息，如需录入信息请重新键入命令.");
		puts("请按照文字提示键入命令：");
		printf("\t\t1.继续删除\n\t\t0.退出删除\n请键入命令:");
		scanf("%d",&k);
	}
} 

void deal() {   //主要处理函数
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
					puts("\t*******商品基本信息已保存在当前目录文件自定义文件中********");
					save();
					flag=0;
					break;
				}
				case 1: {
					case1();
					break;
				}
				case 2: {
					case2();
					break;
				}
				case 3: {
					case3();
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

	printf(" >************欢 迎 您 使 用 商 品 信 息 管 理 系 统(正式版1.0.1)************<\n");
	printf(">*************     请键入以下命令行所对应的数字进行操作：     *************<\n");
	printf(">*************   请确定正确退出程序,否则文件将不被正常保存.     *************<\n");
	deal(); 
	return 0;
}

