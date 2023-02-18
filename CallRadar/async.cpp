#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

const char* IP_ADDRESS = "192.168.1.199";  // �״��IP��ַ
const int PORT = 5000;                     // �״�Ķ˿ں�
const int BUFFER_SIZE = 1024;              // ���ջ�������С

const unsigned short CMD_START = 0x7362;   // ��ʼ����
const unsigned short CMD_SELECT = 0x6531;  // ѡ��������
const unsigned short CMD_SCAN = 0x652a;    // ��ʼɨ������
const unsigned short CMD_STOP = 0x6f70;    // ֹͣ����

SOCKET sock;                                // �ͻ���Socket

// �첽�������ݵĻص�����
void CALLBACK recv_callback(DWORD error, DWORD bytes_transferred, LPWSAOVERLAPPED overlapped, DWORD flags);

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);  // ��ʼ��Winsock2��

    // ����Socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // ���÷�������ַ
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    serverAddr.sin_port = htons(PORT);

    // ���ӵ�������
    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ���Ϳ�ʼ����
    unsigned short cmd = CMD_START;
    send(sock, (const char*)&cmd, sizeof(cmd), 0);

    // �첽��������
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

    // �ȴ��û���������
    std::cout << "Input command: ";
    std::string input;
    std::getline(std::cin, input);
    while (input != "quit") {
        // ������ת��Ϊ16λ�޷�������
        unsigned short cmd = std::stoi(input, nullptr
            // ��������
            if (cmd == CMD_SELECT || cmd == CMD_SCAN || cmd == CMD_STOP) {
                send(sock, (const char*)&cmd, sizeof(cmd), 0);
            }

        // �ȴ���Ӧ
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

        // �ȴ��û�������һ������
        std::cout << "Input command: ";
        std::getline(std::cin, input);
    }

    // �ر�Socket
    closesocket(sock);

    // �ͷ�Winsock2����Դ
    WSACleanup();

    return 0;
}

// �첽�������ݵĻص�����
void CALLBACK recv_callback(DWORD error, DWORD bytes_transferred, LPWSAOVERLAPPED overlapped, DWORD flags) {
    if (error == 0) {
        char* buffer = (char*)overlapped->hEvent;
        buffer[bytes_transferred] = '\0';
        std::cout << "Received: " << buffer << std::endl;
        // �����첽��������
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