#include "udp.h"

void UDPClass::SetPortNum(int port)
{
	this->port = port;
}

void UDPClass::StartUp()
{
	WSAStartup(MAKEWORD(2, 0), &wsaData);//MAKEWORD(2, 0)はバージョン
	sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));//AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？

	addr.sin_family = AF_INET;  //IPv4
	addr.sin_port = htons(port);   //通信ポート番号設定
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // INADDR_ANYはすべてのアドレスからのパケットを受信する
	bind(sock, (struct sockaddr*)&addr, sizeof(addr));
	u_long val = 1;
	ioctlsocket(sock, FIONBIO, &val);
}

void UDPClass::Recive()
{
	char buf[100];
	memset(buf, 0, 100);
	int data_size = recv(sock, buf, 100, 0);
	tex = buf;
}

void UDPClass::EndRecive()
{
	closesocket(sock);
	WSACleanup();
}