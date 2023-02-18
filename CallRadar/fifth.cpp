#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")//加载 ws2_32.dll
#pragma warning(disable:4996)

using namespace std;
#define PORT5 5000
#define IP_ADDRESS5 "172.16.20.181"
CRITICAL_SECTION cs;

//接收每个客户端连接的处理函数
DWORD WINAPI ClientThread(LPVOID lpParameter);
//连接和服务器端有连接的客户端
DWORD WINAPI ConnectClientsThread(LPVOID lpParameter);

const char START_COMMAND[] = { 0x73, 0x62 };
const char STOP_COMMAND[] = { 0x6f, 0x70 };
const char SCAN_COMMAND[] = { 0x65, 0x2a };

int main5() 
{
	InitializeCriticalSection(&cs);

	//初始化事件和关键段，自动置位，初始无触发的匿名事件
	WSADATA  Ws;
	SOCKET ServerSocket, ClientSocket;
	//TCP/IP 套接字指定套接字的地址
	struct sockaddr_in LocalAddr, ClientAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;
	HANDLE hConnectThread = NULL;

	//初始化Winsock2.2.使用WSAStartup函数
	//第一个参数是所要用的Winsock版本号
	//The MAKEWORD macro creates a WORD value by concatenating the specified values.
	//第二个参数就是WSADATA 结构体的指针。如果初始化成功则返回0
	//要注意任何WinsockAPI函数都必须在初始化后使用，包括错误检查函数
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		cout << "初始化 Socket 失败:" << GetLastError() << endl;
		return -1;
	}

	//Create Socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);				   
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "创建 Socket 失败:" << GetLastError() << endl;
		system("pause");
		return -1;
	}

	//the address of family specification
	LocalAddr.sin_family = AF_INET;
	//The inet_addr function converts a string containing an (Ipv4) Internet Protocol dotted address into a proper address for the IN_ADDR structure.
	LocalAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS5);
	//The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
	LocalAddr.sin_port = htons(PORT5);
	//Sets buffers to a specified character.
	memset(LocalAddr.sin_zero, 0x00, 8);

	//Bind Socket,The bind function associates a local address with a socket.
	Ret = bind(ServerSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if (Ret != 0)
	{
		cout << "绑定 Socket 失败:" << GetLastError() << endl;
		return -1;
	}
}