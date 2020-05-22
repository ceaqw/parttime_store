#include<stdio.h>
#include<math.h>

int main()
{
    int i,j,k,n=0;
    int num[30];
    for(i=100;i<=200;i++)			//判断该数是否为素数 
    {
        k=(int)sqrt(i);
        for(j=2;j<=k;j++)
        {
			if(i%j==0)
			{
				break;
			} 
		}    
        if(j>k)
        {
        	num[n] = i;
        	n++;
        }
    }

    printf("100到200之间的相邻素数有：\n");
    for(i=0;i<n;i+=2)			//判断是否为相邻素数 
    {
    	if((num[i+1]-num[i]) == 2)
    	{
    		printf("%d %d\n",num[i],num[i+1]);
		}
	}
    return 0;
}
