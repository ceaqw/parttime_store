#include <stdio.h>
#define MAX 100


void fuc(int tmp,int row_col[MAX][MAX])
{
	int r = 4*tmp-1;

	int i = 0, n=1;
	while (i < r)		//初始化第零行
	{
		row_col[0][i] = 1;
		i ++;
	}
	
	while(n < (r/2+1))
	{
		i = 0;
		while (i < r)	//获取上一行数据
		{
			row_col[n][i] = row_col[n-1][i];
			i ++;
		}

		if (n%2 == 1)		//设置0
		{
			int temp = n;	
			while(temp < (r-n))
			{
				row_col[n][temp] = 0;
				temp ++;
			}
		}

		if (n%2 == 0)	//设置1
		{
			int temp = n;	
			while(temp < (r-n))
			{
				row_col[n][temp] = 1;
				temp ++;
			}	
		}
		n ++;
	}
}

// 反转对称部分
void reverse(int num ,int row_col[MAX][MAX])
{
	num = 4*num - 1;
	int tmp = num/2-1;
	int n = num/2 + 1;
	int i = 0;

	while (n < num)
	{
		i = 0;
		while (i < num)
		{
			row_col[n][i] = row_col[tmp][i];
			i ++;
		}
		tmp --;
		n ++;
	}
}

// 打印二维数组
void print(int num ,int row_col[MAX][MAX])
{
	num = num*4 - 1;
	for (int r = 0; r < num; ++r)
	{
		for (int c = 0; c < num; ++c)
		{
			printf("%d ", row_col[r][c]);
		}
		printf("\n");
	}
}

int main()
{
	int row_col[MAX][MAX] = {0};

	int cnt = 0;
	printf("请输入盒子数(考虑美观，建议数字不要大于12):");

	scanf("%d", &cnt);

	fuc(cnt, row_col);

	reverse(cnt, row_col);

	print(cnt, row_col);

	return 0;
}