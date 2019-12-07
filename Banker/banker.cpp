#include <stdio.h>
#include <string.h>
#include "banker.h"

//��̽����
void ProbeAlloc(int process, RESOURCE* res)
{  
	Available.A -= res->A;
	Available.B -= res->B;
	Available.C -= res->C;
	Available.D -= res->D;
	Allocation[process].A += res->A;
	Allocation[process].B += res->B;
	Allocation[process].C += res->C;
	Allocation[process].D += res->D;
	Need[process].A -= res->A;
	Need[process].B -= res->B;
	Need[process].C -= res->C;
	Need[process].D -= res->D;
}

//����̽�������벻��ȫ״̬��������ع�
void RollBack(int process, RESOURCE* res)
{
	Available.A += res->A;
	Available.B += res->B;
	Available.C += res->C;
	Available.D += res->D;
	Allocation[process].A -= res->A;
	Allocation[process].B -= res->B;
	Allocation[process].C -= res->C;
	Allocation[process].D -= res->D;
	Need[process].A += res->A;
	Need[process].B += res->B;
	Need[process].C += res->C;
	Need[process].D += res->D;
}

//��ȫ�Լ��
bool SafeCheck()
{
	RESOURCE    Work = Available;
	bool        Finish[PROCESSES_NUMBER] = { false,false,false,false,false };
	int     i;
	int     j = 0;
	for (i = 0; i < PROCESSES_NUMBER; i++)
	{
		//�Ƿ��Ѽ���
		if (Finish[i] == false)
		{
			//�Ƿ����㹻����Դ������ý���
			if (Need[i].A <= Work.A && Need[i].B <= Work.B && Need[i].C <= Work.C && Need[i].D <= Work.D)
			{
				//����ʹ��ִ����ɣ������ѷ�����ý��̵���Դȫ������
				Work.A += Allocation[i].A;
				Work.B += Allocation[i].B;
				Work.C += Allocation[i].C;
				Work.D += Allocation[i].D;
				Finish[i] = true;
				safe[j++] = i;
				i = -1;             //���½��б���
			}
		}
	}
	//������н��̵�Finish������Ϊtrue���ڰ�ȫ״̬������Ϊ����ȫ״̬
	for (i = 0; i < PROCESSES_NUMBER; i++)
	{
		if (Finish[i] == false)
		{
			return false;
		}
	}
	return true;
}
//��Դ��������
bool request(int process, RESOURCE* res)
{
	//request������С��Need�����ж�Ӧ������
	if (res->A <= Need[process].A && res->B <= Need[process].B && res->C <= Need[process].C && res->D <= Need[process].D)
	{
		//request������С��Available����
		if (res->A <= Available.A && res->B <= Available.B && res->C <= Available.C && res->D <= Available.D)
		{
			//��̽����
			ProbeAlloc(process, res);
			//�����ȫ��������������ɹ������򽫷���ع�������ʧ��
			if (SafeCheck())
			{
				return true;
			}
			else
			{
				printf("��ȫ�Լ��ʧ�ܡ�ԭ��ϵͳ�����벻��ȫ״̬���п�������������\n");
				printf("���ڻع�...\n");
				RollBack(process, res);
			}
		}
		else
		{
			printf("��ȫ�Լ��ʧ�ܡ�ԭ�������������ڿ�������Դ������\n");
		}
	}
	else
	{
		printf("��ȫ�Լ��ʧ�ܡ�ԭ������������������������\n");
	}
	return false;
}
//�����Դ�����
void PrintTable()
{
	printf("\t\t\t*********��Դ�����*********\n");
	printf("Process             Max              Allocation               Need                  Available\n");
	printf("          A    B    C    D     A    B    C    D      A    B    C    D     A    B    C   D\n");
	printf("  P0      %d    %d    %d    %d     %d    %d    %d    %d      %d    %d    %d    %d     %d    %d    %d   %d\n", Max[0].A, Max[0].B, Max[0].C, Max[0].D, Allocation[0].A, Allocation[0].B, Allocation[0].C, Allocation[0].D, Need[0].A, Need[0].B, Need[0].C, Need[0].D, Available.A, Available.B, Available.C, Available.D);
	printf("  P1      %d    %d    %d    %d     %d    %d    %d    %d      %d    %d    %d    %d\n", Max[1].A, Max[1].B, Max[1].C, Max[1].D, Allocation[1].A, Allocation[1].B, Allocation[1].C, Allocation[1].D, Need[1].A, Need[1].B, Need[1].C, Need[1].D);
	printf("  P2      %d    %d    %d    %d     %d    %d    %d    %d      %d    %d    %d    %d\n", Max[2].A, Max[2].B, Max[2].C, Max[2].D, Allocation[2].A, Allocation[2].B, Allocation[2].C, Allocation[2].D, Need[2].A, Need[2].B, Need[2].C, Need[2].D);
	printf("  P3      %d    %d    %d    %d     %d    %d    %d    %d      %d    %d    %d    %d\n", Max[3].A, Max[3].B, Max[3].C, Max[3].D, Allocation[3].A, Allocation[3].B, Allocation[3].C, Allocation[3].D, Need[3].A, Need[3].B, Need[3].C, Need[3].D);
	printf("  P4      %d    %d    %d    %d     %d    %d    %d    %d      %d    %d    %d    %d\n", Max[4].A, Max[4].B, Max[4].C, Max[4].D, Allocation[4].A, Allocation[4].B, Allocation[4].C, Allocation[4].D, Need[4].A, Need[4].B, Need[4].C, Need[4].D);
	printf("\n");
}
int main()
{
	int ch;
	printf("�ȼ���ʼ״̬�Ƿ�ȫ��\n");
	if (SafeCheck())
	{
		printf("ϵͳ���ڰ�ȫ״̬��\n");
		printf("��ȫ������{P%d,P%d,P%d,P%d,P%d}��\n", safe[0], safe[1], safe[2], safe[3], safe[4]);
	}
	else
	{
		printf("ϵͳ���ڲ���ȫ״̬��\n");
	}
	while(true){
		int     process;
		RESOURCE    res;
		PrintTable();
		printf("�����������������Ľ��̺Ͷ�������Դ������������");
		scanf("%d%d%d%d%d", &process, &res.A, &res.B, &res.C, &res.D);
		if (request(process, &res))
		{
			printf("����ɹ���\n");
			printf("��ȫ������{P%d,P%d,P%d,P%d,P%d}��\n", safe[0], safe[1], safe[2], safe[3], safe[4]);
			if (Need[process].A == 0 &&
				Need[process].B == 0 &&
				Need[process].C == 0 &&
				Need[process].D == 0)
			{
				Available.A +=Allocation[process].A;
				Available.B += Allocation[process].B;
				Available.C += Allocation[process].C;
				Available.D += Allocation[process].D;
				Allocation[process].A =
					Allocation[process].B =
					Allocation[process].C =
					Allocation[process].D = 0;
			}
		}
		else
		{
			printf("����ʧ�ܡ�\n");
		}
		printf("ִ����ϡ�\n");
}
	return 0;
}
