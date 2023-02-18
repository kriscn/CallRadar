#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")//���� ws2_32.dll
#pragma warning(disable:4996)

using namespace std;
#define PORT5 5000
#define IP_ADDRESS5 "172.16.20.181"
CRITICAL_SECTION cs;

//����ÿ���ͻ������ӵĴ�����
DWORD WINAPI ClientThread(LPVOID lpParameter);
//���Ӻͷ������������ӵĿͻ���
DWORD WINAPI ConnectClientsThread(LPVOID lpParameter);

const char START_COMMAND[] = { 0x73, 0x62 };
const char STOP_COMMAND[] = { 0x6f, 0x70 };
const char SCAN_COMMAND[] = { 0x65, 0x2a };

int main5() 
{
	InitializeCriticalSection(&cs);

	//��ʼ���¼��͹ؼ��Σ��Զ���λ����ʼ�޴����������¼�
	WSADATA  Ws;
	SOCKET ServerSocket, ClientSocket;
	//TCP/IP �׽���ָ���׽��ֵĵ�ַ
	struct sockaddr_in LocalAddr, ClientAddr;
	int Ret = 0;
	int AddrLen = 0;
	HANDLE hThread = NULL;
	HANDLE hConnectThread = NULL;

	//��ʼ��Winsock2.2.ʹ��WSAStartup����
	//��һ����������Ҫ�õ�Winsock�汾��
	//The MAKEWORD macro creates a WORD value by concatenating the specified values.
	//�ڶ�����������WSADATA �ṹ���ָ�롣�����ʼ���ɹ��򷵻�0
	//Ҫע���κ�WinsockAPI�����������ڳ�ʼ����ʹ�ã����������麯��
	if (WSAStartup(MAKEWORD(2, 2), &Ws) != 0)
	{
		cout << "��ʼ�� Socket ʧ��:" << GetLastError() << endl;
		return -1;
	}

	//Create Socket
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);				   
	if (ServerSocket == INVALID_SOCKET)
	{
		cout << "���� Socket ʧ��:" << GetLastError() << endl;
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
		cout << "�� Socket ʧ��:" << GetLastError() << endl;
		return -1;
	}
}