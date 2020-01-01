//Copyright github.com/charmingjohn

#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <string>


using namespace std;

#include "station.h"
#include "queue.h"
#include "stack.h"

station::station()
{
	memset(time_table, 0, sizeof(time_table));
	memset(station_head, 0, sizeof(station_head));
	memset(line_head, 0, sizeof(line_head));
	memset(size, 0, sizeof(size));
	memset(line_number, 0, sizeof(line_number));

}

station::~station()
{
	// 사용한 메모리 해제
	for (int i = 0; i < MAX_STATION; i++) {
		station_node* p = station_head[i];
		while (p != NULL) {
			station_node* q = p->next;
			delete p;
			p = q;
		}
	}

	// 노선도 출력에 필요할 거 같아서 준비했으나 미구현(무쓸모)
	for (int i = 0; i < MAX_LINE; i++) {
		station_node* p = line_head[i];
		while (p != NULL) {
			station_node* q = p->next;
			delete p;
			p = q;
		}
	}

	// 각 역이 속해있는 노선이 여러개라 리스트로 구현
	for (int i = 0; i < MAX_STATION; i++) {
		station_node* p = line_number[i];
		while (p != NULL) {
			station_node* q = p->next;
			delete p;
			p = q;
		}
	}
}

void station::add_station(int line_no, int name1, int name2, int t)
{
	time_table[name1][name2] = t;
	time_table[name2][name1] = t;
	
	line_table[name1][name2] = line_no;
	line_table[name2][name1] = line_no;

	if (station_head[name1] == NULL){
		station_node *node = new station_node(line_no, name1);
		station_head[name1] = node;
	}
	station_node* node2 = new station_node(line_no, name2);
	station_node* p1 = station_head[name1];
	int inserted = 0;
	while (p1->next != NULL) {
		if (name2 < p1->next->name)
			//인접리스트로 주변 역들을 이어줌. 이 때 알파벳 순서로 인접하게 만든다.
			//최단거리가 같을 경우 갈림길에서 알파벳 순서가 낮은 역으로 먼저 가게 하기 위함.
		{
			node2->next = p1->next;
			p1->next = node2;
			inserted = 1;
			break;
		}
		p1 = p1->next;
	}
	if(!inserted) // 삽입이 안 됐으면 뒤에 추가
		p1->next = node2;

	if (station_head[name2] == NULL) {
		station_node *node = new station_node(line_no, name2);
		station_head[name2] = node;
	}
	station_node* node1 = new station_node(line_no, name1);
	station_node* p2 = station_head[name2];
	inserted = 0;
	while (p2->next != NULL){
		if (name1 < p2->next->name)
		{
			node1->next = p2->next;
			p2->next = node1;
			inserted = 1;
			break;
		}
		p2 = p2->next;
	}
	if (!inserted)
		p2->next = node1;
}

void station::readfile(int line_no, char* line1)
{
	int last_time = 0;
	int last_station = 0;

	ifstream line_1;
	line_1.open(line1);
	string station;
	int time = 0;
	while (!line_1.eof())
	{
		getline(line_1, station);
		if (station.compare("") == 0)
			break;
		stringstream stream(station);
		stream >> station;
		stream >> time;
		int name = station[0];
		add_line_station(name, line_no);
		if (last_station != 0)
			add_station(line_no, last_station, name, last_time);

		station_node *nodel = new station_node(line_no, name);
		if (line_head[line_no] == NULL)
			line_head[line_no] = nodel;
		else { // 끝에 붙여준다.
			station_node *p = line_head[line_no];
			while (p->next != NULL)
				p = p->next;
			p->next = nodel;
		}
		size[name]++;//환승역인지 판단 (size[]=2인 경우 환승역)
		last_time = time;
		last_station = name;
	}
	line_1.close();


}

void station::shortest(int start, int destination, ofstream &out)
{
	station_node *p = line_number[start];
	station_node *p2 = line_number[destination];
	if (p == NULL || p2 == NULL) {
		if (p == NULL)
		{
			out << (char)start << " not exists in the graph" << endl;
		}
		if (p2 == NULL)
		{
			out << (char)destination << " not exists in the graph" << endl;
		}
		return;
	}
	//다익스트라 알고리즘 사용
	stack stack1(MAX_STATION);
	queue queue1;

	for (int i = 0; i < MAX_STATION; i++)
	{
		dist[i] = INT_MAX;
		previous[i] = 0;
	}

	dist[start] = 0;
	queue1.enqueue(start);
	int cur=0;

	int prev_line = 0;
	int current_line = 0;

	while (!queue1.isempty())
	{
		cur = queue1.dequeue();
		station_node* target = station_head[cur]->next;
	//	if (cur == destination)
		//	break;

		while (target != NULL)
		{
			int length = dist[cur] + time_table[cur][target->name];
			if (length < dist[target->name])
			{
				queue1.enqueue(target->name);
				previous[target->name] = cur;
				dist[target->name] = length;
			}
			target = target->next;
		}
	}

	int cur1 = destination;
	stack1.push(cur1);
	while (previous[cur1] != 0) {
		cur1 = previous[cur1];
		stack1.push(cur1);
	}

	out << "Short " << (char)start << " to " << (char)destination << endl;

	prev_line = 0; // 환승 여부를 알기 위한 이전 역까지의 노선 번호.
	int transfer_count = 0; // 환승 횟수
	int prev_station = 0; // 방금 전역의 이름이자 번호.
	//int total_time = 0;

	out << (char)stack1._top(); // 출발역을 표시한다.

	int time = 0;
	int prev_time = 0;

	while (!stack1.isempty())
	{
		int a = stack1.pop();
		if (prev_station != 0) { // 이전 역이 존재하면
			//total_time += time_table[prev_station][a]; // 소요시간 누적.
			int current_line = line_table[prev_station][a]; // 노선표에서 노선 번호를 가져온다.
			if (prev_line != 0 &&               // 이전 노선 번호가 존재하고, 
				current_line != prev_line )     // 이전 노선과 지금 노선이 다르면 환승을 한 것으로 봄.
			{
				// 노선이 변경되었으면 이전의 노선 번호와 환승역을 표시한다. 현재 역은 종점이 아니면 표시할 필요는 없다.
				// 만일 현재역이 종점이라면 저 밑에서 마지막에 표시된다.
				time = dist[prev_station];
				out << "--" << (time - prev_time) % 200 << "--" << (char)prev_station; // 노선출력 샘플
				prev_time = time;
				
				transfer_count++; // 환승횟수를 계산해두자. 나중에 쓰일 수가 있다. 쓸모가 없으면 나중에 삭제해도 된다.
			}
			prev_line = current_line; // 현재 노선으로 이전 노선을 바꿔주고
		}

		prev_station = a; // 현재 역이 이전 역이 된다.
	}
	// 마지막 역과 그 역까지의 노선 번호를 표시한다.
	// 맨 마지막 역 정보를 prev_station이 가지고 있다.
	out << "--" << (dist[prev_station]-prev_time)%200 << "--" << (char)prev_station << endl; // print destination
}

int station::calculate_expected_length(int cur2, int tname)
{//최소환승시 노드까지의 거리를 계산하기 위함.
	stack s3(MAX_STATION);
	s3.push(tname);
	while (cur2 != 0) {
		s3.push(cur2);
		cur2 = previous[cur2];
	}
	
	int length = 0;
	int prev_line = 0;
	int prev = 0;
	while (!s3.isempty()) {
		int curr = s3.pop();
		if (prev != 0) {
			length += time_table[prev][curr];
			int curr_line = line_table[prev][curr];
			if (prev_line != 0 && prev_line != curr_line)
				length += 200; //환승이 발생하면 해당 length(역 사이의 거리)에 200 가중치를 추가하여 환승 가중치를 줌.
			prev_line = curr_line;
		}
		prev = curr;
	}

	return length;
}

void station::least(int start, int destination, ofstream &out)
{

	station_node *p = line_number[start];
	station_node *p2 = line_number[destination];
	if (p == NULL || p2 == NULL) {
		if (p == NULL)
		{
			out << (char)start << " not exists in the graph" << endl;
		}
		if (p2 == NULL)
		{
			out << (char)destination << " not exists in the graph" << endl;
		}
		return;
	}
	

		stack stack2(MAX_STATION);
		queue queue2;

		for (int i = 0; i < MAX_STATION; i++)
		{
			dist[i] = INT_MAX;
			previous[i] = 0;
		}

		dist[start] = 0;
		queue2.enqueue(start);
		int cur2;
		int prev_line = 0;
		while (!queue2.isempty())
		{

			cur2 = queue2.dequeue();
			station_node* target2 = station_head[cur2]->next;


			while (target2 != NULL)
			{
				int current_line = line_table[cur2][target2->name];
				
				int length = calculate_expected_length(cur2, target2->name);
				//가중치 200만큼 증가한 length를 사용한다.
				//즉, 환승을 한 번 할때마다 200만큼 비용이 증가하므로 

				prev_line = current_line;
				if (length < dist[target2->name])
				{
					queue2.enqueue(target2->name);
					previous[target2->name] = cur2;
					dist[target2->name] = length;
				}
				target2 = target2->next;
			}
		}

		int cur1 = destination;
		stack2.push(cur1);
		while (previous[cur1] != 0) {
			cur1 = previous[cur1];
			stack2.push(cur1);
		}

		out << "Least " << (char)start << " to " << (char)destination << endl;

		prev_line = 0; // 환승 여부를 알기 위한 이전 역까지의 노선 번호.
		int transfer_count = 0; // 환승 횟수
		int prev_station = 0; // 방금 전역의 이름이자 번호.
		//int total_time = 0; 쓰려다 말음

		out << (char)stack2._top(); // 출발역을 표시한다.

		int time = 0;
		int prev_time = 0;

		while (!stack2.isempty())
		{
			int a = stack2.pop();
			if (prev_station != 0) { // 이전 역이 존재하면
				//total_time += time_table[prev_station][a]; // 소요시간 누적.
				int current_line = line_table[prev_station][a]; // 노선표에서 노선 번호를 가져온다.
				if (prev_line != 0 &&               // 이전 노선 번호가 존재하고, 
					current_line != prev_line)     // 이전 노선과 지금 노선이 다르면 환승을 한 것이다.
				{
					// 노선이 변경되었으면 이전의 노선 번호와 환승역을 표시한다. 현재 역은 종점이 아니면 표시할 필요는 없다.
					// 만일 현재역이 종점이라면 저 밑에서 마지막에 표시된다.
					time = dist[prev_station] % 200; //200을 더해줬기 때문에 200의 나머지를 사용.
					out << "--" << time << "--" << (char)prev_station; // 노선출력 샘플이다.
					prev_time = time;

					transfer_count++; // 환승횟수를 계산해두자. 나중에 쓰일 수가 있다. 쓸모가 없으면 나중에 삭제해도 된다.
				}
				prev_line = current_line; // 현재 노선으로 이전 노선을 바꿔주고
			}

			prev_station = a; // 현재 역이 이전 역이 된다.
		}
		// 마지막 역과 그 역까지의 노선 번호를 표시한다.
		// 맨 마지막 역 정보를 prev_station이 가지고 있다.
		out << "--" << (dist[prev_station]%200) - prev_time << "--" << (char)prev_station << endl; // print destination
}

void station::read_command(int sta, ofstream &out)
{
	// 노선 번호를 가져온다
	if (sta < 0 || sta >= MAX_STATION) {
		out << (char)sta << " not exist in the graph" << endl;
		return;
	}

	station_node *p = line_number[sta];
	if (p == NULL) {
		out << (char)sta << " not exist in the graph" << endl;
		return;
	}

	// 노선을 따라간다
	out << "Read " << (char)sta<<endl;
	while (p != NULL) {
		out << "Line number: " << p->line << endl;
		follow_line(p->line, sta, out);
		p = p->next;
	}
}

void station::follow_line(int line, int sta, ofstream &out)
{
	station_node *p = line_head[line];
	int prev_station = 0;
	while (p != NULL) {
		if (p->name == sta) { // 역을 찾았다.
			if (prev_station != 0)
				out << (char)prev_station << "--" << time_table[prev_station][sta] << "--";
			out << (char)sta;
			if (p->next != NULL) {
				station_node* q = p->next;
				out << "--" << time_table[sta][q->name] << "--" << (char)q->name;
			}
			out << endl;
			break;
		}
		prev_station = p->name;
		p = p->next;
	}
}

void station::add_line_station(int sta, int line)
{
	// 각 역별 노선 번호 정보
	station_node *node = new station_node(line, sta);
	if (line_number[sta] == NULL) {
		line_number[sta] = node;
	} else {
		// 노선 번호 적은 것부터 들어오니까, 항상 마지막에 붙여주면 된다.
		station_node *p = line_number[sta];
		while (p->next != NULL)
			p = p->next;
		p->next = node;
	}
}
