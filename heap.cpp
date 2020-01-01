//Copyright github.com/charmingjohn


#include <iostream>
#include "heap.h"

using namespace std;

Heap::Heap(int Capacity) :capacity(Capacity)
{
	heap = new heap_entry[capacity];
	last = 0;
}

Heap::~Heap()
{
	delete[] heap;
}

void Heap::swap(int a, int b)
{
	heap_entry t = heap[a];
	heap[a] = heap[b];
	heap[b] = t;
}

void Heap::insert(string lbuf, string cmd, int key, string src, string dst)
{
	if (last == capacity)
	{
		cout << "힙이 가득 찼습니다." << endl;
		return;
	}

	strcpy_s(heap[last].linebuf, 256, lbuf.c_str());
	strcpy_s(heap[last].command, 32, cmd.c_str());
	heap[last].key = key;
	heap[last].source = src[0];
	if(dst.compare("") != 0)
		heap[last].dest = dst[0];
	int current = last;
	int parent = int((last - 1) / 2);

	while (heap[current].key < heap[parent].key)
	{
		swap(current, parent);
		current = parent;
		parent = int((current - 1) / 2);
	}
	last++;
}

void Heap::extractmin()
{
	if (last == 0)
		return;
	int parent = 0, leftchild = 1, rightchild = 2;

	last--;
	swap(0, last);
	while (true)
	{
		int selectchild = 0;//0부터 찾아내려감
		if (leftchild >= last)
			break;//이미 최소값이므로 바꿀 필요가 없음
		if (rightchild >= last)
			selectchild = leftchild;//index가 2까지 밖에 없는 경우 최소값을 heap[2]로 바꿔줌.
		else{
			if (heap[leftchild].key > heap[rightchild].key)//더 작은 값을 비교하기 위해 사용
				selectchild = rightchild;
			else
				selectchild = leftchild;
		}
		if (heap[selectchild].key < heap[parent].key){
			swap(parent, selectchild);
			parent = selectchild;//부모값을 현재 값으로 설정.
		}
		else break;

		leftchild = 2 * parent + 1;//부모값에 대해 왼쪽 child
		rightchild = leftchild + 1;//부모값에 대해 오른쪽 child
	}
}

heap_entry Heap::output()
{
	return heap[0];
}


int Heap::isEmpty()
{
	return last == 0;
}
