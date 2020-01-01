//Copyright github.com/charmingjohn

#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <string>
#include "stdlib.h"
#include "string.h"

using namespace std;

#include "station.h"
#include "stack.h"
#include "queue.h"
#include "heap.h"

Heap heap(100);

station station_instance;

int main(int argc, char** argv)
{
	ofstream out;
	out.open("output.txt");
	// Build 개시
	out << "Build" << endl;
	for (int i = 1; i < argc-1; i++) {
		station_instance.readfile(i, argv[i]);
	}
	out << "Build complete" << endl;
	
	// 명령을 읽어들인다.
	ifstream ifs;
	ifs.open(argv[argc - 1]);
	while (!ifs.eof()) {
		string inbuf;
		getline(ifs, inbuf);
		if (inbuf.compare("") == 0)
			break;
		stringstream stream(inbuf);
		string command;
		stream >> command;
		int key;
		stream >> key; // 우선순위
		string source;
		stream >> source; // 출발지, 또는 질문할 역
		string dest;
		stream >> dest; // 도착지
		heap.insert(inbuf, command, key, source, dest);
	}
	ifs.close();

	
	// input.txt에서 시키는대로 수행하자
	while (!heap.isEmpty()) {
		heap_entry he = heap.output();
		heap.extractmin();
		
		if (strcmp(he.command, "Build") == 0) {
			// 무시한다.
		}
		else if (strcmp(he.command, "Read") == 0) {
			station_instance.read_command(he.source, out);
		}
		else if (strcmp(he.command, "Short") == 0) {
			station_instance.shortest(he.source, he.dest, out);
		}
		else if (strcmp(he.command, "Least") == 0) {
			station_instance.least(he.source, he.dest, out);
		}
		else {
			out << "Error command: " << he.command << endl;
		}
	}
	out.close();
	

	return 0;
}