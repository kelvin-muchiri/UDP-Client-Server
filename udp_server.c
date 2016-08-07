#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define BUFLEN 512
#define SERVER_PORT 5555
int main() {

	SOCKET master = INVALID_SOCKET;
	struct sockaddr_in server, client;
	char recvbuf[BUFLEN] = "";
	int iResult;
	WSADATA wsaData;
	int count;
	char str[INET_ADDRSTRLEN];
	char *message = "I am server";
	int addrlen = sizeof(client);

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	printf("Winsock initialized\n");

	//create socket
	master = socket(AF_INET, SOCK_DGRAM, 0);
	if (master == INVALID_SOCKET) {
		wprintf(L"master socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	printf("Socket created\n");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERVER_PORT);

	//bind 
	if (bind(master, (struct sockaddr *)&server, sizeof(server)) < 0) {

		printf("bind() failed : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Binding done\n");

	while (TRUE) {

		
		fflush(stdout);

		//clear the buffer
		memset(recvbuf, '\0', BUFLEN);

	    //receive data
		if ((count = recvfrom(master, recvbuf, BUFLEN, 0, (struct sockaddr *) &client, &addrlen))
		== SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		printf("Received from client , ip %s,  port %d \n",
			inet_ntop(AF_INET, &(client.sin_addr), str, INET_ADDRSTRLEN), ntohs(client.sin_port));

		printf("Data: %s\n", recvbuf);

		if (sendto(master, message, (int)strlen(message), 0, (struct sockaddr*) &client, addrlen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		

	}//end while

	 // close the socket
	iResult = closesocket(master);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();

return 0;
}//end main