#pragma once
#include <WinSock2.h> //windows
#include <string>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

class UDPClass
{
	WSAData wsaData;
	u_short port;
	int sock;
	//アドレスの格納
	struct sockaddr_in addr;
	//受け取った結果の格納
	std::string tex;
public:
	void SetPortNum(int port);
	void StartUp();
	void Recive();
	void EndRecive();
	std::string GetResult() {return tex;}
};