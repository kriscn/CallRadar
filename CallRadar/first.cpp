#include <iostream>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")//加载 ws2_32.dll

//添加此句，屏蔽"warning作为error", 提高兼容性
#pragma warning(disable:4996)

using namespace std;

const int BUFF_SIZE = 1024;
const int CMD_START = 0x7362;
const int CMD_STOP = 0x6f70;
const int CMD_SCAN = 0x652a;
const char* IP_ADDRESS1 = "192.168.1.199";
const int PORT = 5000;

int main1() {
    // 初始化 Windows Sockets API
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // 创建套接字
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置雷达地址和端口
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS1);
    serverAddr.sin_port = htons(PORT);    

    // 连接到服务器
    iResult = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    char sendbuff[BUFF_SIZE];
    char recvbuff[BUFF_SIZE];
    std::string userInput;

    do {
        // 读取用户输入
        std::cout << "Enter a command (start, stop, scan): ";
        std::getline(std::cin, userInput);
        int cmd = 0;
        if (userInput == "start") {
            cmd = CMD_START;
        }
        else if (userInput == "stop") {
            cmd = CMD_STOP;
        }
        else if (userInput == "scan") {
            cmd = CMD_SCAN;
        }
        else {
            continue;
        }

        // 把命令转换为字符串
        sprintf_s(sendbuff, "%d", cmd);
        // 发送数据
        try {
            char start_cmd[] = { 0x73, 0x62 };
            iResult = send(sock, start_cmd, sizeof(start_cmd), 0);
        } catch (const std::exception& e) {
             std::cerr << "Error sending data: " << e.what() << std::endl;
        }
        
        if (iResult == SOCKET_ERROR) {
            std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }
        
        // 接收数据
        iResult = recv(sock, recvbuff, BUFF_SIZE, 0);
        if (iResult > 0) {
            // 将接收到的数据转换为字符串
            recvbuff[iResult] = '\0';
            std::cout << "Received: " << recvbuff << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closed" << std::endl;
        }
        else {
            std::cout << "recv failed with error: " << WSAGetLastError() << std::endl;
        }
    } while (userInput != "exit");

    // 关闭套接字并清理 Windows Sockets API
    closesocket(sock);
    WSACleanup();

    return 0;
}