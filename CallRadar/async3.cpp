// 这段代码可以正常执行，还可以。
// 包含必要的头文件和库
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main_async3()
{
    // 初始化Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // 创建一个TCP套接字
    SOCKET radarSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 设置连接信息
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.199");
    addr.sin_port = htons(5000);

    // 连接到雷达
    connect(radarSocket, (SOCKADDR*)&addr, sizeof(addr));

    // 发送开始命令
    char startCmd[] = { 0x73, 0x62 };
    send(radarSocket, startCmd, sizeof(startCmd), 0);

    // 接收雷达发送的数据
    char recvBuf[1024];
    int bytesRecv;
    while (true)
    {
        bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // 处理雷达发送的数据
            // 注意：这里需要使用异步处理或非阻塞方式来处理数据，以避免客户端被阻塞
            // 从这里开始 =>
            // 创建一个事件对象
            WSAEVENT event = WSACreateEvent();

            // 将事件对象与套接字关联
            WSAEventSelect(radarSocket, event, FD_READ);

            // 等待事件通知
            while (true)
            {
                // 等待事件通知
                DWORD result = WaitForSingleObject((HANDLE)event, 1000);
                if (result == WAIT_OBJECT_0)
                {
                    // 事件已经触发，可以接收数据
                    char recvBuf[1024];
                    int bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
                    if (bytesRecv > 0)
                    {
                        // 处理数据
                        // ...
                        printf("deal data(line:65):%d\n", bytesRecv);
                    }
                    else if (bytesRecv == 0)
                    {
                        // 连接已经断开
                        printf("Connection closed(line:70)\n");
                        break;
                    }
                    else
                    {
                        // 发生错误
                        // 2023.02.19(
                        //printf("recv start1(line:76): %d\n", WSAGetLastError());
                        //break;
                    }
                }
            }
            // <= 到这里结束
        }
        else if (bytesRecv == 0)
        {
            // 连接已经断开
            printf("Connection closed(line:86)\n");
            break;
        }
        else
        {
            // 发生错误
            printf("recv start2(line:92): %d\n", WSAGetLastError());
            break;
        }
    }
    printf("Press any key to send group command...\n");
    getchar();
    // 发送选择组命令
    try {
        char selectCmd[] = { 0x65, 0x31 };
        iResult = send(radarSocket, selectCmd, sizeof(selectCmd), 0);
        std::cerr << "sending select command result(line:101): " << iResult << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending select command(line:104): " << e.what() << std::endl;
    }

    // 发送开始扫描命令
    try {
        char scanCmd[] = { 0x65, 0x2a };
        iResult = send(radarSocket, scanCmd, sizeof(scanCmd), 0);
        std::cerr << "sending scan command result(line:111): " << iResult << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending scan command(line:114): " << e.what() << std::endl;
    }    

    // 等待雷达发送数据
    std::cerr << "等待雷达发送数据: " << std::endl;
    while (true)
    {
        bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // 处理雷达发送的数据
            // 注意：这里需要使用异步处理或非阻塞方式来处理数据，以避免客户端被阻塞
            // 从这里开始 =>
            // 创建一个事件对象
            WSAEVENT event = WSACreateEvent();

            // 将事件对象与套接字关联
            WSAEventSelect(radarSocket, event, FD_READ);

            // 等待事件通知
            while (true)
            {
                // 等待事件通知
                DWORD result = WaitForSingleObject((HANDLE)event, 1000);
                if (result == WAIT_OBJECT_0)
                {
                    //std::cerr << "3: " << std::endl;
                    // 事件已经触发，可以接收数据
                    char recvBuf[1024];
                    int bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
                    if (bytesRecv > 0)
                    {
                        // 处理数据
                        // ...
                    }
                    else if (bytesRecv == 0)
                    {
                        // 连接已经断开
                        printf("Connection closed\n");
                        std::cerr << "1: " << std::endl;
                        break;
                    }
                    else
                    {
                        // 发生错误
                        //printf("recv failed2: %d\n", WSAGetLastError());
                        //std::cerr << "2: " << std::endl;
                        break;
                    }
                }
            }
            // <= 到这里结束
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
            //printf("recv failed(176): %d\n", WSAGetLastError());
            break;
        }
    }
    std::cerr << "99: " << std::endl;
    // 发送停止命令
    char stopCmd[] = { 0x6f, 0x70 };
    try {
        char start_cmd[] = { 0x73, 0x62 };
        iResult = send(radarSocket, stopCmd, sizeof(stopCmd), 0);
        std::cerr << "sending stopCommand ok: " << std::endl;
        char recvbuff[1025] = {0};
        recv(radarSocket, recvbuff, 1025, 0);
        std::cerr << "recvbuff: " << recvbuff << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending data: " << e.what() << std::endl;
    }    

    // 关闭套接字
    closesocket(radarSocket);
    WSACleanup();

    // 清理Winsock
    return 0;
}
