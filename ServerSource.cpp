#pragma comment (lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <thread>

std::vector<SOCKET> connections(8);

void clientHandler(int index_)
{
	char msg[256];
	while (connections.at(index_)!=0)
	{
		if (recv(connections.at(index_), msg, sizeof(msg), 0) < 0)
			break;
		for (size_t i = 0; i < connections.size(); i++)
		{
			if (!connections.at(i))
				break;
			if (i == index_)
				continue;
			send(connections.at(i), msg, sizeof(msg), 0);
		}
	}
}
int main(int argc, char* argv[])
{
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

	int sizeofAddr = sizeof(addr);

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, 0);
	bind(sListen, (SOCKADDR*)&addr, sizeofAddr);
	listen(sListen, SOMAXCONN);
	
	std::vector<std::thread> clientHandlerList;
	SOCKET newConnection;
	for (size_t i = 0; i < connections.size(); i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofAddr);
		if (!newConnection)
			std::cout << "error\n";
		else
		{
			std::cout << "Client connected\n";
			char msg[256] = "Hello, its works";
			send(newConnection, msg, sizeof(msg), 0);
		}
		connections.at(i) = newConnection;
		clientHandlerList.push_back(std::thread(clientHandler,i));
		clientHandlerList.back().detach();
	}
	system("pause");
	return 0;
}