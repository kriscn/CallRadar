#include <iostream>
#include <winsock2.h>

#define BUFFER_SIZE 1460

int main_gpt() {
    // 创建socket并绑定端口
    SOCKET s;
    sockaddr_in addr;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    bind(s, (sockaddr*)&addr, sizeof(addr));

    // 开始接收数据
    char buffer[BUFFER_SIZE];
    int bytes_received;
    while (true) {
        bytes_received = recv(s, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            // 解析数据包
            if (bytes_received == 6120) { // 每个数据包为5840字节
                for (int i = 0; i < 1020; i++) { // 每个数据包有1020个数据组
                    // 处理包头和包尾
                    if (i == 0 || i == 1019) {
                        continue;
                    }
                    // 处理点云数据
                    else if (i >= 1 && i <= 1000) {
                        // 每个数据组为3个16位数据，依次表示距离、角度和相对强度
                        int distance = (buffer[i * 6] << 8) + buffer[i * 6 + 1];
                        int angle = (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3];
                        int intensity = (buffer[i * 6 + 4] << 8) + buffer[i * 6 + 5];
                        // 在这里进行距离、角度、相对强度的处理
                        std::cout << "Point: (" << distance << ", " << angle << ", " << intensity << ")" << std::endl;
                    }
                    // 处理状态信息或预留数据
                    else if (i >= 1001 && i <= 1018) {
                        // 在这里处理状态信息或预留数据
                    }
                    // 处理特殊参数
                    else if (i >= 1001 && i <= 1010) {
                        if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 0x0000) {
                            // 处理雷达停止状态
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 200 &&     //起始角角度
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 160) {      //结束角角度
                            // 处理雷达正在扫描
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8)+ buffer[i * 6 + 3] == 0xaa55) {
                            // 处理雷达正在返程
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 0x002a) {
                            // 处理雷达选组就绪
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 0x002a) {
                            // 处理雷达扫描完成
                        }
                    }
                }
            }
        }
    }

    closesocket(s);
    WSACleanup();

    return 0;
}