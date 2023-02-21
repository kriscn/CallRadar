#include <winsock2.h>
#include <stdio.h>

SOCKET ConnectToRadar(const char* ipAddr, int port)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        return INVALID_SOCKET;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return INVALID_SOCKET;
    }

    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ipAddr);

    if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("connect failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return sock;
}
int ReceiveData(SOCKET sock, char* buf, int bufSize)
{
    int bytesRecv = 0;

    while (true) {
        int result = recv(sock, buf + bytesRecv, bufSize - bytesRecv, 0);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if (error != WSAEWOULDBLOCK) {
                printf("recv failed: %d\n", error);
                return -1;
            }
            // 暂无数据可读，等待一段时间后再继续读取
            Sleep(100);
        }
        else if (result == 0) {
            printf("Connection closed by remote host\n");
            return -1;
        }
        else {
            bytesRecv += result;
            if (bytesRecv == bufSize) {
                printf("Receive buffer overflow\n");
                return -1;
            }
            // 数据接收完成
            break;
        }
    }

    return bytesRecv;
}
int SendCommand(SOCKET sock, const char* cmd, int cmdLen, char* response, int respSize)
{
    // 发送命令
    if (send(sock, cmd, cmdLen, 0) == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        return -1;
    }

    // 等待响应
    int bytesRecv = ReceiveData(sock, response, respSize);
    if (bytesRecv == -1) {
        printf("Failed to receive response\n");
        return -1;
    }

    // 处理响应
    if (bytesRecv < 4) {
        printf("Invalid response length: %d\n", bytesRecv);
        return -1;
    }

    int status = (response[2] << 8) | response[3];
    if (status != 0) {
        printf("Command failed with status code: %d\n", status);
        return -1;
    }

    return 0;
}
int main_async()
{
    SOCKET sock = ConnectToRadar("192.168.1.199", 5000);
    if (sock == INVALID_SOCKET) {
        printf("Failed to connect to radar\n");
        return 1;
    }

    char response[1024];
    if (SendCommand(sock, "\x73\x62", 2, response, sizeof(response)) == -1) {
        printf("Failed to send start command\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (SendCommand(sock, "\x65\x31", 2, response, sizeof(response)) == -1) {
        printf("Failed to send select group command\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (SendCommand(sock, "\x65\x2a", 2, response, sizeof(response)) == -1) {
        printf("Failed to send start scan command\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // 扫描中，等待用户输入停止命令
    if (SendCommand(sock, "\x6f\x70", 2, response, sizeof(response)) == -1) {
        printf("Failed to send stop command\n");
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}