#include <iostream>
#include <fstream>
#include <iomanip>
#include<iostream>
using namespace std;
const int defMaxJobNumber = 5;   //��ҵ���������ֵ
class Time   //ʱ����
{
public:
	int hour;
	int minute;
};
class Job   //��ҵ��
{
public:
	int ID;   //��ҵ���
	char JobName[2];	//��ҵ����
	Time enter;   //����ʱ��
	int requesttime;   //��������ʱ��
	Time start;   //��ʼʱ��
	Time end;   //����ʱ��
	int Ttime;   //��תʱ��
	double WTtime;   //��Ȩ��תʱ�� 
};
class schedule   //������
{
private:
	int size;   //��ҵ��
	Job *job;   //��ҵ����
	int *r;   //����������

	int Differ(Time t1, Time t2)   //������ʱ�̼��ʱ���
	{
		int borrow = (t2.minute<t1.minute) ? 1 : 0;
		return ((t2.hour - t1.hour - borrow) * 60 + (borrow * 60 + t2.minute - t1.minute));
	}

	void show()   //���������
	{
		double averageTime = 0.0,   //ƽ����תʱ��
			wAverageTime = 0.0;   //��Ȩƽ����תʱ��
		cout << "���  ��ҵ����  ����ʱ��  ����ʱ��  ��ʼʱ��  ����ʱ��  ��תʱ��  ��Ȩ��תʱ��" << endl;
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
		cout << "��ҵƽ����תʱ��T=" << averageTime / size << endl;
		cout << "��ҵ��Ȩƽ����תʱ��W=" << wAverageTime / size << endl << endl;
	}
public:
	schedule()   //���캯��
	{
		size = 0;
		job = new Job[defMaxJobNumber];
		r = new int[defMaxJobNumber - 1];
	}

	void readFile()   //���ļ�����Ϣ
	{
		ifstream txtfile;
		txtfile.open("a.txt");
		if (!txtfile)
		{
			cerr << "�ı��ļ���ʧ��!" << endl;
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

		cout << "���  ��ҵ����  ����ʱ��  ����ʱ��" << endl;
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

	void FCFS()   //�����ȷ���(First Come First Serve)
	{
		int hour, minute, carry;
		cout << "�����ȷ�����ҵ�����㷨������:" << endl;
		job[0].start = job[0].enter;//��һ����ҵ����Ŀ�ʼʱ��
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
		cout << "��ҵ����˳��" << job[0].JobName << "->" << job[1].JobName << "->" << job[2].JobName << "->" << job[3].JobName << "->" << job[4].JobName << endl;
		show();
	}

	void SJF()   //����ҵ����(Shortest Job First)
	{
		int hour, minute, carry;
		cout << "����ҵ������ҵ�����㷨������:" << endl;
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
		for (int i = 1; i<size - 1; i++)   //������ҵ����ʱ��ӵ͵�������
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
		cout << "��ҵ����˳��" << job[0].JobName << "->" << job[r[1]].JobName << "->" << job[r[2]].JobName << "->" << job[r[3]].JobName << "->" << job[r[4]].JobName << endl;
		show();
	}

	void HRN()   //�����Ӧ������(Highest Response_ratio Next)��ҵ��תʱ��/��ҵ����ʱ��
	{
		int hour, minute, carry;
		cout << "�����Ӧ��������ҵ�����㷨������:" << endl;
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
		cout << "��ҵ����˳��" << job[0].JobName << "->" << job[r[1]].JobName << "->" << job[r[2]].JobName << "->" << job[r[3]].JobName << "->" << job[r[4]].JobName << endl;
		show();
	}
};

int main()
{
	schedule s;
	s.readFile();   //���ļ�����Ϣ
	s.FCFS();   //�����ȷ���(First Come First Serve)
	s.SJF();   //����ҵ����(Shortest Job First)
	s.HRN();   //�����Ӧ������(Highest Response_ratio Next)
	system("pause");
	return 0;
}