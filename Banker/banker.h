#pragma warning(disable:4996)
//typedef int bool;
#define false 0
#define true !false
//ϵͳ�����н�������
#define PROCESSES_NUMBER    5
typedef struct {
	int A;
	int B;
	int C;
	int D;
}RESOURCE;
//����������  Max
RESOURCE Max[PROCESSES_NUMBER] =
{
	{4,1,1,1},
	{0,2,1,2},
	{4,2,1,0},
	{1,1,1,1},
	{2,1,1,0}
};
//�ѷ�����Դ������ Allocation
RESOURCE Allocation[PROCESSES_NUMBER] =
{
	{3,0,1,1},
	{0,1,0,0},
	{1,1,1,0},
	{1,1,0,1},
	{0,0,0,0}
};
//�������
RESOURCE Need[PROCESSES_NUMBER] =
{
	{1,1,0,0},
	{0,1,1,2},
	{3,1,0,0},
	{0,0,1,0},
	{2,1,1,0}
};
//������Դ����
RESOURCE Available = { 2,1,2,0 };
int safe[PROCESSES_NUMBER];

