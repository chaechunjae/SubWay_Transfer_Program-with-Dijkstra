#include <iostream>

class station_node
{
public:
	station_node(int l, int n)
	{
		line = l;
		name = n;
		next = NULL;
	};

	int line;
	int name;
	
	station_node *next;
};

#define	MAX_STATION	256
#define	MAX_LINE	10

class station
{
public:

	// 각 역간의 거리
	int time_table[MAX_STATION][MAX_STATION];
	// 역과 역을 연결하는 노선 정보
	int line_table[MAX_STATION][MAX_STATION];

	station_node* station_head[MAX_STATION];
	station_node* line_head[MAX_LINE];
	// 특정 역의 노선 번호 정보
	station_node* line_number[MAX_STATION];

	int size[MAX_STATION];
	int dist[MAX_STATION];
	int previous[MAX_STATION];

	station();
	~station();

	void add_station(int line_no, int s1, int s2, int t);
	void readfile(int line_no, char* line1);
	void shortest(int start, int destination, ofstream &out);
	void least(int start, int destination, ofstream &out);
	void read_command(int sta, ofstream &out);
	void follow_line(int line, int sta, ofstream &out);
	void add_line_station(int sta, int line);
	int calculate_expected_length(int cur2, int tname);
};