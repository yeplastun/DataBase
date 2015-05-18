#include <sys/types.h>
#include <sys/socket.h>
//#include <netintet/in.h>
#include <netdb.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <string.h>

using namespace std;

void reverse(string s) {
	unsigned long int i, j;
	char c;
	for (i = 0, j = s.size() - 1; i < j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
	return;
}

string itoa(int n) {
	string s;
	s.clear();
	char c;
	int i = 0, sign;
	(void)i;
	(void) sign;
	do {
		c = n % 10 + '0';
		s.push_back(c);
	} while ((n /= 10) > 0);
	c = '\0';
	s.push_back(c);
	reverse(s);
	return s;
}


int main (int argc, char* argv[]) {
	(void)argc;
	string ofname;
	ofname = "request_";
	int request_number = 1;
	int code;
	char message[1024];
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
		bzero( message, sizeof(message));
		//fgets (message, 1024, stdin);
		cin >> message;
		cout << "Read!" << endl;
		len = strlen(message) - 1;
		message[len] = 0;
		if ( strcmp(message, "end") == 0 ) break;
		write(s, &len, sizeof(int));
		write(s, message, len );

		recv(s, &code, sizeof(int), MSG_WAITALL);
		//cout << "code: " << code << endl;

		if (code < 0) {
			recv(s, &len, sizeof(int), MSG_WAITALL);
			recv(s, message, len, MSG_WAITALL);
			message[len] = 0;
			printf("message is : %s\n", message );
		}
		if (code == 1) {
			string str,ofname_tmp;
			str = itoa(request_number);
			ofname_tmp += str;
			FILE* output;
			output = fopen (ofname_tmp.c_str(), "w");
			int kol;
			recv(s, &kol, sizeof(int), MSG_WAITALL);
			for (int i = 0; i < kol; i++) {
				recv(s, &len, sizeof(int), MSG_WAITALL);
				recv(s, message, len, MSG_WAITALL);
				message[len] = 0;
				fprintf(output, "%s", message );
				for (int q = 0; q < (8 - len); q++) {
					fprintf (output, " ");
				}
			}
			fprintf(output, "\n");
			int kol1;
			int z = 0;
			(void) z;
			recv(s, &kol1, sizeof(int), MSG_WAITALL);
			//kol1 *= kol;
			for (int z = 0; z < kol1; z++) {
				for (int k = 0; k < kol; k++) {
					recv(s, &len, sizeof(int), MSG_WAITALL);
					if (len < 0) {
						recv(s, &int_message, sizeof(int), MSG_WAITALL);
						int rzd = 1;
						int s_kol = 1;
						while ((int_message / rzd) >= 1) {
							rzd *= 10;
							s_kol++;
						}
						fprintf(output, "%d", int_message);
						if (s_kol > 10)
							fprintf(output, "  ");
						else {
							for (int q = 0; q < (11 - s_kol); q++) {
								fprintf(output, " ");
							}
						}
					}
					else {
						recv(s, message, len, MSG_WAITALL);
						message[len] = 0;
						if (len > 10)
							fprintf(output, "%s  ", message );
						else  {
							fprintf(output, "%s", message );
							for (int q = 0; q < 11 - len; q++)
								fprintf(output, " ");
						}
					}
				}
				fprintf(output, "\n");
			}
			fclose(output);
			request_number++;
		}
	}
	return 0;
}
