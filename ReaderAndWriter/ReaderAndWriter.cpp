// ReaderWriter.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<iostream>
#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<io.h>
#include<string.h>
#include<conio.h>
using namespace std;

#define INTE_PER_SEC 100
#define MAX_THREAD_NUM 10
#define Writer 'W'
#define Reader 'R'

struct ThreadInfo {
	int serial;
	char entity;
	int delay;
	int persist;
};

int read_count = 0;
CRITICAL_SECTION mutex, w;

void Reader_Writer(char* file);
void Thread_Reader(void* p);
void Thread_Writer(void* p);

int main()
{
	char s[] = "wr.txt";
	Reader_Writer(s);
	return 0;
}

void Reader_Writer(char* file) {
	DWORD n_thread = 0;
	DWORD thread_ID;

	HANDLE h_thread[MAX_THREAD_NUM];
	ThreadInfo thread_info[MAX_THREAD_NUM];

	ifstream InFile;
	InFile.open(file);
	puts("Read Data File\n");

	while (InFile) {
		InFile >> thread_info[n_thread].serial;
		InFile >> thread_info[n_thread].entity;
		InFile >> thread_info[n_thread].delay;
		InFile >> thread_info[n_thread++].persist;
		InFile.get();
	}

	for (int i = 0; i < (int)n_thread; i++) {
		if (thread_info[i].entity == Writer) {
			h_thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(Thread_Writer), &thread_info[i], 0, &thread_ID);
		}
		else if (thread_info[i].entity == Reader) {
			h_thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(Thread_Reader), &thread_info[i], 0, &thread_ID);
		}
		else {
			puts("Bad File\n");
			exit(0);
		}
	}

	InitializeCriticalSection(&mutex);
	InitializeCriticalSection(&w);

	WaitForMultipleObjects(n_thread, h_thread, TRUE, -1);
	printf("Task is finished!\n");
	_getch();

}

// 读者优先、可以连续读
void Thread_Reader(void* p) {
	int r_serial,r_delay,r_persist;

	r_serial = ((ThreadInfo*)(p))->serial;
	r_delay = ((ThreadInfo*)(p))->delay;
	r_persist = ((ThreadInfo*)(p))->persist;

	while (true) {
		Sleep(r_delay);
		printf("R thread %d send a require\n", r_serial);
		EnterCriticalSection(&mutex);
		read_count++;//读者读进程标志
		if (read_count == 1)
			EnterCriticalSection(&w);
		LeaveCriticalSection(&mutex);
		printf("R thread %d begin to read\n", r_serial);
		Sleep(r_persist);
		printf("R thread %d finish!\n", r_serial);
		EnterCriticalSection(&mutex);
		read_count--;
		if (read_count == 0)
			LeaveCriticalSection(&w);
		LeaveCriticalSection(&mutex);
	}

}

void Thread_Writer(void* p) {
	int w_serial, w_delay, w_persist;

	w_serial = ((ThreadInfo*)(p))->serial;
	w_delay = ((ThreadInfo*)(p))->delay;
	w_persist = ((ThreadInfo*)(p))->persist;

	while (true) {
		Sleep(w_delay);
		printf("W thread %d send a require\n", w_serial);
		EnterCriticalSection(&w);
		printf("W thread %d begin to write\n", w_serial);
		Sleep(w_persist);
		printf("W thread %d finish!\n", w_serial);
		LeaveCriticalSection(&w);
	}
}