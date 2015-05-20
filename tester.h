#ifndef DataBase_tester_h
#define DataBase_tester_h

#include <string.h>
#include <fstream>
#include <cstdlib>

using std::string;
using std::ofstream;
using std::endl;

void generate(int size, string name) {
	ofstream out(name.c_str());
	srand (unsigned(time(NULL)));
	for (int i = 0; i < size; i++) {
		char name[22];
		int name_length = rand() % 5 + 6;
		for (int j = 0; j < name_length; j++) {
			name[j] = rand() % 26 + 'A';
		}
		name[name_length] = 0;
		int group = rand() % 100 + 1;
		double rating = 1 + (double)rand() / RAND_MAX;
		char info[27];
		int info_length = rand() % 5 + 6;
		for (int j = 0; j < info_length; j++) {
			info[j] = rand() % 26 + 'A';
		}
		info[info_length] = 0;
		out << name << ' ' << group << ' ';
		out << rating << ' ' << info << endl;
	}
}

#endif
