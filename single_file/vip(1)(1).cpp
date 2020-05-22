#include <stdio.h>
#include <string.h>
#define OK 1 
#define ERROR 0
#define NULL 0
#define MIN_C 100
#define MAX_G 1000
typedef char status;
int i_entry = 0;

typedef struct LNode {   //定义结构体
	char num[MIN_C];   //  会员的帐号 
	char name[MIN_C];  // 会员的姓名 
	char sex[MIN_C];    //会员的性别
	char pass[MIN_C];   // 会员使用密码
	double score;   // 会员账户金额  				    
	LNode *next;    //指向下一位的指针
}LNode, *LinkList;

struct student {
	char stuid[MIN_C];
	char name[MIN_C];
	char age[3];
	char gender[MIN_C];
	double birth;	
} DATA[MAX_G];
struct student temp;

void stuid_sort() {//排序 冒泡排序算法
	int r,t;
	for(r=1; r<i_entry; r++) {
		for(t=0; t<i_entry-r; t++) {
			if(DATA[t+1].birth > DATA[t].birth) {//由高到低
				temp=DATA[t+1];
				DATA[t+1]=DATA[t];
				DATA[t]=temp;
			}
		}
	}
}

void paixu(){
	int i,cnt=1;
	stuid_sort();
	printf(" 序号\t帐号\t姓名\t性别\t密码\t消费金额\n");
	for(i=0;i<i_entry;i++){
		printf("%d\t%s\t%s\t%s\t%s\t%.3lf\n",cnt,DATA[i].stuid,DATA[i].stuid,&DATA[i].age,DATA[i].gender,DATA[i].birth);
		cnt++;
	}
} 

typedef struct {
	LinkList A;

	status init()
	{
		A = new LNode;
		A->next = NULL;
		return OK;
	}

	status CreatList(int n) {    // 创建链表，输入初始的数据
		A = new LNode;
		A->next = NULL;
		LNode *p;
		for (int i = 1; i <= n; i++) {
			p = new LNode;
			printf("请输入第%d个会员的注册信息", i);
			scanf("%s %s %s %s %lf", p->num, p->name, p->sex,p->pass,&p->score);
			strcpy(DATA[i_entry].stuid,p->num);
			strcpy(DATA[i_entry].name, p->name);
			strcpy(DATA[i_entry].age,p->sex);
			strcpy(DATA[i_entry].gender,p->pass);
			DATA[i_entry].birth = p->score;
			p->next = A->next; A->next = p;
			}
			i_entry++;
			A->score = n;
			return OK;
	}


	status GetLength() {        // 得到链表中会员的个数 
		return (int)A->score;   //	score用于存放会员个数
	}


	status TraverseList() {      // 显示链表中所有的会员
		if (A->next == NULL) return ERROR;
		LNode *p; 
		int cnt = 1;
		printf("序号  账号  姓名   性别    密码      账户金额          是否会员（会员享受9折优惠）\n");
		for (p = A->next; p; p = p->next)
			if((p->score)>1000){printf("%d     %s     %s     %s     %s     %.3lf      是\n", cnt++, p->num, p->name, p->sex, p->pass, p->score);}
			else {printf("%d     %s     %s     %s     %s     %.3lf      否\n", cnt++, p->num, p->name, p->sex, p->pass, p->score);}
		return OK;
	}


	status ListInsert(int pos, LNode *temp) {  // 插入一个会员信息 
		int cnt = 0;
		LNode *p, *q;
		for (p = A; p; p = p->next) {
			if (cnt == pos - 1) break;
			cnt++;
		}
		if (!p) return ERROR;
		temp->next = p->next;
		p->next = temp;
		i_entry++;
		A->score += 1;
		return OK;
	}


	status ListDelete(int pos) {  //按照序号删除会员 
		int cnt = 0;
		LNode *p, *q;
		for (p = A; p; p = p->next) {
			if (cnt == pos - 1) break;
			cnt++;
		}
		if (!p) return ERROR;
		q = p->next;
		p->next = q->next;
		A->score -= 1;
		return OK;
	}
	status Listfix(int pos) {  //按照序号修改会员信息 
		int cnt = 0;
		LNode *p, *q;
		for (p = A; p; p = p->next) {
			if (cnt == pos) break;
			cnt++;
		}
		if (!p) return ERROR;
		int Change;
		double je;
		char x[25];
		printf("请输入所需修改项对应的序号。\n 1.账号 2.姓名 3.性别 4.密码 5.用户金额\n");
		scanf("%d", &Change);
		if (Change != 5)
		{
			printf("请输入修改的信息\n");
			scanf("%s", x);
			printf("修改成功\n");
		}
		else
		{
			printf("请输入修改后的金额\n");
			scanf("%lf", &je);
		}
		switch (Change) {
		case 1:strcpy(p->num, x); break;
		case 2:strcpy(p->name , x); break;
		case 3:strcpy(p->sex , x); break;
		case 4:strcpy(p->pass , x); break;
		case 5:p->score = je; break;
		default:printf("ERROR"); break;
		}
		return OK;
	}

	status ListDelete(char *str, int kind) {  // kind =1 按照姓名来删除，kind=2按照账号来删除 
		LNode *p, *q;
		for (p = A; p; q = p, p = p->next) {
			if (strcmp(p->name, str) == 0 && kind == 1) break;
			if (strcmp(p->num, str) == 0 && kind == 2) break;
		}
		if (!p) return ERROR;
		q->next = p->next;
		A->score -= 1;
		return OK;
	}

	LNode *ListSearch(int pos) {//  按照序号查找会员
		LNode *p; int cnt = 0;
		for (p = A->next; p; p = p->next) {
			cnt++;
			if (cnt == pos) break;
		}
		return p;
	}

	LNode *ListSearch(char *str, int &pos, int kind) { // kind =1 按照姓名来查找，kind=2按照帐号来查找 
		LNode *p; pos = 0;
		for (p = A->next; p; p = p->next) {
			pos++;
			if (strcmp(p->name, str) == 0 && kind == 1) break;
			if (strcmp(p->num, str) == 0 && kind == 2)  break;
		}
		return p;
	}

}List;



 main() {
	List stu;
	puts("                 欢迎使用会员卡计费系统!");
	printf("                 请输入会员人数:");
	int n;
	scanf("%d", &n);
	puts("                 请按照格式依次输入会员的信息(账号 姓名 性别 密码 账户金额):\n");
	stu.CreatList(n);
	puts("                 会员信息已经录入成功!\n");
	int NoEnd = 1;
	while (NoEnd) {
		puts("                 管理菜单\n");
		puts("                 A:当前管理的会员人数。       B:显示当前管理的所有会员的信息。");
		puts("                 C:插入某会员信息。           D:删除某会员信息。");
		puts("                 E:查找某位会员的信息。       F:修改某会员的信息");
		puts("                 H:查看会员消费情况。         O:退出。");
		char op[5]; scanf("%s", op);
		switch (op[0]) {
		case 'A': {
			printf("当前管理的会员人数为 %d。\n", stu.GetLength());
			break;
		}
		case 'B': {
			status t = stu.TraverseList();
			if (t == ERROR) puts("目前还没有存储会员信息");
			break;
		}
		case 'C': {
			int pos; 
			LNode *t = new LNode;	//新的节点 
			printf("请输入将要插入的位置序号:"); 
			scanf("%d", &pos);
			if (stu.GetLength() != 0 && pos>stu.GetLength())  
				puts("您输入的序号有误");
			else {
				puts("请输入插入会员的信息:");
				printf("账号:"); scanf("%s", t->num);
				printf("姓名:"); scanf("%s", t->name);
				printf("性别:"); scanf("%s", t->sex);
				printf("密码:"); scanf("%s", t->pass);
				printf("金额:"); scanf("%lf", &t->score);
				strcpy(DATA[i_entry].stuid,t->num);
				strcpy(DATA[i_entry].name, t->name);
				strcpy(DATA[i_entry].age,t->sex);
				strcpy(DATA[i_entry].gender,t->pass);
				DATA[i_entry].birth = t->score;
				i_entry++;				
				status tt = stu.ListInsert(pos, t);//尾接 
				puts("插入会员信息成功");
			}
			break;
		}
		case 'D': 
		{
			if (stu.GetLength() == 0) {
				puts("当前没有存储会员信息,无法删除");
				break;
			}
			int NotEnd = 1;
			while (NotEnd) {
				NotEnd = 0; char r[5];
				puts("请选择删除的方式:");
				printf("A :按照会员序号进行删除   B:按照会员姓名进行删除\n");
				printf("C :按照会员学号进行删除   D:返回主菜单\n"); scanf("%s", r);
				if (r[0] == 'A') {
					int pos;
					printf("请输入将要删除的会员的序号:"); scanf("%d", &pos);
					if (pos<1 || pos>stu.GetLength())  puts("您输入的序号有误");
					else {
						stu.ListDelete(pos);
						puts("删除成功");
					}
				}
				else if (r[0] == 'B') {
					char name[25];
					printf("请输入将要删除的会员的姓名:"); scanf("%s", name);
					status t = stu.ListDelete(name, 1);
					if (!t) puts("您输入的姓名有误，无法删除");
					else  puts("删除成功");
				}
				else if (r[0] == 'C') {
					char num[25];
					printf("请输入将要删除的会员的帐号:"); scanf("%s", num);
					status t = stu.ListDelete(num, 2);
					if (!t) puts("您输入的帐号有误，无法删除");
					else  puts("删除成功");
				}
				else if (r[0] == 'D')  continue;
				else {
					NotEnd = 1;
					puts("请输入正确的操作命令");
				}
			}
			break;
		}
		case 'E': {
			if (stu.GetLength() == 0) {
				puts("当前没有存储会员信息,无法进行查询操作");
				break;
			}
			int NotEnd = 1;
			while (NotEnd) {
				NotEnd = 0;
				char r[5];
				puts("请选择查找的方式:");
				printf("A:按照会员序号进行查找   B:按照会员姓名进行查找\n");
				printf("C:按照会员帐号进行查找   D:返回主菜单\n");
				scanf("%s", r);
				if (r[0] == 'A') {
					int pos;
					printf("请输入将要查找的会员的序号:"); scanf("%d", &pos);
					if (pos<1 || pos>stu.GetLength())  puts("您输入的序号有误");
					else {
						LNode *t = stu.ListSearch(pos);
						puts("查到的信息为:");
						printf(" 帐号 姓名 性别 密码 账户金额\n");
						printf("%s  %s %s %s %.3lf\n", pos,t->num, t->name, t->sex, t->pass, t->score);
					}
				}
				else if (r[0] == 'B') {
					char name[25];  int pos;
					printf("请输入将要查找的会员的姓名:"); 
					scanf("%s", name);
					LNode *t = stu.ListSearch(name, pos, 1);
					if (!t) puts("您输入的姓名有误，无法查到");
					else {
						puts("查到的信息为:");
						printf(" 帐号 姓名 性别 密码 账户金额\n");
						printf("%d %s %s %s %s %.3lf\n", pos, t->num, t->name, t->sex, t->pass, t->score);
					}
				}
				else if (r[0] == 'C') {
					char num[25]; int pos;
					printf("请输入将要查找的会员的帐号:"); scanf("%s", num);
					LNode *t = stu.ListSearch(num, pos, 2);
					if (!t) puts("您输入的帐号有误，无法查到");
					else {
						puts("查到的信息为:");
						printf(" 帐号 姓名 性别 密码 消费金额\n");
						printf("%s  %s %s %s %.3lf\n", pos, t->num, t->name, t->sex, t->pass, t->score);
					}
				}
				else if (r[0] == 'D') continue;
				else {
					NotEnd = 1;
					puts("请输入正确的操作命令");
				}
			}
			break;
		}
		case 'F':
		{
			if (stu.GetLength() == 0) {
				puts("当前没有存储会员信息,无法修改");
				break;
			}
			int NotEnd = 1;
			while (NotEnd) {
				NotEnd = 0; char r[5];
				puts("请选择修改的方式:");
				printf("A :按照会员序号进行修改   ");
				printf("  D:返回主菜单\n"); scanf("%s", r);
				if (r[0] == 'A') {
					int pos;
					printf("请输入将要修改信息的的会员的序号:"); 
					scanf("%d", &pos);
					if (pos<1 || pos>stu.GetLength())  puts("您输入的序号有误");
					else {
						stu.Listfix(pos);
						printf("修改成功");
					}
				}
				
				else if (r[0] == 'D')  continue;
				else {
					NotEnd = 1;
					puts("请输入正确的操作命令");
				}
			}
			break;
		}
		case 'H': {
				if (stu.GetLength() == 0) {
				puts("当前没有存储会员信息,无法查看");
				break;
			}
				int NotEnd = 1;
			while (NotEnd) {
				NotEnd = 0; char r[5];
				printf("A:查看会员消费情况。   B:按返回主菜单\n");
				scanf("%s", r);
				if (r[0] == 'A') {
					paixu();
				}				
				else if (r[0] == 'B')  continue;
				else {
					NotEnd = 1;
					puts("请输入正确的操作命令");
				}
			}
			break;
		
			}
		case 'O': {
			NoEnd = 0;
			break;
		}
		default: {
			puts("您的操作有误，请输入A-H或Q字符来确定您的操作");
			break;
		}
		}
	}
	puts("欢迎下次使用！");
	return 0;
}

