#include <stdio.h>
#define MAX 100

int num[MAX];

int i_number; 

void Swap(int A[], int i, int j)
{
    int temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

void BubbleSort(int A[], int n)
{
    int i,j;
    for ( j = 0; j < n - 1; j++)         // 每次最大元素就像气泡一样"浮"到数组的最后
    {
        for ( i = 0; i < n - 1 - j; i++) // 依次比较相邻的两个元素,使较大的那个向后移
        {
            if (A[i] > A[i + 1])            // 如果条件改成A[i] >= A[i + 1],则变为不稳定的排序算法
            {
                Swap(A, i, i + 1);
            }
        }
    }
}

void insert()  //插入数字排序 
{
	int i; 
	printf("请输入要插入的数字：");
	scanf("%d",&num[i_number]);
	i_number++;
	BubbleSort(num, i_number);
	printf("气泡排序结果：\n");
    for ( i = 0; i < i_number; i++)
    {
        printf("%d ", num[i]);
        if(i == 5)
        {
        	printf("\n");
		}
    } 
 } 

int main()
{
	int i,k;
	printf("请输入排序数字个数：");
	scanf("%d",&k);
	for(i=0;i<k;i++)
	{
		printf("请输入第%d个随机数字：",i+1);
		scanf("%d",&num[i]);
		i_number++;
	}
    BubbleSort(num, i_number);
    printf("气泡排序结果：\n");
    for ( i = 0; i < i_number; i++)
    {
        printf("%d ", num[i]);
        if(i == 5)
        {
        	printf("\n");
		}
    }
    puts("");
    while(1)
    {
    	puts("1.插入排序    2.退出");
    	printf("请输入：");
    	scanf("%d",&k);
    	if(k == 1)
    	{
    		insert();
		}
		else
		{
			break;
		}
	}
    return 0;
}


