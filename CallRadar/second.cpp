#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

const char START_COMMAND[] = { 0x73, 0x62 };
const char STOP_COMMAND[] = { 0x6f, 0x70 };
const char SCAN_COMMAND[] = { 0x65, 0x2a };
//const char RADAR_IP[] = "192.168.1.199";
const char* RADAR_IP = "192.168.1.199";
const int RADAR_PORT = 5000;

int main2()
{
    // 初始化 Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        cout << "WSAStartup failed with error: " << iResult << endl;
        return 1;
    }

    // 创建套接字
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // 填充服务器地址结构体
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;    
    addrServer.sin_addr.s_addr = inet_addr(RADAR_IP);
    addrServer.sin_port = htons(RADAR_PORT);

    // 连接到服务器
    iResult = connect(ConnectSocket, (SOCKADDR*)&addrServer, sizeof(addrServer));
    if (iResult == SOCKET_ERROR)
    {
        cout << "connect failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // 发送开始命令
    iResult = send(ConnectSocket, START_COMMAND, sizeof(START_COMMAND), 0);
    if (iResult == SOCKET_ERROR)
    {
        cout << "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // 发送扫描命令
    iResult = send(ConnectSocket, SCAN_COMMAND, sizeof(SCAN_COMMAND), 0);
    if (iResult == SOCKET_ERROR)
    {
        cout << "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    // 接收服务器的回复
    char recvBuf[1024];
    iResult = recv(ConnectSocket, recvBuf, sizeof(recvBuf), 0);
    if (iResult > 0)
    {
        cout << "Bytes received: " << iResult << endl;
        cout << "Received data: ";
        for (int i = 0; i < iResult; i++)
        {
            cout << hex << (int)recvBuf[i] << " ";
        }
        cout << endl;
    }
    else if (iResult == 0)
    {
        cout << "Connection closed" << endl;
    }
    else
    {
        cout << "recv failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // 发送停止命令
    iResult = send(ConnectSocket, STOP_COMMAND, sizeof(STOP_COMMAND), 0);
    if (iResult == SOCKET_ERROR)
    {
        cout << "send failed with error: " << WSAGetLastError() << endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // 关闭套接字
    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR)
    {
        cout << "closesocket failed with error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // 清理 Winsock
    WSACleanup();

    return 0;
}
