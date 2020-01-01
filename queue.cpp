//Copyright github.com/charmingjohn

#include <iostream>
#include "queue.h"
#include "node.h"
using namespace std;

queue::queue()
{
	front = NULL;
	rear = NULL;
	size = 0;
}

queue::~queue()
{
	while (!isempty())
		dequeue();
}

void queue::enqueue(int data)
{
	node* newnode = new node();
	newnode->data = data;
	if (front == NULL)
	{
		front = newnode;
		rear = front;
	}
	else
	{
		rear->next = newnode;
		rear = newnode;
	}

	size++;
}

int queue::dequeue()
{
	if (isempty())
		cout << "큐가 비었음" << endl;
	else
	{
		node* temp = front;
		front = front->next;
		size--;
		return temp->data;
	}

}

bool queue::isempty()
{
	return front == NULL;
}