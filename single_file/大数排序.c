#include<stdio.h>
#include<string.h>

struct big{				//创建储存数据的结构体 
	char c[1000];		//数据最长为1000位 
	int length;
}buf[100],b;		//最多为100个数 


int main(){
	int n,j,i;
	printf("请输入需要排序的大数据个数："); 
	while(scanf("%d",&n)!=EOF){
		for(i=0;i<n;i++){
			printf("请输入第%d个数：",i+1);
			scanf("%s",buf[i].c);
			buf[i].length=strlen(buf[i].c);
		}
		for(i=0;i<n;i++){					//遍历结构体排序 
			for(j=0;j<n-1-i;j++){
				if(buf[j].length>buf[j+1].length){
					b=buf[j];
					buf[j]=buf[j+1];
					buf[j+1]=b;
				}
				else if(buf[j].length == buf[j+1].length){
					if(strcmp(buf[j].c,buf[j+1].c)>0){
						b=buf[j];
						buf[j]=buf[j+1];
						buf[j+1]=b;
					}
				}
			}
		}
		printf("排序结果为：\n");
		for(i=0;i<n;i++){
			printf("第%d个数：%s\n",i+1,buf[i].c);
		}
	}
	return 0;
}
