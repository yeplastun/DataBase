#include "tester.h"
#include <ctime>
#include "data.h"

#define DATA_SIZE 1000

using namespace std;

int main() {

	database d;
	generate(1000, "raw.txt");
	request req;
	cout << req.get_request("import raw.txt",0) << endl;
	d.Execute(req);
	cout << req.get_request("remove group = 1 - 99",0) << endl;
	d.Execute(req);
	cout << req.get_request("select group = 100",0) << endl;
	d.Execute(req);
	cout << req.get_request("print name group rating info sort info",0) << endl;
	d.Execute(req);
	for (vector<string>::iterator it = d.printed[0].begin(); it != d.printed[0].end(); ++it) 
		cout << *it << endl;
	cout << d.size() << endl;
	return 0;
	// segmentation fault если select вынужден вернуть пустое значение
}
