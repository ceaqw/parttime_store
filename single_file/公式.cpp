/******************************
	公式：P_j = √((X_j - X_u)2 + (Y_i - Y_u)2 + (Z_j - Z_u)2)  + CT_u 
*******************************/
#include<stdio.h>
#include<stdlib.h>
#define MAX 1000


float P_j[MAX];	//P_j数组记录 
float S[MAX][3];	//S_j数据储存
float C;
int cnt;		 


void P_j_init()	//P_j,S_j数组的输入 
{
	int i;
	printf("请输入常数C的值：");
	scanf("%f",&C);
	printf("请输入P_j中的数字个数：");
	scanf("%d",&cnt);
	for(i=0;i<cnt;i++)
	{
		printf("请输入第%d个数的值：",i+1);
		scanf("%f",&P_j[i]);
		printf("请输入S_j第%d行的数值(数值间用空格隔开)：",i+1);
		scanf("%f %f %f",&S[i][0],&S[i][1],&S[i][2]);   //输入X_j,Y_j,Z_j的值 
	}
}


void snum(int number)  //求值函数 
{
	float num;		//记录结果数组 
	float flash[4];	//临时储存变量 
	num = rand() /(double)(RAND_MAX/(P_j[number] - 1)); 
	flash[0] = P_j[number] - num;
	flash[0] = flash[0] * flash[0];	//得到根号下的值		
	flash[1] = rand() /(double)(RAND_MAX/flash[0]);		//X_u的方
	flash[2] = flash[0] - flash[1];
	flash[2] = rand() /(double)(RAND_MAX/flash[2]);	//Y_u的方
	flash[3] = flash[0] - flash[1] - flash[2]; 	//Z_u的方 
	printf("输出：u%d = [√%.2f - %.2f,√%.2f - %.2f,√%.2f - %.2f]\n",number+1,flash[1],S[number][0],flash[2],S[number][1],flash[3],S[number][2]); 
 	printf("输出：T_u = %.2f\n",num);
 }



int main()
{
	int i;
	P_j_init(); 	//P_j,S_j数组的输入
	for(i=0;i<cnt;i++)
	{
		snum(i);
	}
	return 0;
}
