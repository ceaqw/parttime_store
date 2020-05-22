#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<Windows.h>

int row, col;  //row表示行，col表示列 
int k = 0;     //k表示关卡 
int dest[8][9] = { 0 };
int map[2][8][9] = {      
 {     
{ 0, 0, 0, 1, 1, 1, 0, 0, 0 },
{ 0, 0, 0, 1, 4, 1, 0, 0, 0 },
{ 1, 1, 1, 1, 2, 1, 0, 0, 0 },
{ 1, 4, 0, 2, 3, 1, 1, 1, 0 },
{ 1, 1, 1, 2, 0, 2, 4, 1, 0 },
{ 0, 0, 1, 0, 1, 1, 1, 1, 0 },
{ 0, 0, 1, 4, 1, 1, 1, 1, 0 },
{ 0, 0, 1, 1, 1, 0, 0, 0, 0 }
	},
	
{
{ 0, 1, 1, 1, 1, 1, 1, 1, 0 },
{ 0, 1, 0, 0, 4, 4, 4, 1, 0 },
{ 0, 1, 0, 3, 0, 1, 1, 1, 1 },
{ 1, 1, 1, 2, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 1, 2, 1, 0, 1 },
{ 1, 0, 2, 0, 1, 0, 0, 0, 1 },
{ 1, 0, 0, 0, 1, 1, 1, 1, 1 },
{ 1, 1, 1, 1, 1, 0, 0, 0, 0 }
} 
   };
 
void init()
{
	printf("*************************\n");
	printf("推箱子小游戏\n");
	printf("基础操作wsad\n");
	printf("按Esc退出\n");
	printf("*************************\n");
}
void showMap()     //此函数用来打印地图
{
	init();//操作介绍
		   //遍历数组，打印形状
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 9; ++j)
			switch (map[k][i][j])
			{
			case 0:
				printf("  ");//空地 0
				break;
			case 1:
				printf("■");//墙 1
				break;
			case 2:
				printf("○");//箱子 2
				break;
			case 3:
				printf("♀");//人 3
				break;
			case 4:
				printf("□");//目的地 4
				break;
			case 6:
				printf("●");//箱子+目的地 2+4=6
				break;
			case 7:
				printf("♀"); //人+目的地 3+4=7
				break;
			default:
				break;
			}
		printf("\n");
	}
}
//游戏规则
//向上
void playGameUp()
{
	 
	//如果向上是墙1，不可以走
	if (map[k][row - 1][col] == 1)
		return;
	//如果向上是箱子，并且箱子上面是墙或者是箱子，不可以走
	if (map[k][row - 1][col] == 2 && (map[k][row - 2][col] == 1 || map[k][row - 2][col] == 2))
		return;
		
	//（1）如果向上走是空地0，可以走
	if (map[k][row - 1][col] == 0 || map[k][row - 1][col] == 4)
	{
		map[k][row][col] -= 3;
		map[k][row - 1][col] += 3;
	}
	//(3)如果向上是箱子（箱子包含（空地+箱子）或者（目的地+箱子）），并且箱子上面是(3)空地或者(4)目的地，人和箱子同时走
	if ((map[k][row - 1][col] == 2 || map[k][row - 1][col] == 6) && (map[k][row - 2][col] == 0 || map[k][row - 2][col] == 4))
	{
		map[k][row][col] -= 3;
		map[k][row - 1][col] += 1;
		map[k][row - 2][col] += 2;
	}
	row--;
 
}
//接下来，向下走，左走，右走，代码基本差不多
//向下走
void playGameDown()
{
	//如果向下是墙，不可以走
	if (map[k][row + 1][col] == 1)
		return;
	//如果向下是箱子，并且箱子下面是墙或者是箱子，不可以走
	if (map[k][row + 1][col] == 2 && (map[k][row + 2][col] == 1 || map[k][row + 2][col] == 2))
		return;
	//（1）如果向下走是空地0，可以走 （2）如果向下走是目的地
	if (map[k][row + 1][col] == 0 || map[k][row + 1][col] == 4)
	{
		map[k][row][col] -= 3;
		map[k][row + 1][col] += 3;
	}
 
	//(3)如果向下是箱子（箱子包含（空地+箱子）或者（目的地+箱子）），并且箱子下面是(3)空地或者(4)目的地，人和箱子同时走
	if ((map[k][row + 1][col] == 2 || map[k][row + 1][col] == 6) && (map[k][row + 2][col] == 0 || map[k][row + 2][col] == 4))
	{
		map[k][row][col] -= 3;
		map[k][row + 1][col] += 1;
		map[k][row + 2][col] += 2;
	}
	row++;
}
//向左走
void playGameLeft()
{
	//如果向左是墙，不可以走
	if (map[k][row][col - 1] == 1)
		return;
	//如果向左是箱子，并且箱子左面是墙或者是箱子，不可以走
	if (map[k][row][col - 1] == 2 && (map[k][row][col - 2] == 1 || map[k][row][col - 2] == 2))
		return;
	//（1）如果向左走是空地0，可以走（2）如果向左走是目的地
	if (map[k][row][col - 1] == 0 || map[k][row][col - 1] == 4)
	{
		map[k][row][col] -= 3;
		map[k][row][col - 1] += 3;
	}
 
	//(3)如果向左是箱子（箱子包含（空地+箱子）或者（目的地+箱子）），并且箱子左面是(3)空地或者(4)目的地，人和箱子同时走
	if ((map[k][row][col - 1] == 2 || map[k][row][col - 1] == 6) && (map[k][row][col - 2] == 0 || map[k][row][col - 2] == 4))
	{
		map[k][row][col] -= 3;
		map[k][row][col - 1] += 1;
		map[k][row][col - 2] += 2;
	}
	col--;
}
void playGameRight()
{
	//如果向右是墙1，不可以走
	if (map[k][row][col + 1] == 1)  //此处检测如果下一个位置是1，即墙，此函数就什么也不执行
		return;
	//如果向右是箱子，但是箱子右面是墙或者是箱子，不可以走
	if (map[k][row][col + 1] == 2 && (map[k][row][col + 2] == 1 || map[k][row][col + 2] == 2)) //如果下一个位置是箱子，但是箱子下一个位置是墙或者是箱子，此函数就什么也不执行
		return;
	//（1）如果向右走是空地0，可以走（2）如果右走是目的地
	if (map[k][row][col + 1] == 0 || map[k][row][col + 1] == 4)  //此处检测下一个位置是空地或目的地，即0 4，地图显示更新
	{
		map[k][row][col] -= 3;    //当前人的位置3 - 3 = 0，即空地
		map[k][row][col + 1] += 3; //下一个位置显示人即0 + 3 = 3，即人 或者 4（目的地）+ 3 = 7（人+目的地）
	}
 
	//(3)如果向右是箱子（箱子包含（空地+箱子）或者（目的地+箱子）），并且箱子右面是(3)空地或者(4)目的地，人和箱子同时走
	if ((map[k][row][col + 1] == 2 || map[k][row][col + 1] == 6) && (map[k][row][col + 2] == 0 || map[k][row][col + 2] == 4))//当（下一个位置是箱子或者箱子+目的地） 并且（下下个位置是空地或者目的地）时执行此函数
	{
		map[k][row][col] -= 3;   //当前人的位置3（人） - 3 = 0，即空地
		map[k][row][col + 1] += 1;//下一个位置显示 2（箱子）+ 1 = 3（人）或者 6（箱子+目的地）+ 1 =7（人+目的地）
		map[k][row][col + 2] += 2;//下下个位置显示 0（空地）+ 2 = 2（箱子） 或者 4（目的地） + 2 = 6（箱子+目的地）
	}
	col++;
}
void getOpCh()   //此函数用来检测用户所需要执行的动作，进而执行上下左右命令
{
	char op;
	op = _getche();   //获取用户键入的字符
 
	switch (op)
	{
	case 'w':
		playGameUp();
		break;
	case 's':
		playGameDown();
		break;
	case 'a':
		playGameLeft();
		break;
	case 'd':
		playGameRight();
		break;
 	case 27 :
 		printf("游戏退出。。。\n");
 		system("pause");
 		exit(0);
	default:
		break;
	}
}
 
int main()
{
 
 
	while (1)
	{
	    //假设没有全部通关
		system("cls");
		showMap();
		//获取人的位置
		for (int i = 0; i < 8; ++i)   //遍历数组，找到 3（人）的位置
			for (int j = 0; j < 9; ++j)
			{
				if (map[k][i][j] == 3)
				{
					row = i;
					col = j;
					break;
				}
			}
		//获取目的的坐标点
		for (int i = 0; i < 8; ++i)
			for (int j = 0; j < 9; ++j)
				if (map[k][i][j] == 4)
					dest[i][j] = 4;
		while (1)
		{
			//进入手动推箱子
			getOpCh();   //调用上下左右函数
			system("cls");
			showMap();   //刷新地图显示
			//判断一下是否过关
			bool next = true;
			for (int i = 0; i < 8; ++i)
				for (int j = 0; j < 9; ++j)
				{
					if (dest[i][j])
					{
						if (map[k][i][j] != 6)
							next = false;
					}
				}
 
			//判断一下,是否过关
			if (next)
			{
				k++;
				memset(dest, 0, sizeof(dest));   //string函数，memset(a,b,c)将字符b赋值给a的前c个字符
				//关卡只有2个，当k==2，表示全部通关
				if (k == 2)
				{
					system("cls");  //windows库函数，清屏
					printf("你已全部通关");
					system("pause"); //windows库函数，暂停此进程，可按任意键继续进程
					exit(0); //全部通关  stdlib库函数，结束当前进程
				}
				else
				{
					system("cls");    //windows库函数，清屏
					printf("你已通关\n");
					system("pause");   //windows库函数，暂停此进程，可按任意键继续进程
					break;
 
				}
			} 
		}
	}
 
	return 0;
}

