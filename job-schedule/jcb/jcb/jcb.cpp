#pragma warning(disable:4996)
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<iostream>
#include<time.h>
#define Maxjob 24
#define recsfnum 5
int recordjobnum;
using namespace std;
struct jcb
{
	char jobname[10];//作业名称
	char status; //状态：开始s，等待d，就绪j
	float arrtime;//到达时间
	float reqtime;//要求服务时间
	float starttime;//开始时间
	float finishtime;//完成时间
	float TAtime;//周转时间
	float TAWtime;//带权周转时间
	float ATAWtime;//平均带权周转时间
	float ATAtime;//平均作业周转时间
	float hrrf;//最高响应比
}job[Maxjob], tim[recsfnum];

float systime = 0;
int recordATA = 0;//记录平均周转时间的个数
int intarr, intfin, intjob;//到达作业个数，完成作业个数，未到达作业个数
void menu()
{
	printf("\t\t|-----------------------------------|\n");
	printf("\t\t|     欢迎进入作业调度              |\n");
	printf("\t\t|-----------------------------------|\n");
	printf("\t\t|           1:输入数据              |\n");
	printf("\t\t|           2:插入数据              |\n");
	printf("\t\t|           3:删除数据              |\n");
	printf("\t\t|           4:选择算法              |\n");
	printf("\t\t|           0:结束                  |\n");
	printf("\t\t|-----------------------------------|\n");
	printf("请输入你要选择的功能(0~4):");
}
void sort()//排序
{
	int i, j;
	float arrtemp, reqtemp;
	char nametemp[10];
	for (i = 0; i < recordjobnum; i++)
	{
		for (j = i + 1; j < recordjobnum; j++)
		{
			if (job[i].arrtime > job[j].arrtime)
			{
				arrtemp = job[i].arrtime;
				reqtemp = job[i].reqtime;
				strcpy(nametemp, job[i].jobname);
				job[i].arrtime = job[j].arrtime;
				job[i].reqtime = job[j].reqtime;
				strcpy(job[i].jobname, job[j].jobname);
				job[j].arrtime = arrtemp;
				job[j].reqtime = reqtemp;
				strcpy(job[j].jobname, nametemp);
			}
		}
	}
}
void HandworkInputData()//手工输入数据
{
	int i;
	printf("请输入作业的个数:");
	scanf("%d", &recordjobnum);
	for (i = 0; i < recordjobnum; i++)
	{
		printf("\n第%d个作业:\n", i + 1);
		printf("作业名:");
		scanf("%s", job[i].jobname);
		printf("到达时间:");
		scanf("%f", &job[i].arrtime);
		printf("CPU运行时间:");
		scanf("%f", &job[i].reqtime);

	}
	printf("\n 作业名    作业到达时间     作业运行所需要时间\n");
	for (i = 0; i < recordjobnum; i++)
		printf("\n%3s%12.2f%15.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);
	printf("\n");
}

void Pseudo_random_number()//伪随机数输入
{
	int i;
	srand((unsigned)time(0));  //参数seed是rand()的种子，用来初始化rand()的起始值。
	//输入作业数
	recordjobnum = rand() % 23 + 5;
	for (i = 0; i < recordjobnum; i++)
	{
		itoa(i + 1, job[i].jobname, 10);
		//作业到达时间
		job[i].arrtime = (float)(rand() % 29 + 1);//把得到的结果强制转换为float
		//作业运行时间
		job[i].reqtime = (float)(rand() % 7 + 1);
	}
	printf("\n 作业名    作业到达时间     作业运行所需要时间\n");
	for (i = 0; i < recordjobnum; i++)
	{
		printf("\n%3s%12.2f%15.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);
	}
	printf("\n");

}
void FileInputtData()//文件读取
{
	int i = 0;
	FILE* fp;     //定义文件指针
	fp = fopen("save.txt", "r");  //打开文件
	if (fp == NULL)
	{
		printf("File open error !\n");
		exit(0);
	}
	printf("\n 作业名    作业到达时间     作业运行所需要时间\n");
	while (!feof(fp))
	{
		fscanf(fp, "%s%f%f", job[i].jobname, &job[i].arrtime, &job[i].reqtime);  //fscanf()函数将数据读入
		printf("\n%3s%12.2f%15.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);  //输出到屏幕
		i++;
	}
	printf("\n");
	if (fclose(fp))     //关闭文件
	{
		printf("Can not close the file !\n");
		exit(0);
	}
	recordjobnum = i;
}
void InputData()//输入数据
{
	int n;
	while (1)
	{
		printf("\t\t*********************************\n");
		printf("\t\t1.调用文本写入数据\n");
		printf("\t\t2.调用伪随机数输入数据\n");
		printf("\t\t3.调用自己输入模拟数据\n");
		printf("\t\t0.退出输入程序\n");
		printf("\t\t**********************************\n");
		printf("请选择输入方式:");
		scanf("%d", &n);
		if (n == 0)
			break;
		switch (n)
		{
		case 1:
			FileInputtData(); 
			break;
		case 2:Pseudo_random_number();
			break;
		case 3:
			HandworkInputData(); 
			break;
		case 0:
			n = 0; 
			break;
		}
	}
}

void InsertData()//插入数据
{
	char name[10];
	float arrivetime;
	float requtime;
	int i, j;
	printf("请输入要插入的数据:\n");
	printf("作业名:");
	scanf("%s", name);
	printf("到达时间:");
	scanf("%f", &arrivetime);
	printf("CPU运行时间:");
	scanf("%f", &requtime);
	sort();//排序
	for (i = 0; i <= recordjobnum; i++)
	{
		//把数据插入第一个的时候
		if ((i < recordjobnum) && (arrivetime <= job[i].arrtime))
		{
			for (j = recordjobnum - 1; j >= i; j--)
			{
				job[j + 1].arrtime = job[j].arrtime;
				job[j + 1].reqtime = job[j].reqtime;
				strcpy(job[j + 1].jobname, job[j].jobname);
			}
			job[i].arrtime = arrivetime;
			job[i].reqtime = requtime;
			strcpy(job[i].jobname, name);
			break;
		}
		//把数据插入中间的时候
		else if ((i < recordjobnum) && (arrivetime >= job[i].arrtime) && (arrivetime <= job[i + 1].arrtime))
		{
			for (j = recordjobnum - 1; j >= i + 1; j--)
			{
				//找到位置后，先把此位置以后的数据都往后移
				job[j + 1].arrtime = job[j].arrtime;
				job[j + 1].reqtime = job[j].reqtime;
				strcpy(job[j + 1].jobname, job[j].jobname);
			}
			//把数据插入到此位置
			job[i + 1].arrtime = arrivetime;
			job[i + 1].reqtime = requtime;
			strcpy(job[i + 1].jobname, name);
			break;
		}
		//插入到最后面
		if ((i = recordjobnum) && (arrivetime >= job[recordjobnum - 1].arrtime))
		{
			job[i].arrtime = arrivetime;
			job[i].reqtime = requtime;
			strcpy(job[i].jobname, name);
		}
	}
	recordjobnum = recordjobnum + 1;//插入一个数据时，数组的容量加1
	printf("\n 作业名    作业到达时间     作业运行所需要时间\n");
	for (i = 0; i < recordjobnum; i++)
		printf("\n%3s%12.2f%15.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);
	printf("\n");
}

void DeleteData()//删除数据
{
	char name[10];
	int i, j;
	printf("请输入要删除的作业名:");
	scanf("%s", name);
	for (i = 0; i < recordjobnum; i++)
	{
		if (strcmp(job[i].jobname, name) == 0)
		{
			for (j = i + 1; j < recordjobnum; j++)
			{
				//找到要删除的数据以后，把此位置后面的数据往前移，后面的数据就会把该位置给覆盖，达到删除的效果
				job[j - 1].arrtime = job[j].arrtime;
				job[j - 1].reqtime = job[j].reqtime;
				strcpy(job[j - 1].jobname, job[j].jobname);
			}
		}
	}
	recordjobnum = recordjobnum - 1;//删除数据后，数组容量减1
	printf("\n 作业名    作业到达时间     作业运行所需要时间\n");
	for (i = 0; i < recordjobnum; i++)
		printf("\n%3s%12.2f%15.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);

}

void SJF()//最短作业优先算法SJF
{
	int i, j;
	float sum = 0, sum1 = 0;
	float arrtemp, reqtemp;
	char nametemp[10];
	for (i = 0; i < recordjobnum; i++)
	{
		//先找到到达系统时间为0的数据，把它放在第一位，因为系统会第一时间执行这个作业
		if (job[i].arrtime == 0)
		{
			arrtemp = job[0].arrtime;
			reqtemp = job[0].reqtime;
			strcpy(nametemp, job[0].jobname);
			job[0].arrtime = job[i].arrtime;
			job[0].reqtime = job[i].reqtime;
			strcpy(job[0].jobname, job[i].jobname);
			job[i].arrtime = arrtemp;
			job[i].reqtime = reqtemp;
			strcpy(job[i].jobname, nametemp);
		}
	}
	printf("**********最短作业优先算法SJF*************\n");
	//从第二位开始根据系统运行时间的长短进行排序，此排序为升序
	for (i = 1; i < recordjobnum; i++)
	{
		for (j = i + 1; j < recordjobnum; j++)
		{
			if (job[i].reqtime > job[j].reqtime)
			{
				arrtemp = job[i].arrtime;
				reqtemp = job[i].reqtime;
				strcpy(nametemp, job[i].jobname);
				job[i].arrtime = job[j].arrtime;
				job[i].reqtime = job[j].reqtime;
				strcpy(job[i].jobname, job[j].jobname);
				job[j].arrtime = arrtemp;
				job[j].reqtime = reqtemp;
				strcpy(job[j].jobname, nametemp);
			}
		}
	}
	printf("\n 作业名 作业到达时间 作业运行所需要时间\n");
	for (i = 1; i < recordjobnum; i++)
		printf("\n%4s%12.2f%12.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);
	//计算作业到达系统的时间，作业周转时间，带权作业周转时间
	for (i = 1; i < recordjobnum; i++)
	{
		job[i].finishtime = systime + job[i].reqtime;
		systime = job[i].finishtime;
		job[i].TAtime = job[i].finishtime - job[i].arrtime;
		job[i].TAWtime = job[i].TAtime / job[i].reqtime;
	}
	printf("\n作业名 作业到达时间 作业完成时间 运行时间 作业周转时间 带权作业周转时间\n");
	for (i = 1; i < recordjobnum; i++)
		printf("%4s%12.2f%12.2f%12.2f%12.2f%12.2f\n", job[i].jobname, job[i].arrtime, job[i].finishtime, job[i].reqtime, job[i].TAtime, job[i].TAWtime);

	for (i = 1; i < recordjobnum; i++)
	{
		sum = sum + job[i].TAtime;//计算作业周转时间的总和
		sum1 = sum1 + job[i].TAWtime;//计算作业带权周转时间的总和
	}
	tim[recordATA].ATAtime = sum / recordjobnum;//计算该算法所需的平均周转时间
	tim[recordATA].ATAWtime = sum1 / recordjobnum;//计算该算法所需的平均带权周转时间
	printf("\n平均周转时间是:%f\n", tim[recordATA].ATAtime);
	printf("平均带权周转时间是:%f\n", tim[recordATA].ATAWtime);
	recordATA = recordATA + 1;
	systime = 0;
}


void FCFS()//先来先服务FCFS
{
	int i, j;
	float arrtemp, reqtemp;
	float sum = 0, sum1 = 0;
	char nametemp[10];
	//先找到到达系统时间为0的数据，把它放在第一位，因为系统会第一时间执行这个作业
	for (i = 0; i < recordjobnum; i++)
	{
		if (job[i].arrtime == 0)
		{
			arrtemp = job[0].arrtime;
			reqtemp = job[0].reqtime;
			strcpy(nametemp, job[0].jobname);
			job[0].arrtime = job[i].arrtime;
			job[0].reqtime = job[i].reqtime;
			strcpy(job[0].jobname, job[i].jobname);
			job[i].arrtime = arrtemp;
			job[i].reqtime = reqtemp;
			strcpy(job[i].jobname, nametemp);
		}
	}
	printf("**********先来先服务算法FCFS***************\n");
	//从第二位开始根据系统运行时间的长短进行排序，此排序为升序
	for (i = 1; i < recordjobnum; i++)
	{
		for (j = i + 1; j < recordjobnum; j++)
		{
			if (job[i].arrtime > job[j].arrtime)
			{
				arrtemp = job[i].arrtime;
				reqtemp = job[i].reqtime;
				strcpy(nametemp, job[i].jobname);
				job[i].arrtime = job[j].arrtime;
				job[i].reqtime = job[j].reqtime;
				strcpy(job[i].jobname, job[j].jobname);
				job[j].arrtime = arrtemp;
				job[j].reqtime = reqtemp;
				strcpy(job[j].jobname, nametemp);
			}
		}
	}

	printf("\n 作业名  作业到达时间   作业运行所需要时间\n");
	for (i = 1; i < recordjobnum; i++)
		printf("\n%3s%12.2f%18.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);
	//计算作业到达系统的时间，作业周转时间，带权作业周转时间
	for (i = 1; i < recordjobnum; i++)
	{
		job[i].finishtime = systime + job[i].reqtime;
		systime = job[i].finishtime;
		job[i].TAtime = job[i].finishtime - job[i].arrtime;
		job[i].TAWtime = job[i].TAtime / job[i].reqtime;
	}
	printf("\n作业名 作业到达时间 作业完成时间 运行时间 作业周转时间 带权作业周转时间\n");
	for (i = 1; i < recordjobnum; i++)
		printf("%4s%12.2f%12.2f%12.2f%12.2f%12.2f\n", job[i].jobname, job[i].arrtime, job[i].finishtime, job[i].reqtime, job[i].TAtime, job[i].TAWtime);

	for (i = 1; i < recordjobnum; i++)
	{
		sum = sum + job[i].TAtime;//计算作业周转时间的总和
		sum1 = sum1 + job[i].TAWtime;//计算作业带权周转时间的总和
	}
	tim[recordATA].ATAtime = sum / recordjobnum;//计算该算法所需的平均周转时间
	tim[recordATA].ATAWtime = sum1 / recordjobnum;//计算该算法所需的平均带权周转时间
	printf("\n平均周转时间是=%.2f\n", tim[recordATA].ATAtime);
	printf("平均带权周转时间是=%.2f\n", tim[recordATA].ATAWtime);
	recordATA = recordATA + 1;
	systime = 0;
}



void HRRF()//最高响应比HRRF
{
	int i, j;
	float arrtemp, reqtemp, hrrftemp;
	float sum = 0, sum1 = 0;
	char nametemp[10];
	sort();//根据到达系统时间进行排序
	printf("\n作业名 作业到达时间 作业运行所需要时间\n");
	for (i = 0; i < recordjobnum; i++)
		printf("\n%4s%12.2f%12.2f", job[i].jobname, job[i].arrtime, job[i].reqtime);
	//计算作业的最高响应比
	for (i = 1; i < recordjobnum; i++)
	{
		printf("\n作业名 最高响应比\n");
		for (j = i; j < recordjobnum; j++)
		{
			job[j].hrrf = 1 + (job[j - 1].reqtime - job[j].arrtime + job[j - 1].arrtime) / job[j].reqtime;
			printf("%4s%12.2f\n", job[j].jobname, job[j].hrrf);
		}
	}
	//根据最高响应比进行降序处理
	for (i = 1; i < recordjobnum; i++)
	{
		for (j = i + 1; j < recordjobnum; j++)
		{
			if (job[i].hrrf < job[j].hrrf)
			{
				arrtemp = job[i].arrtime;
				reqtemp = job[i].reqtime;
				strcpy(nametemp, job[i].jobname);
				hrrftemp = job[i].hrrf;
				job[i].arrtime = job[j].arrtime;
				job[i].reqtime = job[j].reqtime;
				job[i].hrrf = job[j].hrrf;
				strcpy(job[i].jobname, job[j].jobname);
				job[j].arrtime = arrtemp;
				job[j].reqtime = reqtemp;
				job[j].hrrf = hrrftemp;
				strcpy(job[j].jobname, nametemp);
			}
		}
	}
	//计算作业到达系统的时间，作业周转时间，带权作业周转时间
	for (i = 1; i < recordjobnum; i++)
	{
		job[i].finishtime = systime + job[i].reqtime;
		systime = job[i].finishtime;
		job[i].TAtime = job[i].finishtime - job[i].arrtime;
		job[i].TAWtime = job[i].TAtime / job[i].reqtime;
	}
	printf("\n作业名 作业到达时间 作业完成时间 运行时间 作业周转时间 带权作业周转时间\n");
	for (i = 1; i < recordjobnum; i++)
		printf("%4s%12.2f%12.2f%12.2f%12.2f%12.2f\n", job[i].jobname, job[i].arrtime, job[i].finishtime, job[i].reqtime, job[i].TAtime, job[i].TAWtime);

	for (i = 1; i < recordjobnum; i++)
	{
		sum = sum + job[i].TAtime;//计算作业周转时间的总和
		sum1 = sum1 + job[i].TAWtime;//计算作业带权周转时间的总和
	}
	tim[recordATA].ATAtime = sum / recordjobnum;//计算该算法所需的平均周转时间
	tim[recordATA].ATAWtime = sum1 / recordjobnum;//计算该算法所需的平均带权周转时间
	printf("\n平均周转时间是=%.2f\n", tim[recordATA].ATAtime);
	printf("平均带权周转时间是=%.2f\n", tim[recordATA].ATAWtime);
	recordATA = recordATA + 1;
	systime = 0;
}
void arithmeticmenu()//算法菜单
{
	printf("\n\n\t\t***************************\n");
	printf("\t\t\t1、FCFS算法调度\n");
	printf("\t\t\t2、SJF算法调度\n");
	printf("\t\t\t3、HRRF算法调度\n");
	printf("\t\t\t4、调度系统清屏\n");
	printf("\t\t\t0、退出算法调度\n");
	printf("\t\t***************************\n");
	printf("\n\n请选择菜单项；");
}
void choosearithmetic()//选择算法
{
	int n;
	while (1)
	{
		arithmeticmenu();
		scanf("%d", &n);
		if (n == 0)
			break;
		switch (n)
		{
		case 1:
			FCFS(); break;
		case 2:
			SJF(); break;
		case 3:
			HRRF(); break;
		case 4:
			system("cls"); break;
		case 0:
			n = 0; break;
		}
	}
}

int main()
{
	int n;

	while (1)
	{
		menu();
		scanf("%d", &n);
		if (n == 0)
			break;
		switch (n)
		{
		case 1:
			InputData(); break;
		case 2:
			InsertData(); break;
		case 3:
			DeleteData(); break;
		case 4:
			choosearithmetic(); break;
		case 0:
			n = 0; break;

		}

		getch();
	}
}