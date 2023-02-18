#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

const char* IP_ADDRESS = "192.168.1.199";  // 雷达的IP地址
const int PORT = 5000;                     // 雷达的端口号
const int BUFFER_SIZE = 1024;              // 接收缓冲区大小

const unsigned short CMD_START = 0x7362;   // 开始命令
const unsigned short CMD_SELECT = 0x6531;  // 选择组命令
const unsigned short CMD_SCAN = 0x652a;    // 开始扫描命令
const unsigned short CMD_STOP = 0x6f70;    // 停止命令

SOCKET sock;                                // 客户端Socket

// 异步接收数据的回调函数
void CALLBACK recv_callback(DWORD error, DWORD bytes_transferred, LPWSAOVERLAPPED overlapped, DWORD flags);

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // 初始化Winsock2库

    // 创建Socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 设置服务器地址
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serverAddr.sin_port = htons(PORT);

    // 连接到服务器
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 发送开始命令
    unsigned short cmd = CMD_START;
    send(sock, (const char*)&cmd, sizeof(cmd), 0);

    // 异步接收数据
    char buffer[BUFFER_SIZE];
    WSAOVERLAPPED overlapped;
    memset(&overlapped, 0, sizeof(overlapped));
    overlapped.hEvent = WSACreateEvent();
    DWORD flags = 0;
    if (WSARecv(sock, buffer, BUFFER_SIZE, NULL, &flags, &overlapped, recv_callback) == SOCKET_ERROR) {
        int error = WSAGetLastError();
        if (error != WSA_IO_PENDING) {
            std::cerr << "Failed to receive data: " << error << std::endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }
    }

    // 等待用户输入命令
    std::cout << "Input command: ";
    std::string input;
    std::getline(std::cin, input);
    while (input != "quit") {
        // 将命令转换为16位无符号整数
        unsigned short cmd = std::stoi(input, nullptr
            // 发送命令
            if (cmd == CMD_SELECT || cmd == CMD_SCAN || cmd == CMD_STOP) {
                send(sock, (const char*)&cmd, sizeof(cmd), 0);
            }

        // 等待响应
        memset(&overlapped, 0, sizeof(overlapped));
        overlapped.hEvent = WSACreateEvent();
        flags = 0;
        if (WSARecv(sock, buffer, BUFFER_SIZE, NULL, &flags, &overlapped, recv_callback) == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSA_IO_PENDING) {
                std::cerr << "Failed to receive data: " << error << std::endl;
                closesocket(sock);
                WSACleanup();
                return 1;
            }
        }

        // 等待用户输入下一个命令
        std::cout << "Input command: ";
        std::getline(std::cin, input);
    }

    // 关闭Socket
    closesocket(sock);

    // 释放Winsock2库资源
    WSACleanup();

    return 0;
}

// 异步接收数据的回调函数
void CALLBACK recv_callback(DWORD error, DWORD bytes_transferred, LPWSAOVERLAPPED overlapped, DWORD flags) {
    if (error == 0) {
        char* buffer = (char*)overlapped->hEvent;
        buffer[bytes_transferred] = '\0';
        std::cout << "Received: " << buffer << std::endl;
        // 继续异步接收数据
        memset(overlapped, 0, sizeof(*overlapped));
        overlapped->hEvent = WSACreateEvent();
        DWORD flags = 0;
        if (WSARecv(sock, buffer, BUFFER_SIZE, NULL, &flags, overlapped, recv_callback) == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSA_IO_PENDING) {
                std::cerr << "Failed to receive data: " << error << std::endl;
                closesocket(sock);
                WSACleanup();
                exit(1);
            }
        }
    }
    else {
        std::cerr << "Error in recv_callback: " << error << std::endl;
        closesocket(sock);
        WSACleanup();
        exit(1);
    }
}