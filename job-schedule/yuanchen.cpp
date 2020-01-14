#include <iostream>
#include <fstream>
#include <iomanip>
#include<iostream>
using namespace std;
const int defMaxJobNumber = 5;   //作业数量的最大值
class Time   //时间类
{
public:
	int hour;
	int minute;
};
class Job   //作业类
{
public:
	int ID;   //作业编号
	char JobName[2];	//作业名称
	Time enter;   //进入时间
	int requesttime;   //估计运行时间
	Time start;   //开始时间
	Time end;   //结束时间
	int Ttime;   //周转时间
	double WTtime;   //带权周转时间 
};
class schedule   //调度类
{
private:
	int size;   //作业数
	Job *job;   //作业数组
	int *r;   //排序用数组

	int Differ(Time t1, Time t2)   //求两个时刻间的时间差
	{
		int borrow = (t2.minute<t1.minute) ? 1 : 0;
		return ((t2.hour - t1.hour - borrow) * 60 + (borrow * 60 + t2.minute - t1.minute));
	}

	void show()   //输出计算结果
	{
		double averageTime = 0.0,   //平均周转时间
			wAverageTime = 0.0;   //带权平均周转时间
		cout << "编号  作业名称  进入时间  运行时间  开始时间  结束时间  周转时间  带权周转时间" << endl;
		for (int i = 0; i<size; i++)
		{
			cout << setw(4) << job[i].ID << setw(7) << job[i].JobName << setw(7) << job[i].enter.hour << ":";
			if (job[i].enter.minute<10)
				cout << "0" << job[i].enter.minute;
			else
				cout << job[i].enter.minute;
			cout << setw(10) << job[i].requesttime << setw(7) << job[i].start.hour << ":";
			if (job[i].start.minute<10)
				cout << "0" << job[i].start.minute;
			else
				cout << job[i].start.minute;
			cout << setw(7) << job[i].end.hour << ":";
			if (job[i].end.minute<10)
				cout << "0" << job[i].end.minute;
			else
				cout << job[i].end.minute;
			cout << setw(10) << job[i].Ttime << setw(14) << job[i].WTtime << endl;
			averageTime += job[i].Ttime;
			wAverageTime += job[i].WTtime;
		}
		cout << "作业平均周转时间T=" << averageTime / size << endl;
		cout << "作业带权平均周转时间W=" << wAverageTime / size << endl << endl;
	}
public:
	schedule()   //构造函数
	{
		size = 0;
		job = new Job[defMaxJobNumber];
		r = new int[defMaxJobNumber - 1];
	}

	void readFile()   //从文件读信息
	{
		ifstream txtfile;
		txtfile.open("a.txt");
		if (!txtfile)
		{
			cerr << "文本文件打开失败!" << endl;
			exit(1);
		}
		int i = 0;
		int entertime;
		while (!txtfile.eof())
		{
			txtfile >> job[i].ID >> job[i].JobName >> entertime >> job[i].requesttime ;
			job[i].enter.hour = entertime / 100;
			job[i].enter.minute = entertime % 100;
			i++;
			size++;
		}
		txtfile.close();

		cout << "编号  作业名称  进入时间  运行时间" << endl;
		for (i = 0; i<size; i++)
		{
			cout << setw(4) << job[i].ID << setw(7) << job[i].JobName << setw(7) << job[i].enter.hour << ":";
			if (job[i].enter.minute<10)
				cout << "0" << job[i].enter.minute;
			else
				cout << job[i].enter.minute;
			cout << setw(10) << job[i].requesttime << endl;
		}
		cout << endl;
	}

	void FCFS()   //先来先服务(First Come First Serve)
	{
		int hour, minute, carry;
		cout << "先来先服务作业调度算法计算结果:" << endl;
		job[0].start = job[0].enter;//第一道作业到达的开始时间
		hour = job[0].requesttime / 60;
		minute = job[0].requesttime % 60;
		job[0].end.minute = (job[0].start.minute + minute) % 60;
		carry = (job[0].start.minute + minute) / 60;
		job[0].end.hour = job[0].start.hour + hour + carry;
		job[0].Ttime = job[0].requesttime;
		job[0].WTtime = ((double)job[0].Ttime) / job[0].requesttime;
		for (int i = 1; i<size; i++)
		{
			job[i].start = job[i - 1].end;
			hour = job[i].requesttime / 60;
			minute = job[i].requesttime % 60;
			job[i].end.minute = (job[i].start.minute + minute) % 60;
			carry = (job[i].start.minute + minute) / 60;
			job[i].end.hour = job[i].start.hour + hour + carry;
			job[i].Ttime = Differ(job[i].enter, job[i].end);
			job[i].WTtime = ((double)job[i].Ttime) / job[i].requesttime;
		}
		cout << "作业调度顺序：" << job[0].JobName << "->" << job[1].JobName << "->" << job[2].JobName << "->" << job[3].JobName << "->" << job[4].JobName << endl;
		show();
	}

	void SJF()   //短作业优先(Shortest Job First)
	{
		int hour, minute, carry;
		cout << "短作业优先作业调度算法计算结果:" << endl;
		job[0].start = job[0].enter;
		hour = job[0].requesttime / 60;
		minute = job[0].requesttime % 60;
		job[0].end.minute = (job[0].start.minute + minute) % 60;
		carry = (job[0].start.minute + minute) / 60;
		job[0].end.hour = job[0].start.hour + hour + carry;
		job[0].Ttime = job[0].requesttime;
		job[0].WTtime = ((double)job[0].Ttime) / job[0].requesttime;
		for (int i = 1; i<size; i++)
			r[i] = i;
		for (int i = 1; i<size - 1; i++)   //按照作业运行时间从低到高排序
		{
			int index = i;
			for (int j = i + 1; j<size; j++)
				if (job[r[j]].requesttime<job[r[index]].requesttime)
					index = j;
			if (index != i)
			{
				int w = r[i];
				r[i] = r[index];
				r[index] = w;
			}
		}

		int dest = 0;
		for (int i = 1; i<size; i++)
		{
			int diff = Differ(job[r[i]].enter, job[dest].end);
			int tmp;
			if(diff<0)
			{
				tmp = r[i];
				r[i] = r[i + 1];
				r[i + 1] = tmp;
			}

			int index = r[i];
			job[index].start = job[dest].end;
			hour = job[index].requesttime / 60;
			minute = job[index].requesttime % 60;
			job[index].end.minute = (job[index].start.minute + minute) % 60;
			carry = (job[index].start.minute + minute) / 60;
			job[index].end.hour = job[index].start.hour + hour + carry;
			job[index].Ttime = Differ(job[index].enter, job[index].end);
			job[index].WTtime = ((double)job[index].Ttime) / job[index].requesttime;
			dest = index;
		}
		cout << "作业调度顺序：" << job[0].JobName << "->" << job[r[1]].JobName << "->" << job[r[2]].JobName << "->" << job[r[3]].JobName << "->" << job[r[4]].JobName << endl;
		show();
	}

	void HRN()   //最高响应比优先(Highest Response_ratio Next)作业周转时间/作业处理时间
	{
		int hour, minute, carry;
		cout << "最高响应比优先作业调度算法计算结果:" << endl;
		job[0].start = job[0].enter;
		hour = job[0].requesttime / 60;
		minute = job[0].requesttime % 60;
		job[0].end.minute = (job[0].start.minute + minute) % 60;
		carry = (job[0].start.minute + minute) / 60;
		job[0].end.hour = job[0].start.hour + hour + carry;
		job[0].Ttime = job[0].requesttime;
		job[0].WTtime = ((double)job[0].Ttime) / job[0].requesttime;

		int dest = 0;
		for (int i = 1; i<size; i++)
			r[i] = i;
		for (int i = 1; i<size; i++)
		{
			int index = i;
			for (int j = i + 1; j<size; j++)
				if (((double)Differ(job[r[j]].enter, job[dest].end)) / job[r[j]].requesttime>
					((double)Differ(job[r[index]].enter, job[dest].end)) / job[r[index]].requesttime)
					index = j;
			if (index != i)
			{
				int w = r[i];
				r[i] = r[index];
				r[index] = w;
			}
			index = r[i];
			job[index].start = job[dest].end;
			hour = job[index].requesttime / 60;
			minute = job[index].requesttime % 60;
			job[index].end.minute = (job[index].start.minute + minute) % 60;
			carry = (job[index].start.minute + minute) / 60;
			job[index].end.hour = job[index].start.hour + hour + carry;
			job[index].Ttime = Differ(job[index].enter, job[index].end);
			job[index].WTtime = ((double)job[index].Ttime) / job[index].requesttime;
			dest = index;
		}
		cout << "作业调度顺序：" << job[0].JobName << "->" << job[r[1]].JobName << "->" << job[r[2]].JobName << "->" << job[r[3]].JobName << "->" << job[r[4]].JobName << endl;
		show();
	}
};

int main()
{
	schedule s;
	s.readFile();   //从文件读信息
	s.FCFS();   //先来先服务(First Come First Serve)
	s.SJF();   //短作业优先(Shortest Job First)
	s.HRN();   //最高响应比优先(Highest Response_ratio Next)
	system("pause");
	return 0;
}