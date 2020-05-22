#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char a[128]={"0"},b[128]={"0"};

int company(char a[],char b[])		//字符串比较
{
	int i=0,cnt;
	while(a[i]!=b[i] || a[i]=='\0' || b[i]=='\0')
	{
		cnt = a[i] - b[i];
		i++;	
	}	
	return cnt;
} 

void replace(char a[],char b[]) 		//替换
{
	int i,t,cnt[2];
	printf("请输入替换的位置(小于%d)及长度(小于%d)(中间空格隔开)：",strlen(a),strlen(b));
	scanf("%d %d",&cnt[0],&cnt[1]);
	for(i=cnt[0];i<cnt[0]+cnt[1];i++)  a[i] = b[i];		//替换对应位置的字符 
	printf("替换结果：%s\n",a);
} 

void del_str(char a[],char b[])		//删除字符串
{
	int i,t=0,flag=0;
	for(i=0;i<strlen(a);i++)
	{
		t = i;
		while(a[t] == b[t-i])		//检测到相
		{
			t++;
			if(b[t-i] == '\0') 		//字符串对照完毕 
			{
				flag = t-i;
				for(t=i;t<strlen(a)-i;t++)
				{
					a[t] = a[strlen(b)+t];
					if(t==flag) a[t]='\0'; break;		//删除完毕 
				}
			}
		}
		t = 0;
		if(flag)
		{
			puts("删除成功！！");
			break;	
		}	
	}	
	if(!flag)
	{
		puts("未找到！！");
	}
} 

int find_str(char a[],char b[])		//搜索字符串
{
	int i,t=0;
	for(i=0;i<strlen(a);i++)
	{
		t = i;
		while(a[t] == b[t-i])		//检测到相
		{
			t++;
			if(b[t-i] == '\0') 		//字符串对照完毕 
			{
				return i;
			}
		}	
	}	
	return 0;
} 

void in_str(char a[],char b[])		//插入字符串
{
	int i,cnt;
	printf("请输入你要插入的位置(小于%d):",strlen(a));
	scanf("%d",&cnt);
	for(i=cnt+strlen(b);i<strlen(b)+strlen(a);i++)
	{
		a[i] = a[i-strlen(b)]; 
	}
	a[i++] = '\0';
	for(i=cnt;i<cnt+strlen(b);i++)		//插入完成 
	{
		a[i] = b[i-cnt];
	}
	printf("插入结果：%s\n",a);
} 

void change(char a[])			//数字转字符
{
	int num;
	printf("请输入数字(小于65536)：");
	scanf("%d",&num);
	itoa(num,a,10);	
	printf("转换结果：%s\n",a);
} 

void encrypt(char a[],char b[])		//加密
{
	int num=57864392,i;
	for(i=0;i<strlen(a);i++) a[i] += num;			//循环加密 
	for(i=0;i<strlen(b);i++) a[i] += num;
	printf("加密结果%s\n",a);
	printf("加密结果%s\n",b);
} 

void decode(char a[],char b[])		//解密
{
	int num=57864392,i;
	for(i=0;i<strlen(a);i++) a[i] -= num;		//循环解密 
	for(i=0;i<strlen(b);i++) a[i] -= num;
	printf("加密结果%s\n",a);
	printf("加密结果%s\n",b);
} 

void meau()		//菜单
{
	puts("请选择：");
	puts("\t0.输入字符串");
	puts("\t1.字符串比较\t");
	puts("\t2.字符串替换\t");
	puts("\t3.删除字符串\t");
	puts("\t4.搜索子字符串位置\t");
	puts("\t5.插入字符串\t");
	puts("\t6.数值转换为字符串\t");
	puts("\t7.字符串加密\t");
	puts("\t8.字符串解密\t");
	puts("\t9.退出\t"); 
	printf("请输入："); 
} 

int main()
{
	int cnt;
	while(1)
	{
		meau();
		scanf("%d",&cnt);
		if(cnt==0)
		{
			printf("请输入两个字符串(中间空格隔开):");
			scanf("%s %s",a,b); 
		}
		else if(cnt==1)
		{
			printf("比较结果为：%d\n",company(a,b));
		}
		else if(cnt==2)
		{
			replace(a,b); 
		}
		else if(cnt==3)
		{
			del_str(a,b);
		}
		else if(cnt==4)
		{
			printf("搜索结果为：%s\n",find_str(a,b));
		}
		else if(cnt==5)
		{
			in_str(a,b); 
		}
		else if(cnt==6)
		{
			change(a); 
		}
		else if(cnt==7)
		{
			encrypt(a,b);
		}
		else if(cnt==8)
		{
			decode(a,b);
		}
		else if(cnt==9)
		{
			break;
		}
		else
		{
			puts("输入有误！！");
		}
	}
	
	return 0;	
} 
