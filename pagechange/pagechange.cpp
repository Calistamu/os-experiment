/*************************************************************************
		> ��������
		> 20
		> 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
		> 3
 ************************************************************************/
#pragma warning(disable:4996)
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include<time.h>
using namespace std;

class PageReplacement {
private:
	vector<int> page;
	int page_num;  //�����ڴ����
public:
	PageReplacement() : page_num(0) {}
	void create();
	void print();
	void physical_block_print(const vector<int>&);
	bool isfind(const vector<int>&, int);  //�����Ƿ������ڴ��������
	void FIFO();  //FIFOҳ���û��㷨
	void LRU();  //LRUҳ���û��㷨
	void OPT(); //OPTҳ���û��㷨
};

void PageReplacement::create() {
	    int choose;
	    printf("��ѡ��ҳ�������ʽ��1.��� 2.ָ��\n");
		printf("�����룺");
		scanf("%d", &choose);
		if (choose == 1)
		{
			srand((unsigned)(time(NULL)));
			int randtime = rand() % 500 + 1000;
			int n = randtime;
			printf("\n������и�����%d", n);
			int randsize = rand() % 5 + 2;
			page_num = randsize;
			printf("\n������������%d", randsize);
			int randnum;
			while (n--){
			randnum = rand() % 30 + 0;
			page.push_back(randnum);
		}
		}
		else if (choose == 2) {
			cout << "������ҳ�����: ";
			int n, t;
			cin >> n;

			cout << "������ҳ�洮��\n";
			while (n--) {
				cin >> t;
				page.push_back(t);
			}

			cout << "�������ڴ��������: ";
			cin >> page_num;
		}
}

void PageReplacement::print() {
	int n = page.size();
	cout << "ҳ�洮:\n";
	for (int i = 0; i < n; i++) {
		cout << page[i] << ' ';
	}
	cout << endl;
}

void PageReplacement::physical_block_print(const vector<int>& t) {
	int n = t.size();
	for (int i = 0; i < n; i++) {
		cout << t[i] << ' ';
	}
	cout << endl;
}

bool PageReplacement::isfind(const vector<int>& block, int t) {
	if (find(block.begin(), block.end(), t) != block.end()) {
		return true;
	}
	return false;
}

/* FIFOҳ���û��㷨 */
void PageReplacement::FIFO() {
	vector<int> physical_block(page_num, -1);

	int n = page.size(), cur = 0;
	int not_found = 0;
	cout << "FIFOҳ���û����:\n";
	for (int i = 0; i < n; i++) {
		if (!isfind(physical_block, page[i])) {
			physical_block[cur] = page[i];  //ҳ���û�
			cur = (cur + 1) % page_num;
			not_found++;
		}
		physical_block_print(physical_block);  //���ÿ���������Ϣ
	}

	cout << "ȱҳ������" << not_found << endl;
	cout << "ҳ���ܸ���:"<<n<<endl;
	float a = (not_found*1.0 / n)*100;
	printf("%.2f%%",a);
}

/* LRUҳ���û��㷨 */
void PageReplacement::LRU() {
	vector<int> physical_block(page_num, -1);
	map<int, int> page_time;
	int n = page.size();
	int not_found = 0;

	for (int i = 0; i < n; i++) {
		if (!isfind(physical_block, page[i])) {  //��ǰҳ����Ϊ�������
			if (page_time.size() < page_num) { //�����δ����
				physical_block[i] = page[i];
				page_time[page[i]] = i;
			}
			else {  //��������������ҳ���û�
				map<int, int>::iterator it = page_time.begin(), t = it;
				for (it++; it != page_time.end(); it++) {
					if (t->second > it->second) {
						t = it;
					}
				}
				int index;
				for (index = 0; index < page_num && physical_block[index] != t->first; index++)
					;
				physical_block[index] = page[i];
				page_time.erase(t);
				page_time[page[i]] = i;
			}
			not_found++;
		}
		else {  //��������У�����ҳ�����ʹ��ʱ��
			page_time[page[i]] = i;
		}
		physical_block_print(physical_block);
	}

	cout << "ȱҳ������" << not_found << endl;
	cout << "ҳ���ܸ���:" << n << endl;
	float a = (not_found * 1.0 / n) * 100;
	printf("%.2f%%", a);

}

/* OPTҳ���û��㷨 */
void PageReplacement::OPT() {
	vector<int> physical_block(page_num, -1);
	int n = page.size();
	int not_found = 0;

	for (int i = 0; i < n; i++) {
		if (!isfind(physical_block, page[i])) {  //��ǰҳ�����������
			if (i < page_num) {  //�����δ����
				physical_block[i] = page[i];
			}
			else {  //��������,����ҳ���û�
				map<int, int> page_time;
				int k, j;

				/* ��ȡ�������ҳ��˺������ʹ��ʱ�� */
				for (j = 0; j < page_num; j++) { //����physical_block
					for (k = i + 1; k < n; k++) {  //����page
						if (physical_block[j] == page[k]) {
							page_time[page[k]] = k;
							break;
						}
					}
					if (k == n) {
						page_time[physical_block[j]] = k;
					}
				}

				/* Ѱ�ҿ����û���ҳ�� */
				map<int, int>::iterator it = page_time.begin(), t = it;
				for (; it != page_time.end(); it++) {
					if (t->second < it->second) {
						t = it;
					}
				}

				/* ��ȡ���û�ҳ�����������±� */
				int index = 0;
				for (; index < page_num && physical_block[index] != t->first; index++)
					;
				physical_block[index] = page[i];
			}
			not_found++;
		}
		physical_block_print(physical_block);
	}

	cout << "ȱҳ������" << not_found << endl;
	cout << "ҳ���ܸ���:" << n << endl;
	float a = (not_found * 1.0 / n) * 100;
	printf("%.2f%%", a);

}

int main() {
	PageReplacement t;
	t.create();
	while (true) {
		printf("\n---------��ѡ���㷨--------------------------------------------\n");
		printf("1.FIFO 2.LRU 3.OPT 4.����\n");
		printf("�����룺");
		int choose;
		scanf("%d", &choose);
		if (choose == 4)
		{
			printf("�˳�����\n");
			break;
	}
		else {
			switch (choose) {
			case 1:
				t.FIFO();
				break;
			case 2:
				t.LRU();
				break;
			case 3:
				t.OPT();
				break;
			}
		}
	}
	return 0;
}
