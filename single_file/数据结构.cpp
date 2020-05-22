#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#define MAX 1000000

int number,cnt;
double C_qp,C_cr,C_xe,C_xz;
clock_t start, finish;
double  duration;
int num[MAX];
FILE *qp,*cr,*xe,*xz;   //文件指针

void Num(){
   number = rand()%100000+20000;   //随机抽取100000以为的大于20000的数字 
 }

void Start(){
	start = clock();     //开始系统计时 
} 

void Finish(){
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;  //结束系统计时，并计算输出结果 
}

void init(){		//读入排序的数字 
	int i;
	puts("请输入你需要排列的随机数数量。(至少1000个，数量太少计时效果不佳)\n");
	scanf("%d",&cnt);
	for(i=0;i<cnt;i++){
		Num();
		num[i] = number;
	}
	puts("随机数已录入完毕");
	system("pause");
}

void notice() { //进入系统的主界面显示提示信息
	puts("\t\t\t1.排序信息录入");
	puts("\t\t\t2.进行起泡排序");
	puts("\t\t\t3.进行插入排序");
	puts("\t\t\t4.进行希尔排序");
	puts("\t\t\t5.进行选择排序");
	puts("\t\t\t6.统计对比");
	printf("\t\t\t0. 退出系统\n");
	printf("请键入：");
}

void BubbleSort(){      //起泡排序 并保存 
    int i,j;
    Start(); 
    for ( j = 0; j < cnt - 1; j++)         // 每次最大元素就像气泡一样"浮"到数组的最后
    {
        for ( i = 0; i < cnt - 1 - j; i++) // 依次比较相邻的两个元素,使较大的那个向后移
        {
            if (num[i] > num[i + 1])            // 如果条件改成num[i] >= num[i + 1],则变为不稳定的排序算法
            {
                int temp = num[i];
    			num[i] = num[j];
    			num[j] = temp;
            }
        }
    }
    Finish();
    printf("起泡排序用时：%.5f秒",duration);
    C_qp = duration;
    qp = fopen("BubbleSort.txt","w+");
    for(i=0;i<cnt;i++){
    	fprintf(qp,"%d\n",num[i]);
	}
	fclose(qp);
	puts("起泡排序结果以成功保存至程序目录下的BubbleSort.txt文件中"); 
}

void SelectionSort(){      //选择排序 
	int i,j;
    Start();
    for (i = 0; i < cnt - 1; i++)         // i为已排序序列的末尾
    {
        int min = i;
        for (j = i + 1; j < cnt; j++)     // 未排序序列
        {
            if (num[j] < num[min])              // 找出未排序序列中的最小值
            {
                min = j;
            }
        }
        if (min != i)
        {
            int temp = num[i];
		    num[i] = num[j];
		    num[j] = temp;    // 放到已排序序列的末尾，该操作很有可能把稳定性打乱，所以选择排序是不稳定的排序算法
        }
    }
    Finish();
    printf("选择排序用时：%.5f秒",duration);
    C_xz = duration;
    xz = fopen("SelectionSort.txt","w+");
    for(i=0;i<cnt;i++){
    	fprintf(xz,"%d\n",num[i]);
	}
	fclose(xz);
	puts("选择排序结果以成功保存至程序目录下的SelectionSort.txt文件中"); 
}

void ShellSort(){     //希尔排序 
   int i,j,h = 0;
   Start();
   while (h <= cnt)                          // 生成初始增量
   {
       h = 3 * h + 1;
   }
   while (h >= 1)
   {
       for (i = h; i < cnt; i++)
       {
           j = i - h;
           int get = num[i];
           while (j >= 0 && num[j] > get)
           {
               num[j + h] = num[j];
               j = j - h;
           }
           num[j + h] = get;
       }
       h = (h - 1) / 3;                    // 递减增量
   }
   Finish();
    printf("希尔排序用时：%.5f秒",duration);
    C_xe = duration;
    xe = fopen("ShellSort.txt","w+");
    for(i=0;i<cnt;i++){
    	fprintf(xe,"%d\n",num[i]);
	}
	fclose(xe);
	puts("希尔排序结果以成功保存至程序目录下的ShellSort.txt文件中"); 
}

void InsertionSort(){        //插入排序 
	Start();
	int i,j;
    for (i = 1; i < cnt; i++)         // 类似抓扑克牌排序
    {
        int get = num[i];                 // 右手抓到一张扑克牌
        j = i - 1;                  // 拿在左手上的牌总是排序好的
        while (j >= 0 && num[j] > get)    // 将抓到的牌与手牌从右向左进行比较
        {
            num[j + 1] = num[j];            // 如果该手牌比抓到的牌大，就将其右移
            j--;
        }
        num[j + 1] = get; // 直到该手牌比抓到的牌小(或二者相等)，将抓到的牌插入到该手牌右边(相等元素的相对次序未变，所以插入排序是稳定的)
    }
    Finish();
    printf("插入排序用时：%.5f秒",duration);
    C_cr = duration;
    cr = fopen("InsertionSort.txt","w+");
    for(i=0;i<cnt;i++){
    	fprintf(cr,"%d\n",num[i]);
	}
	fclose(cr);
	puts("插入排序结果以成功保存至程序目录下的ShellSort.txt文件中"); 
}

void Compare(){
	int j,i; 
	double com_time[4]={C_qp,C_cr,C_xe,C_xz};
	for (int i = 1; i < 4; i++)         // 类似抓扑克牌排序
    {
        double get = com_time[i];                 // 右手抓到一张扑克牌
        int j = i - 1;                  // 拿在左手上的牌总是排序好的
        while (j >= 0 && com_time[j] > get)    // 将抓到的牌与手牌从右向左进行比较
        {
            com_time[j + 1] = com_time[j];            // 如果该手牌比抓到的牌大，就将其右移
            j--;
        }
        com_time[j + 1] = get; 
    }
    if(com_time[0] == C_qp){printf("排序速度最快为起泡排序，用时：%.5f\n",com_time[0]);}
    if(com_time[0] == C_cr){printf("排序速度最快为插入排序，用时：%.5f\n",com_time[0]);}
    if(com_time[0] == C_xe){printf("排序速度最快为希尔排序，用时：%.5f\n",com_time[0]);}
    if(com_time[0] == C_xz){printf("排序速度最快为选择排序，用时：%.5f\n",com_time[0]);}
    if(com_time[1] == C_qp){printf("排序速度第二为起泡排序，用时：%.5f\n",com_time[1]);}
    if(com_time[1] == C_cr){printf("排序速度第二为插入排序，用时：%.5f\n",com_time[1]);}
    if(com_time[1] == C_xe){printf("排序速度第二为希尔排序，用时：%.5f\n",com_time[1]);}
    if(com_time[1] == C_xz){printf("排序速度第二为选择排序，用时：%.5f\n",com_time[1]);}
	system("pause");
	
} 

void deal() {   //主要处理函数
	int k,flag,p;
	p=1;
	flag=1;
	while(flag) {
		if(p>0)
			notice();
			scanf("%d",&k);
			system("cls");
			switch(k) {
				case 0: {
					puts("\t    ^…………^……^^谢谢您的支持,欢迎再次使用!^^……^…………^ ");
					flag=0;
					break;
				}
				case 1: {
					init();
					break;
				}
				case 2: {
					BubbleSort();
					break;
				}
				case 3: {
					InsertionSort();
					break;
				}
				case 4: {
					ShellSort();
					break;
				}
				case 5: {
					SelectionSort();
					break;
				}
				case 6: {
					Compare();
					break;
				}
				default: {
					printf("请正确键入命令前数字！\n");
					break;
				}
		}
		puts("\n");
	}
}

int main(){
	puts(">************那 个 啥 欢 迎 您 使 呗************<\n"); 
	puts(">*************     嗯~~ 请键入以下命令行所对应的数字进行操作吧^_^：     *************<\n");
	deal();//处理数据
	return 0;
}
