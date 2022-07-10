#include "VoiceUDPSender.h"
#include <string>

void VoiceUDPSender::StartUp()
{
	WSACleanup();
	WSAStartup(MAKEWORD(2, 0), &wsaData);   //MAKEWORD(2, 0)��winsock�̃o�[�W����2.0���Ă���
	sock = static_cast<int>(socket(AF_INET, SOCK_DGRAM, 0));  //AF_INET��IPv4�ASOCK_DGRAM��UDP�ʐM�A0�́H
	addr.sin_family = AF_INET;
	addr.sin_port = htons(50008);// �҂��󂯃|�[�g�ԍ���50008�ɂ���
	std::string add = "127.0.0.1";
	addr.sin_addr.S_un.S_addr = inet_addr(add.c_str());// ���M�A�h���X��ݒ�
}

void VoiceUDPSender::SendData()
{
	for (int i = 0; i < 100; i++)
	{
		char data[] = "test";
		//�f�[�^���M
		sendto(sock, data, sizeof(data), 0, (struct sockaddr*)&addr, sizeof(addr));//addr�ɕ����񑗐M
		printf("send data!\n");
	}
}

void VoiceUDPSender::EndSend()
{
	closesocket(sock);
	WSACleanup();
}
