#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")//加载 ws2_32.dll
#pragma warning(disable:4996)

const char START_COMMAND[] = { 0x73, 0x62 };
const char STOP_COMMAND[] = { 0x6f, 0x70 };
const char SCAN_COMMAND[] = { 0x65, 0x2a };
const char* IP_ADDRESS4 = "192.168.1.199";

int main4()
{
    // 初始化 Windows Sockets API
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置雷达地址和端口
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS4);
    serverAddress.sin_port = htons(5000);

    iResult = connect(ConnectSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    /*iResult = closesocket(ConnectSocket);
    std::cerr << "disconnect" << WSAGetLastError() << std::endl;*/
    char sendbuff[] = { 0x73, 0x62 };
    iResult = send(ConnectSocket, sendbuff, (int)strlen(sendbuff), 0);

    // Send the stop command
    iResult = send(ConnectSocket, START_COMMAND, sizeof(START_COMMAND), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Send the stop command
    iResult = send(ConnectSocket, STOP_COMMAND, sizeof(STOP_COMMAND), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
}