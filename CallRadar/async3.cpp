// 包含必要的头文件和库
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // 初始化Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // 创建一个TCP套接字
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 设置连接信息
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.199");
    addr.sin_port = htons(5000);

    // 连接到雷达
    connect(s, (SOCKADDR*)&addr, sizeof(addr));

    // 发送开始命令
    char startCmd[] = { 0x73, 0x62 };
    send(s, startCmd, sizeof(startCmd), 0);

    // 接收雷达发送的数据
    char recvBuf[1024];
    int bytesRecv;
    while (true)
    {
        bytesRecv = recv(s, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // 处理雷达发送的数据
            // 注意：这里需要使用异步处理或非阻塞方式来处理数据，以避免客户端被阻塞
        }
        else if (bytesRecv == 0)
        {
            // 连接已经断开
            printf("Connection closed\n");
            break;
        }
        else
        {
            // 发生错误
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    // 发送选择组命令
    char selectCmd[] = { 0x65, 0x31 };
    send(s, selectCmd, sizeof(selectCmd), 0);

    // 发送开始扫描命令
    char scanCmd[] = { 0x65, 0x2a };
    send(s, scanCmd, sizeof(scanCmd), 0);

    // 等待雷达发送数据
    while (true)
    {
        bytesRecv = recv(s, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // 处理雷达发送的数据
            // 注意：这里需要使用异步处理或非阻塞方式来处理数据，以避免客户端被阻塞
        }
        else if (bytesRecv == 0)
        {
            // 连接已经断开
            printf("Connection closed\n");
            break;
        }
        else
        {
            // 发生错误
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    // 发送停止命令
    char stopCmd[] = { 0x6f, 0x70 };
    send(s, stopCmd, sizeof(stopCmd), 0);

    // 关闭套接字
    closesocket(s);
    WSACleanup();

    // 清理Winsock
    return 0;
}
