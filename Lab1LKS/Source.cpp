
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <ctime>

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")
char* sword_del(char* s);
char* nullify(char* s, int rad);

using namespace std;

void main(void) {
	char namePC[30], clientName[30], message[200], serverMessage[200], serverName[30], IP[16] = { 0 };
	string chatMessage;
	WSAData WSADat;
	sockaddr_in sin; 
	SOCKET sock, client; 
	int flag = 0;
	bool closeFlag = false, clientDisconnected = false, diconnectFromServer = false;

	printf("Server - 1\nClient - 2\n");
	cin >> flag;
	system("cls");
	if (flag == 1) {
		clock_t start;

		start = clock();

		WSAStartup(0x0202, &WSADat); 
		gethostname(namePC, 30); 

		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = 0;  
		sin.sin_port = htons(2803); 

		sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bind(sock, (sockaddr*)&sin, sizeof(sin)); 
		listen(sock, SOMAXCONN); 
		do {
			printf("Wait of client...\n");
			client= accept(sock, (sockaddr*)&sin, 0); 
			printf("clientAccepted\n");
			recv(client, clientName, 30, 0); 
			send(client, namePC, strlen(namePC) + 1, 0); 
			system("cls");
			printf("client( \"%s\" ) has connected!\n", clientName);

			int errorCheck;
			do {
				if ((errorCheck = recv(client, message, 200, 0) != 0)) {

					printf("%s\n", message);
					chatMessage = message;
				
					if (chatMessage.compare(0, 4, "quit") == 0) {
						closeFlag = true;
					}
					if (chatMessage.compare(0, 4, "time") == 0) {
					
						char time[30];
						_itoa_s(((clock() - start) / (double)CLOCKS_PER_SEC), time, 10);
						strcat_s(time, " seconds\0");
						send(client, time, strlen(time), 0);
					}
					if (chatMessage.compare(0, 4, "echo") == 0) {
						
						char* new_message = sword_del(message);
						if (strlen(new_message) == 0) {
							new_message = "\n";
						}
						send(client, new_message, strlen(new_message), 0);
					}
					if (chatMessage.compare(0, 10, "disconnect") == 0) {
						clientDisconnected = true;
					}
			
				}
				if (WSAGetLastError() == WSAECONNRESET) {
					clientDisconnected = true;
				}

				nullify(message, 199);
				nullify(serverMessage, 199);
			} while (!closeFlag && !clientDisconnected);
			closesocket(client);
			system("cls");
			clientDisconnected = false;
		} while (!closeFlag);
		closesocket(sock);
		WSACleanup();
	}
	else {

		WSAStartup(0x0202, &WSADat);
		gethostname(namePC, 30);
		do{
			printf("Enter server's IP: ");
			scanf_s("%15s", IP, 16);

			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = inet_addr(IP); 
			sin.sin_port = htons(2803); 

			sock = socket(AF_INET, SOCK_STREAM, 0); 
			printf("Connecting to server...\n");
			system("cls");

			if (connect(sock, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR) {
				printf("Connection error!\n");
				closesocket(sock);
				WSACleanup();
			}
			else {
				send(sock, namePC, strlen(namePC) + 1, 0);
				recv(sock, serverName, 30, 0);

				printf("Connected to \"%s\"!\n", serverName);

				int errorCheck;
				do {
					getline(cin, chatMessage);
					strcpy_s(message, chatMessage.c_str());
					if (send(sock, message, strlen(message) + 1, 0) != SOCKET_ERROR) {
						if (chatMessage.compare(0, 4, "time") == 0) {
							recv(sock, serverMessage, 30, 0);
							std::cout << serverMessage << endl;
						}
						if (chatMessage.compare(0, 4, "echo") == 0) {
							recv(sock, serverMessage, 30, 0);
							if (serverMessage[0] != '\n') {
								std::cout << serverMessage << endl;
							}
						}
						if (chatMessage.compare(0, 10, "disconnect") == 0) {
							diconnectFromServer = true;
						}
					}
					else {
						printf("Error of sending!\n");
					}
					if (chatMessage.compare(0, 4, "quit") == 0) {
						closeFlag = true;
					}
					nullify(message, 199);
					nullify(serverMessage, 199);
					std::cout << "> ";
				} while (!closeFlag && !diconnectFromServer);
				closesocket(sock);
				diconnectFromServer = false;
				system("cls");
			}
		} while (!closeFlag);
		WSACleanup();
	}
}

char* sword_del(char* s) {
	char* p, *t = s;

	while (isspace(*s))
		++s;
	p = s;
	while (*p && !isspace(*p))
		++p;
	p++;
	while ((*s = *p) != '\0') {
		++p;
		++s;
	}
	return t;
}

char* nullify(char* s, int rad) {
	for (int i = 0; i < rad; i++) {
		s[i] = '\0';
	}
	return s;
}