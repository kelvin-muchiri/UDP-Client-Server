#include <winsock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#define SERVER_PORT 5555
#define BUFLEN 512

int main() {

	SOCKET clientSocket;
	struct sockaddr_in server_addr;
	char recvbuf[BUFLEN];
	char message[BUFLEN];
	int iResult;
	WSADATA wsaData;
	int addrlen = sizeof(server_addr);
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	printf("Winsock initialized\n");


	//create socket
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		wprintf(L"connect socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	printf("socket created\n");

	//setup address structure
	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	//server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//server address
															  //use inet_pton, inet_addr deprecated
	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	}

	while (TRUE)
	{
		printf("Enter message : ");
		gets(message);

		//send the message
		if (sendto(clientSocket, message, (int)strlen(message), 0, (struct sockaddr *) &server_addr, addrlen)
			== SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//clear the buffer
		memset(recvbuf, '\0', BUFLEN);
		
		if (recvfrom(clientSocket, recvbuf, BUFLEN, 0, (struct sockaddr *) &server_addr, &addrlen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		puts(recvbuf);
	}//end while

	 // close the socket
	iResult = closesocket(clientSocket);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"close failed with error: %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	WSACleanup();




	return 0;
}