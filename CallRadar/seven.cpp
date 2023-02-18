// VC++ TCP编程实例
// https://blog.csdn.net/Think88666/article/details/101309731
// 
// TcpClient.cpp : 定义控制台应用程序的入口点。
// 

#include "stdafx.h"		// 网上下载的
#include <tchar.h>
#include <winsock2.h>
#pragma comment(lib, "WS2_32")
#include <iostream>
#include "Message.h"

using namespace std;

//const char C_START_CMD[] = { 0x73, 0x62 };
//const char C_STOP_CMD[] = { 0x6f, 0x70 };
//const char C_SCAN_CMD[] = { 0x65, 0x2a };
//const char C_SELEA_CMD[] = { 0x65, 0x31 };

int NRecvData(SOCKET sock, char* data, int maxlen = -1)
{
	//先发送消息头，再发送消息体
	int nBodyLen = 0;
	int ret = recv(sock, (char*)&nBodyLen, 4, 0);
	if (4 == ret && nBodyLen > 0 && (maxlen == -1 || maxlen >= nBodyLen))
	{
		ret = recv(sock, data, nBodyLen, 0);
	}
	return ret;
}

int SendData(SOCKET sock, char* data, int len)
{
	//先发送头再发送消息体
	int ret = send(sock, (char*)&len, 4, 0);
	if (ret == 4)
	{
		ret = send(sock, data, len, 0);
	}
	return ret;
}



int main7(int argc, _TCHAR* argv[])
{
	char C_START_CMD[] = { 0x73, 0x62 };
	char C_STOP_CMD[] = { 0x6f, 0x70 };
	char C_SCAN_CMD[] = { 0x65, 0x2a };
	char C_SELEA_CMD[] = { 0x65, 0x31 };

	//1.启动SOCKET库，版本为1.1
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		cout << "Socket1.1 init error，Exit!";
		return 0;
	}
	//似乎是版本有误
	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 0;
	}

	//2.创建socket，
	SOCKET sock;                   //socket
	sock = socket(
		AF_INET,           // ipv4   internetwork: UDP, TCP, etc
		SOCK_STREAM,        //TCP
		0                  //protocol
	);
	if (sock == INVALID_SOCKET)
	{
		cout << "invalide socket,exit" << endl;
		WSACleanup();
		return 0;
	}
	SOCKADDR_IN addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_addr.S_un.S_addr = inet_addr("192.168.1.199");
	addr_in.sin_port = htons(5000);

	int ret = connect(sock, (SOCKADDR*)&addr_in, sizeof(SOCKADDR));
	if (ret == SOCKET_ERROR)
	{
		cout << "connect error!" << endl;
		WSACleanup();
		return 0;
	}
	//SendData(sock, C_START_CMD, sizeof(C_START_CMD));
	//stChat chat;
	unsigned long chat;
	//while (true)
	//{
		//收发消息
		//cin >> chat.szContent;
		ret = SendData(sock, C_START_CMD, sizeof(C_START_CMD));
		if (SOCKET_ERROR == ret)
		{
			cout << "send error" << endl;
			//break;
		}
		//chat.Clear();
		// 开始尝试读一个字节
		char buff[1];
		recv(sock, buff, 1, 0);
		// 读到一个字节，执行ioctlsocket函数
		unsigned long n;
		ioctlsocket(sock, FIONBIO, &n);

		// 这个是时候拿到了缓冲区内剩余字节的数量，然后便用这个数去读剩下的全部
		char* buff_2 = new char[n + 1];	// 创建的数组要比目标数组多1为，最后一个字节为\0，否则会出现乱码
		ret = recv(sock, buff_2, n, 0);

		//ret = NRecvData(sock, buff_2, n);
		if (SOCKET_ERROR == ret)
		{
			cout << "recv error" << endl;
			//break;
		}
		ret = SendData(sock, C_SELEA_CMD, sizeof(C_SELEA_CMD));
		ret = SendData(sock, C_SCAN_CMD, sizeof(C_SCAN_CMD));
		ret = SendData(sock, C_STOP_CMD, sizeof(C_STOP_CMD));

		closesocket(sock);
		WSACleanup();

		buff_2[n] = '\0';	// 把字节数组最后一位变为\0，防止乱码
		/*if (chat.nID == Msg_Chat)
			cout << chat.szContent << endl;
		chat.Clear();*/
	//}

	
	std::system("pause");
	return 0;
}

//// TcpClient.cpp : 定义控制台应用程序的入口点。
////
//
//#include "stdafx.h"
//#include <winsock2.h>
//#include <tchar.h>
//#pragma comment(lib, "WS2_32")
//#include <iostream>
//#include "Message.h"
//
//using namespace std;
//
//int NRecvData(SOCKET sock, char* data, int maxlen = -1)
//{
//	//先发送消息头，再发送消息体
//	int nBodyLen = 0;
//	int ret = recv(sock, (char*)&nBodyLen, 4, 0);
//	if (4 == ret && nBodyLen > 0 && (maxlen == -1 || maxlen >= nBodyLen))
//	{
//		ret = recv(sock, data, nBodyLen, 0);
//	}
//	return ret;
//}
//
//int SendData(SOCKET sock, char* data, int len)
//{
//	//先发送头再发送消息体
//	int ret = send(sock, (char*)&len, 4, 0);
//	if (ret == 4)
//	{
//		ret = send(sock, data, len, 0);
//	}
//	return ret;
//}
//
//
//
//int main(int argc, _TCHAR* argv[])
//{
//
//	//1.启动SOCKET库，版本为1.1
//	WORD wVersionRequested;
//	WSADATA wsaData;
//	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	int err;
//	wVersionRequested = MAKEWORD(1, 1);
//	err = WSAStartup(wVersionRequested, &wsaData);
//	if (err != 0)
//	{
//		cout << "Socket1.1 init error，Exit!";
//		return 0;
//	}
//	//似乎是版本有误
//	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
//	{
//		WSACleanup();
//		return 0;
//	}
//
//	//2.创建socket，
//	SOCKET sock;                   //socket
//	sock = socket(
//		AF_INET,           // ipv4   internetwork: UDP, TCP, etc
//		SOCK_STREAM,        //TCP
//		0                  //protocol
//	);
//	if (sock == INVALID_SOCKET)
//	{
//		cout << "invalide socket,exit" << endl;
//		WSACleanup();
//		return 0;
//	}
//	SOCKADDR_IN addr_in;
//	memset(&addr_in, 0, sizeof(addr_in));
//	addr_in.sin_family = AF_INET;
//	addr_in.sin_addr.S_un.S_addr = inet_addr("192.168.1.199");
//	addr_in.sin_port = htons(5000);
//
//	int ret = connect(sock, (SOCKADDR*)&addr_in, sizeof(SOCKADDR));
//	if (ret == SOCKET_ERROR)
//	{
//		cout << "connect error!" << endl;
//		WSACleanup();
//		return 0;
//	}
//	
//	//iResult = recv(sock, recvBuf, sizeof(recvBuf), 0);
//
//	// 雷达的开始命令
//	BYTE cmd1[2] = { 0x73, 0x62 };
//	send(sock, (char*)cmd1, sizeof(cmd1), 0);
//
//	std::string res;
//	char buff[1];
//	recv(sock, buff, 1, 0);
//	// 读到一个字节，执行ioctlsocket函数
//	unsigned long n;
//	ioctlsocket(sock, FIONBIO, &n);
//	std::cout << n << std::endl;
//	// 这个是时候拿到了缓冲区内剩余字节的数量，然后便用这个数去读剩下的全部
//	char* buff_2 = new char[n + 1];	// 创建的数组要比目标数组多1为，最后一个字节为\0，否则会出现乱码
//	recv(sock, buff_2, n, 0);
//	// 最后别忘了要跟第一个字节拼一下
//	buff_2[n] = '\0';	// 把字节数组最后一位变为\0，防止乱码
//	//res += buff + buff_2;
//	res += buff;
//	std::cout << res << std::endl;
//
//	// 雷达的选择命令
//	BYTE cmdSelectA[2] = { 0x65, 0x31 };
//	//iResult = send(sock, cmdSelectA, sizeof(cmdSelectA), 0);
//	iResult = send(sock, (char*)cmdSelectA, sizeof(cmdSelectA), 0);
//	if (iResult == SOCKET_ERROR)
//	{
//		cout << "send failed with error: " << WSAGetLastError() << endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//	//char recvBuf[1024];
//	//iResult = recv(sock, recvBuf, sizeof(recvBuf), 0);
//
//
//	// 雷达的单次扫描
//	BYTE cmdScanSingle[2] = { 0x65, 0x2a };
//	//iResult = send(sock, cmdScanSingle, sizeof(cmdScanSingle), 0);
//	iResult = send(sock, (char*)cmdScanSingle, sizeof(cmdScanSingle), 0);
//	if (iResult == SOCKET_ERROR)
//	{
//		cout << "send failed with error: " << WSAGetLastError() << endl;
//		closesocket(sock);
//		WSACleanup();
//		return 1;
//	}
//
//	BYTE cmd[2] = { 0x6f, 0x70 };
//	send(sock, (char*)cmd, sizeof(cmd), 0);
//	closesocket(sock);
//	WSACleanup();
//	//stChat chat;
//	//while (true)
//	//{
//	//	//收发消息
//	//	cin >> chat.szContent;
//	//	ret = SendData(sock, (char*)&chat, sizeof(chat));
//	//	if (SOCKET_ERROR == ret)
//	//	{
//	//		cout << "send error" << endl;
//	//		break;
//	//	}
//	//	chat.Clear();
//	//	ret = NRecvData(sock, (char*)&chat, sizeof(chat));
//	//	if (SOCKET_ERROR == ret)
//	//	{
//	//		cout << "recv error" << endl;
//	//		break;
//	//	}
//	//	if (chat.nID == Msg_Chat)
//	//		cout << chat.szContent << endl;
//	//	chat.Clear();
//	//}
//
//	/*closesocket(sock);
//	WSACleanup();*/
//	/*system("pause");*/
//	return 0;
//}
