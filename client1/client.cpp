#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string.h>
//#include "data.h"

using namespace std;

int main (int argc, char* argv[]) {
	(void)argc;
	string ofname;
	ofname = "request_";
	int request_number = 1;
	int code;
	//char message[1024];
	string message;
	int int_message;
	sockaddr_in SrvAddr;
	hostent * Host = gethostbyname(argv[1]);
	SrvAddr.sin_family = AF_INET;
	SrvAddr.sin_port = htons(atoi(argv[2]));

	int s = socket(AF_INET, SOCK_STREAM, 0);
	memmove(&(SrvAddr.sin_addr.s_addr), Host->h_addr_list[0], 4);
	printf("Connecting.....\n");
	if (connect(s, (struct sockaddr*) &SrvAddr, sizeof(SrvAddr)) < 0) {
		fprintf(stderr, "Error: cannot connect\n");
		return 0;
	}
	printf("You are connected to server!\n");


	long long len = strlen(argv[3]); //чтоб не было проблем с последним символом +1
	//вроде и не надо +1
	//cout << len << endl;
	write(s, &len, sizeof(int)); //отправляем в sock то, что лежит в &len, sizeof(int) байт
	write(s, argv[3], len);

	while (1) {
		cout << "Write a command:" << endl;
		message.clear();
		//bzero( message, sizeof(message));
		//fgets (message, 1024, stdin);
		getline(cin, message);
		//if ( strcmp(message, "end") == 0 )
		if (message == "end")
			break;
		cout << "Read!" << endl;
		len = message.size();
		message[len] = 0;
		//cout << "len " << len << endl;
		write(s, &len, sizeof(int));
		write(s, message.c_str(), len);
		recv(s, &code, sizeof(int), MSG_WAITALL);
		if (code == -1) {
			cout << "Wrong command!" << endl;
		}
		if (code == 1) {
			int num;
			recv(s, &num, sizeof(int), MSG_WAITALL);
			for (int i = 0; i < num; ++i) {
				int len;
				char tmp[1024];
				recv(s, &len, sizeof(int), MSG_WAITALL);
				recv(s, tmp, len, MSG_WAITALL);
				cout << tmp << endl;
			}
		}
		if (code == 2) break;
	}
	return 0;
}
