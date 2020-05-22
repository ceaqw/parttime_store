#include<stdio.h>
#include<stdlib.h> 
#include<math.h>
#define N 10

void calc_ulate(int n,float r[N],float s[N][3],float answer[4],float ps[N],float g1[N][4]);
 
float calc_A(float g3[N][N], int n)//按第一行展开计算|A|
{
    float ans = 0;
    float temp[N][N];
    int i, j, k;
	if (n == 1)
    {
        return g3[0][0];
    }
    for (i = 0; i<n; i++)
    {
        for (j = 0; j<n - 1; j++)
        {
            for (k = 0; k<n - 1; k++)
            {
                temp[j][k] = g3[j + 1][(k >= i) ? k + 1 : k];

            }
        }
        float t = calc_A(temp, n - 1);
        if (i % 2 == 0)
        {
            ans += g3[0][i] * t;
        }
        else
        {
            ans -= g3[0][i] * t;
        }
    }
    return ans;
}

void getAStart(float g3[N][N], int n, float ans[N][N])//计算每一行每一列的每个元素所对应的余子式，组成A*
{
    if (n == 1)
    {
        ans[0][0] = 1;
        return;
    }
    int i, j, k, t;
    float temp[N][N];
    for (i = 0; i<n; i++)
    {
        for (j = 0; j<n; j++)
        {
            for (k = 0; k<n - 1; k++)
            {
                for (t = 0; t<n - 1; t++)
                {
                    temp[k][t] = g3[k >= i ? k + 1 : k][t >= j ? t + 1 : t];
                }
            }


            ans[i][j] = calc_A(temp, n - 1);
            if ((i + j) % 2 == 1)
            {
                ans[i][j] = -ans[i][j];
            }
        }
    }
}

void calc_ulate(int n,float r[N],float s[N][3],float answer[4],float ps[N],float g1[N][4])
 {
 	for(int i=0;i<n;i++)
 	{
 	  r[i]=	sqrt((s[i][0]-answer[0])*(s[i][0]-answer[0])+(s[i][1]-answer[1])*(s[i][1]-answer[1])+(s[i][2]-answer[2])*(s[i][2]-answer[2]));
 	  ps[i]=r[i]+answer[3];
 	  for(int j=0;j<3;j++)
 	  g1[i][j]=(s[i][j]-answer[j])/r[i];
 	  g1[i][3]=1;     // 最后要弄个answer相加的循环来迭代 
	 }
 }

void deal()	//主处理函数 
{
	int n;
	float x=100;float y=100;float z=100;float t=100;//迭代法变量初始值，用于满足while循环条件的 
	float p[N]={0};  //原p数组 
	float p1[N]={0};  //上面两数组的相减 
	float s[N][3]={0};//泰勒展开式的零阶项
	float ps[N]={0};
	float answer[4]={0}; //输出的答案
	float c,e;
	float r[N]={0};
	float g1[N][4]={0};  //泰勒级数展开后的系数行列式 
	float g2[4][N]={0};  //g1的转置
	float g3[N][N]={0};//g1*g2transpose
	float g4[N][N]={0};//inverse
	float ans[N][N]={0};//g3的余子式的转置 
	printf("请输入n的大小\n");
	scanf("%d",&n);
	printf("请输入p数组\n");
	for(int i=0;i<n;i++)
	{
	    scanf("%f",&p[i]);
	}
	printf("请输入s数组\n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<3;j++)
	    scanf("%f",&s[i][j]);
	}
	printf("请输入t的精度c\n");
	scanf("%f",&c);
	printf("请输入精度e\n");
	scanf("%f",&e);          //前面全是数据输入
	 
    while(sqrt(x*x+y*y+z*z+t*t)>e)     //while循环作为总的条件 
	{
		calc_ulate(n,r,s,answer,ps,g1);  //计算 
		for(int i=0;i<n;i++)		//矩阵转换
		{
			for(int j=0;j<4;j++)
			{
				g2[j][i]=g1[i][j];
			}
		}
		for(int i=0;i<n;i++)
		{
			p1[i]=ps[i]-p[i];
		}
		for(int i=0;i<n;i++)	//g1,g2的相乘矩阵
		{
			for(int j=0;j<n;j++)
			{
				for(int k=0;k<n;k++)
				{
					g3[i][j]=g3[i][j]+g1[i][k]*g2[k][j];
				}
			}
		}	   
		float astar[N][N];
    
        float a = calc_A(g3, n);  //判断所给矩阵是否有逆矩阵 
        if (a == 0)
        {
            printf("can not transform!\n");
        }
        else
        {
            getAStart(g3, n, astar);
            for (int i = 0; i<n; i++)
            {
                for (int j = 0; j<n; j++)
                {
                  g4[i][j]=astar[j][i]/a;  //得出其逆矩阵 
                }
            }
        }
        /*最后通过公式求解*/   
	}	 	
} 

int main()		//主函数
{
	deal();
	system("pause");	//打包程序暂停 
	return 0;
}

