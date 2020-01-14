#pragma warning(disable:4996)
//typedef int bool;
#define false 0
#define true !false
//系统中所有进程数量
#define PROCESSES_NUMBER    5
typedef struct {
	int A;
	int B;
	int C;
	int D;
}RESOURCE;
//最大需求矩阵  Max
RESOURCE Max[PROCESSES_NUMBER] =
{
	{4,1,1,1},
	{0,2,1,2},
	{4,2,1,0},
	{1,1,1,1},
	{2,1,1,0}
};
//已分配资源数矩阵 Allocation
RESOURCE Allocation[PROCESSES_NUMBER] =
{
	{3,0,1,1},
	{0,1,0,0},
	{1,1,1,0},
	{1,1,0,1},
	{0,0,0,0}
};
//需求矩阵
RESOURCE Need[PROCESSES_NUMBER] =
{
	{1,1,0,0},
	{0,1,1,2},
	{3,1,0,0},
	{0,0,1,0},
	{2,1,1,0}
};
//可用资源向量
RESOURCE Available = { 2,1,2,0 };
int safe[PROCESSES_NUMBER];

