#include "data.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>

using namespace std;

int main () {
	clock_t start, end;
	cout << "--------------------------------------------------------------------------";
	cout << endl;
	start = clock();
	generate(10000,"raw.txt");
	end = clock();
	cout << "end of generating: ";
	cout << ((double) end - start)/ ((double) CLOCKS_PER_SEC) << endl;
	start = clock();
	database d;
	request req("import raw.txt",0);
	d.Import(req);
	cout << d.size() << endl;
	end = clock();
	cout << "end of creating base: ";
	cout << ((double) end - start)/ ((double) CLOCKS_PER_SEC) << endl;
	cout << "--------------------------------------------------------------------------" << endl;
	// int code;
	int Port = 1234;
	int s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	int yes = 1;
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) == -1 );
	sockaddr_in MyAddr;
	memset(&MyAddr, 0, sizeof(sockaddr_in));
	MyAddr.sin_family = AF_INET;
	MyAddr.sin_port = htons(Port);
	MyAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if ( ::bind(s, (sockaddr*) &MyAddr, sizeof(MyAddr)) < 0 )
		perror("bind error\n");
	listen (s,10);
	char buf[1024];
	long long len;
	ssize_t bytes_read;
	set<int> clients;
	clients.clear();
	//clients.insert(s1);
	fd_set rfds;
	struct timeval tv;
	int retval;
	while(1) {
		FD_ZERO(&rfds);
		FD_SET(s, &rfds);
		FD_SET(0, &rfds);
		for(set<int>::iterator it = clients.begin(); it != clients.end(); it++)
			FD_SET(*it, &rfds);
		tv.tv_sec = 5;
		tv.tv_usec = 0;
		retval = select(1234, &rfds, NULL, NULL, &tv);
		(void)retval;
		if (FD_ISSET(0, &rfds)) {
			cout << "!!!" << endl;
			fgets (buf, 1024, stdin);
			len = strlen(buf) - 1;
			buf[len] = 0;
			string cmd;
			cmd = buf;
			if (cmd == "end") break;
		}
		if (FD_ISSET(s, &rfds)) {
			int s2;
			sockaddr_in CliAddr;
			socklen_t AddrLen;
			if ( (s2 = accept(s, (struct sockaddr*) &CliAddr, &AddrLen)) < 0 )
				perror("accept error\n");
			else {
				cout << "Client connected" << endl;
				clients.insert(s2);
				int len;
				recv(s2, &len, sizeof(int), MSG_WAITALL);
				//cout << len << endl;
				bytes_read = recv(s2, buf, len, MSG_WAITALL);
				buf[len] = 0;
				printf("message is: %s\n", buf);
			}
			continue;
		} 
		// cout << "Number of clients: " << clients.size() << endl;
		for(set<int>::iterator it = clients.begin(); it != clients.end(); it++) {
			// cout << "Trying to find a mistake" << endl;
			if(FD_ISSET(*it, &rfds)) {
				bytes_read = recv(*it, &len, sizeof(int), MSG_WAITALL);
				if(bytes_read <= 0) {
					cout << "Client disconnected" << endl;
					close(*it);
					clients.erase(*it);
					d.DeleteClient(*it);
					continue;
				}
				bytes_read = recv(*it, buf, len, MSG_WAITALL);
				buf[len] = 0;
				request RT;
				string str;
				str = buf;
				cout << str << endl;
				RT.get_request(str, *it);
				if (RT.empty()) {
					string mist = "Mistake!\n";
					write(*it, mist.c_str(), mist.length());
					//cout << err_analyze (err) << endl;
					continue;
				}
				cout << RT;
				start = clock();
				d.Execute(RT);
				end = clock();
				cout << "last request: ";
				cout << ((double) end - start)/ ((double) CLOCKS_PER_SEC);
				cout << " seconds" << endl;
			}
		}
	}
	close(s);
	return 0;
}
