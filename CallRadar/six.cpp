#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)

int main6()
{
    // 初始化套接字库
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup failed." << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cout << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    // 连接到雷达
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.199");
    addr.sin_port = htons(5000);
    iResult = connect(sock, (SOCKADDR*)&addr, sizeof(addr));
    // kaishi
    char start_cmd[] = { 0x73, 0x62 };
    iResult = send(sock, start_cmd, sizeof(start_cmd), 0);
    // select
    char select_cmd[] = { 0x65, 0x35 };
    iResult = send(sock, select_cmd, sizeof(select_cmd), 0);
    // sacn
    char scan_cmd[] = { 0x65, 0x2a };
    iResult = send(sock, scan_cmd, sizeof(scan_cmd), 0);
    /*if (iResult == SOCKET_ERROR) {
        std::cout << "Connection failed." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }*/

    //// 选择E组
    //char start_cmd[] = { 0x73, 0x62 };
    //iResult = send(sock, start_cmd, sizeof(start_cmd), 0);
    //if (iResult == SOCKET_ERROR) {
    //    std::cout << "Start command sending failed." << std::endl;
    //    closesocket(sock);
    //    WSACleanup();
    //    return 1;
    //}

    // 关闭套接字和清理套接字库
    closesocket(sock);
    WSACleanup();

    return 0;
}

