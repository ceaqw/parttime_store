#include <stdio.h>
#include <string.h>   

int name[2][2][2][2];  	//定义全局变量数组来储存四个物体的状态 

int dfs(int p, int v, int s, int w)  //判断物体状态的函数p,v,s,w分别代表人，菜，羊，狼在数组中的位置 
{ 
	if(name[p][v][s][w]>=0)        //如果此位置的值大于等于0表示已过河 
		return name[p][v][s][w];
	name[p][v][s][w] = 0;  //初始化此位置数据表示未过河 
	if(v==s && s!=p)	//菜和羊在一边并且人和羊不在一边返回0 
		return 0;
	if(w==s&&w!=p)    //未羊和狼在一边并且狼和人不在一边返回0 
		return 0;
	if(p==0&&v==0&&s==0&&w==0)  //没有物品需要带，即都过河返回1 （0表示过河，1表示未过河） 
		return 1;
	if(dfs(1-p,v,s,w))  // 人自己过河，我i他对象已过河 
	{ 
		puts("人 自己到对岸");
		name[p][v][s][w] = 1;
		return 1;
	}
	if(v==p && dfs(1-p,1-v,s,w)) // 判断人和菜能否过河 ，即人和菜在一块 （狼和羊不同是在一边）。 
	{ 
		puts("人 和 菜 到对岸");
		name[p][v][s][w] = 1;
		return 1;
	} 
	if(w==p && dfs(1-p,v,s,1-w))	// 判断人和狼能否过河 ，即人和狼在一块 （菜和羊不同是在一边）。 
	{ 
		puts("人 和 狼 到对岸");
		name[p][v][s][w] = 1;
		return 1;
	}
	if(s==p&&dfs(1-p,v,1-s,w))	// 判断人和羊能否过河 ，即人和羊在一块 （菜和狼可在同一边）。 
	{ 
		puts("人 和 羊 到对岸");
		name[p][v][s][w] = 1;
		return 1;
	} 
	return 0;
} 
int main()
{ 
	memset(name,-1,sizeof(name)); //将数组初始化位-1 
	dfs(1,1,1,1); //调用算法函数 初始位置1都未过河 
	return 0;
}
