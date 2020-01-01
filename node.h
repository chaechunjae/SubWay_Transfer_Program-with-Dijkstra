//Copyright github.com/charmingjohn

#include <iostream>
#ifndef _NODE_H
#define _NODE_H
class node
{
public:
	int data;
	node* next;
	node()
	{
		data = -1;
		next = NULL;
	}
};
#endif
