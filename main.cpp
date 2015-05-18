#include "tester.h"
#include <ctime>

#define DATA_SIZE 1000

using namespace std;

void print_groups(database& students) {
	for (int i = 0; i < DATA_SIZE; i++) {
		list<cell*> group = *students.get_group(i);
		if (group.size() > 0) {
			cout << "group N " << i << " :" << endl;
			for(list<cell*>::iterator it = group.begin(); 
					it != group.end(); it++)
				cout << **it;
		}
	}
	cout << students.size() << endl;
}

int main() {

	database d;
	d.Interface(std::cin,std::cout);

	return 0;
}
