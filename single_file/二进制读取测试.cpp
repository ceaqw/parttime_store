#include <iostream>
#include <stdio.h>
#include <stdlib.h>
int main() 
{
 FILE *f = fopen("1-0.dat", "rb");//二进制读取(无文件时自动创建)
 int nVal ,i;
 char str[32]; //一次读取四个字节也就是32位 
 for(i=0;i<1000000;i++)  //调用读取次数 
 {
 	fread(&nVal, 1, sizeof(int), f);//读文件内容（一次读取四个字节也就是32位） 
 	itoa(nVal,str,2);		//转换为二进制字符串储存在字符串中以便后期使用 
 	printf("%s", str);		//打印预览 
 }
 fclose(f);
 return 0;
}
