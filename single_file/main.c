//
//  main->c
//  大一下实验2
//
//  Created by 蓝翔 on 2019/5/16->
//  Copyright ? 2019 Azure-> All rights reserved->
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN  20
#define MAX  1000
#define NameMaxSize 20
#define SexMaxSize  5
#define MajorMaxSize  20
#define CnameMaxSize  20

int i_number[3] = {0};  //全局变量，记录人数

typedef struct student
{
    int sno;
    char sname[NameMaxSize];
    char sex[SexMaxSize];
    char major[MajorMaxSize];
}Student;

typedef struct stud
{
    int sno;
    char sname[NameMaxSize];
    char sex[SexMaxSize];
    char major[MajorMaxSize];
    struct stud *next;			//链表节点 
}Stud;

//课程信息
typedef struct course
{
    int cno;
    char cname[CnameMaxSize];
    int classHours;
}Course;

//课程成绩
typedef struct cgrade
{
    int sno;
    int cno;
    int score;
}CourseGrade;

typedef struct stu{                    //临时储存信息结构体
    int number;
    char name[MIN];
    char maj[MIN];
    char cls_name[MIN];
    int cals;
    int sor;
    struct stu *next; 				//链表节点 
}StuList;

void save_file(int n,Student S[],Course C[],CourseGrade CG[])        //文件保存
{
    FILE *fp;
    if(n==1)        //保存至student.dat
    {
        if(!(fp = fopen("student.dat","w")))
        {
            fp = fopen("student.dat","w+");
        }
        n = i_number[0];
        while(n)
        {
            fprintf(fp,"%d\t%s\t%s\t%s\n",S[i_number[0]-n].sno,S[i_number[0]-n].sname,S[i_number[0]-n].sex,S[i_number[0]-n].major);
            n--;
        }
        fclose(fp);
    }
    else if(n==2)
    {
        if(!(fp = fopen("course.dat","w")))
        {
            fp = fopen("course.dat","w+");
        }
        n = i_number[1];
        while(n)
        {
            fprintf(fp,"%d\t%s\t%d\n",C[i_number[1]-n].cno,C[i_number[1]-n].cname,C[i_number[1]-n].classHours);
            n--;
        }
        fclose(fp);
    }
    else if(n==3)
    {
        if(!(fp = fopen("courseGrade.dat","w")))
        {
            fp = fopen("courseGrade.dat","w+");
        }
        n = i_number[2];
        while(n)
        {
            fprintf(fp,"%d\t%d\t%d\n",CG[i_number[2]-n].sno,CG[i_number[2]-n].cno,CG[i_number[2]-n].score);
            n--;
        }
        fclose(fp);
    }
    puts("保存成功！！");
  
}

void student_init(Student S[],Course C[],CourseGrade CG[])            //信息输入
{
    int i,cnt;
    while(1)
    {
       
        printf("请选择输入选项(1.输入学生记录 2.输入课程 3.输入成绩 0.退出):");
        scanf("%d",&cnt);
        if(cnt==0)
        {
            break;
        }
        else if(cnt==1)
        {
           
            printf("请输入学生个数：");
            scanf("%d",&cnt);
            for(i=0;i<cnt;i++)
            {
                printf("请输入第%d个学生的信息(学号 姓名 性别 专业)：",i+1);
                scanf("%d %s %s %s",&S[i_number[0]].sno,S[i_number[0]].sname,S[i_number[0]].sex,S[i_number[0]].major);
                i_number[0]++;
            }
            save_file(1,S,C,CG);
        }
        else if(cnt==2)
        {
            printf("请输入课程个数：");
            scanf("%d",&cnt);
            for(i=0;i<cnt;i++)
            {
                printf("请输入第%d个课程的信息(课程号 课程名 课程学时)：",i+1);
                scanf("%d %s %d",&C[i_number[1]].cno,C[i_number[1]].cname,&C[i_number[1]].classHours);
                i_number[1]++;
            }
            save_file(2,S,C,CG);
        }
        else if(cnt==3)
        {
           
            printf("请输入成绩个数：");
            scanf("%d",&cnt);
            for(i=0;i<cnt;i++)
            {
                printf("请输入第%d个学生的信息(学号 课程号 成绩)：",i+1);
                scanf("%d %d %d",&CG[i_number[2]].sno,&CG[i_number[2]].cno,&CG[i_number[2]].score);
                i_number[2]++;
            }
            save_file(3,S,C,CG);
        }
    }
    puts("录入完成！！");
   
}

void file_read(Student S[],Course C[],CourseGrade CG[])            //文件读取
{
    char str[MIN];
    int i,r,t;
    while(1)
    {
      
        printf("请输入你要读取的文件名(输入0退出)：");
        scanf("%s",str);
        if(strcmp("0",str) == 0)
        {
            break;
        }
        else if(strcmp(str,"student.dat") == 0)
        {
            FILE *fp;
            Student temp;
            if(!(fp = fopen("student.dat","rb")))
            {
                printf("该文件不存在！！！\n");
            }
            else
            {
                while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中
                {
                    fscanf(fp,"%d\t%s\t%s\t%s\n",&S[i_number[0]].sno,S[i_number[0]].sname,
                           S[i_number[0]].sex,S[i_number[0]].major);
                    i_number[0]++;
                }
            }
            fclose(fp);
            for(r=1; r<i_number[0]; r++)         //排序
            {
                for(t=0; t<i_number[0]-1; t++)
                {
                    if(S[t+1].sno < S[t].sno)
                    {
                        temp=S[t+1];
                        S[t+1]=S[t];
                        S[t]=temp;
                    }
                }
            }
   
            printf("学号\t姓名\t性别\t专业\n");
            for(i=0;i<i_number[0];i++)
            {
                printf("%d\t%s\t%s\t%s\n",S[i].sno,S[i].sname,S[i].sex,S[i].major);
            }
            printf("读入成功\n");
        
        }
        else if(strcmp(str,"course.dat") == 0)
        {
            FILE *fp;
            Course temp;
            if(!(fp = fopen("course.dat","rb")))
            {
                printf("该文件不存在！！！\n");
            }
            else
            {
                while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中
                {
                    fscanf(fp,"%d\t%s\t%d\n",&C[i_number[1]].cno,C[i_number[1]].cname,&C[i_number[1]].classHours);
                    i_number[1]++;
                }
            }
            fclose(fp);
            for(r=1; r<i_number[1]; r++)         //排序
            {
                for(t=0; t<i_number[1]-1; t++)
                {
                    if(C[t+1].cno < C[t].cno)
                    {
                        temp=C[t+1];
                        C[t+1]=C[t];
                        C[t]=temp;
                    }
                }
            }
          
            printf("课程号\t课程名称\t课时\n");
            for(i=0;i<i_number[1];i++)
            {
                printf("%d\t%s\t%d\n",C[i].cno,C[i].cname,C[i].classHours);
            }
            printf("读入成功\n");
          
        }
        else if(strcmp(str,"courseGrade.dat") == 0)
        {
            FILE *fp;
            CourseGrade temp;
            if(!(fp = fopen("courseGrade.dat","rb")))
            {
                printf("该文件不存在！！！\n");
            }
            else
            {
                while (!feof(fp))   //如果检测不到文本末尾继续读取，储存到相应的变量中
                {
                    fscanf(fp,"%d\t%d\t%d\n",&CG[i_number[2]].sno,&CG[i_number[2]].cno,&CG[i_number[2]].score);
                    i_number[2]++;
                }
            }
            fclose(fp);
            for(r=1; r<i_number[2]; r++)         //排序
            {
                for(t=0; t<i_number[2]-1; t++)
                {
                    if(CG[t+1].sno < CG[t].sno)
                    {
                        temp=CG[t+1];
                        CG[t+1]=CG[t];
                        CG[t]=temp;
                    }
                }
            }
         
            printf("学号\t课程号\t成绩\n");
            for(i=0;i<i_number[2];i++)
            {
                printf("%d\t%d\t%d\n",CG[i].sno,CG[i].cno,CG[i].score);
            }
            printf("读入成功\n");
      
        }
    }
}


void stu_scores(Student S[],Course C[],CourseGrade CG[])        //学生成绩
{
    FILE *fp;
    StuList *head,*node,*end,*p,*q;		//定义头节点、普通节点、尾部节点
	head = (StuList*)malloc(sizeof(StuList));		//分配内存
	end = head; 
    int i,t,cnt;
    while(1)
    {
      
        printf("请选择功能(1.统计成绩 2.查找指定课程号成绩 3.查询小于60分学生信息 0.退出)：");
        scanf("%d",&cnt);
        if(cnt==1)
        {
          
            for(i=0;i<i_number[2];i++)        //查找对应的信息
            {
            	node = (StuList*)malloc(sizeof(StuList));
                node->number= CG[i].sno;        //学号
                node->cals = CG[i].cno;        //课程号
                node->sor = CG[i].score;        //分数
                for(t=0;t<i_number[0];t++)
                {
                    if(node->number==S[t].sno)
                    {
                        strcpy(node->name,S[t].sname);        //姓名
                        strcpy(node->maj,S[t].major);    //专业
                        break;
                    }
                }
                for(t=0;t<i_number[1];t++)
                {
                    if(node->cals == C[t].cno)
                    {
                        strcpy(node->cls_name,C[t].cname);
                        break;
                    }
                }
                end->next = node;
                end = node;
            }
            end->next = NULL;		//链表创建结束 
            for(p = end->next; p != NULL; p = p->next)			//冒泡排序 
            {
            	for(q = p->next; q != NULL; q = q->next)
            	{
            		if((p->sor) > (q->sor))
            		{
            			int s = q->sor;
						q->sor = p->sor;
						p->sor = s;
					}      
				}     
			}
            printf("序号\t学号\t姓名\t专业\t课程名\t成绩\n");
            while(node->next != NULL)
            {
                printf("%d\t%s\t%s\t%s\t%d\n",node->number,node->name,node->maj,node->cls_name,node->sor);
            }
            if(!(fp=fopen("studentGrade->dat","w")))
            {
                fp = fopen("studentGrade->dat","w+");
            }
            while(node->next != NULL)
            {
                fprintf(fp,"\t%d\t%s\t%s\t%s\t%d\n",node->number,node->name,node->maj,node->cls_name,node->sor);
            }
            fclose(fp);
            puts("写入成功！！");
           
        }
        else if(cnt == 2)
        {
          
            printf("请输入你要查询的课程号：");
            scanf("%d",&cnt);
            printf("查询结果如下····\n\n");
            printf("学号\t姓名\t专业\t课程名\t成绩\n");
            i = 0;
            while(node->next != NULL)
            {
                if(node->cals == cnt)
                {
                    printf("%d\t%s\t%s\t%s\t%d\n",node->number,node->name,node->maj
                           ,node->cls_name,node->sor);
                }
                i++;
            }
        
        }
        else if(cnt==3)
        {
           
            i = 0;
            printf("查询小于60分结果如下····\n\n");
            printf("学号\t姓名\t专业\t课程名\t成绩\n");
            while(node->next != NULL)
            {
                if(node->sor<60)
                {
                    printf("%d\t%s\t%s\t%s\t%d\n",node->number,node->name,node->maj
                           ,node->cls_name,node->sor);
                }
                i++;
            }
           
        }
        else if(cnt == 0)
            break;    
    }
}

void zhai_new(Student S[])            //逆序
{
 	
    printf("链表逆序结果如下···\n\n");
    int i=0;
    Stud *end,*node,*ret,*p;
	end = (Stud*)malloc(sizeof(Stud));
	for(i=0;i<i_number[0];i++)
	{
		node = (Stud*)malloc(sizeof(Stud));
		strcpy(node->major,S[i].major);
		strcpy(node->sex,S[i].sex);
		strcpy(node->sname,S[i].sname);
		node->sno = S[i].sno;
		end->next = node;
		end = node;
	} 
	end->next = NULL;
	
	ret = node->next;
	node->next = NULL;
	p = NULL;
	while (ret)		//链表逆序
	{
		p = ret->next;
		ret->next = node->next;
		node->next = ret;
		ret = p;
	}
    printf("学号\t姓名\t性别\t专业\n");
    while(node->next != NULL)
    {
        printf("%d\t%s\t%s\t%s\n",node->sno,node->sname,node->sex,node->major);
    }
}

int main()
{
    Student S[MAX];
    Course C[MAX];
    CourseGrade CG[MAX];
    int cnt;
    while(1)
    {
        printf("请选择功能(1.信息输入 2.文件读取 3.链表运算 4.链表逆序 0.退出)：");
        scanf("%d",&cnt);
        if(cnt==0)
        {
            printf("谢谢使用\n");
            break;
        }
        else if(cnt==1)
        {
            student_init(S,C,CG);
        }
        else if(cnt==2)
        {
            file_read(S,C,CG);
        }
        else if(cnt==3)
        {
            stu_scores(S,C,CG);
        }
        else if(cnt==4)
        {
            zhai_new(S);
        }
        else
        {
            puts("输入有误！！！请重新输入");
        }
    }
    return 0;
}

