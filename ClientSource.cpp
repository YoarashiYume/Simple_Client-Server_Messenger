#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <thread>

SOCKET connection;

void clientHandler()
{
	char msg[256];
	while (true)
	{
		if (recv(connection, msg, sizeof(msg), 0) >= 0)
			std::cout << msg << std::endl;
		else
			break;
	}
}

int main(int argc, char* argv[])
{
	//add library
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0)
	{
		std::cout << "Error with library\n";
		exit(1);
	}
	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	connection = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(connection, (SOCKADDR*)&addr, sizeof(addr)))
	{
		std::cout << "Failed connection\n";
		return -1;
	}
	std::cout << "Connected\n";

	std::thread th(clientHandler);
	th.detach();
	char msgi[256];
	while (true)
	{
		std::cin.getline(msgi, sizeof(msgi));
		if (send(connection, msgi, sizeof(msgi), 0) < 0)
			break;
		Sleep(10);
		
	}
	system("pause");
	
	return 0;
}