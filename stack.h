//Copyright github.com/charmigjohn

class stack
{
private:
	int* stack_n;
	int top;
	int size;
public:
	stack(int s);
	~stack();
	void push(int data);
	int pop();
	int _top();
	bool isempty();
};