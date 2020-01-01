//Copyright github.com/charmingjohn

#include "node.h"

class queue
{
private:
	int size;
	node* front;
	node* rear;
public:
	queue();
	~queue();
	void enqueue(int data);
	int dequeue();
	bool isempty();
};