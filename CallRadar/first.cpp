#include <iostream>
#include <WinSock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")//���� ws2_32.dll

//��Ӵ˾䣬����"warning��Ϊerror", ��߼�����
#pragma warning(disable:4996)

using namespace std;

const int BUFF_SIZE = 1024;
const int CMD_START = 0x7362;
const int CMD_STOP = 0x6f70;
const int CMD_SCAN = 0x652a;
const char* IP_ADDRESS1 = "192.168.1.199";
const int PORT = 5000;

int main1() {
    // ��ʼ�� Windows Sockets API
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // �����׽���
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cout << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // �����״��ַ�Ͷ˿�
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS1);
    serverAddr.sin_port = htons(PORT);    

    // ���ӵ�������
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
        // ��ȡ�û�����
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

        // ������ת��Ϊ�ַ���
        sprintf_s(sendbuff, "%d", cmd);
        // ��������
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
        
        // ��������
        iResult = recv(sock, recvbuff, BUFF_SIZE, 0);
        if (iResult > 0) {
            // �����յ�������ת��Ϊ�ַ���
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

    // �ر��׽��ֲ����� Windows Sockets API
    closesocket(sock);
    WSACleanup();

    return 0;
}