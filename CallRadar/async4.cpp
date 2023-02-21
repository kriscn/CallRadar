#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main_async4()
{
    // ��ʼ��Winsock��
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    // �����׽���
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // ���÷�������ʽ
    u_long mode = 1;
    result = ioctlsocket(sock, FIONBIO, &mode);
    if (result == SOCKET_ERROR) {
        printf("ioctlsocket failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ���ӷ�����
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("192.168.1.199");
    serverAddr.sin_port = htons(5000);

    result = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("connect failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ���Ϳ�ʼ����
    char startCmd[] = { 0x73, 0x62, 0x00, 0x00 };
    result = send(sock, startCmd, sizeof(startCmd), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ���տ�ʼ������Ӧ
    char recvData[1024];
    result = recv(sock, recvData, sizeof(recvData), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ����ѡ��������
    char selectGroupCmd[] = { 0x65, 0x31, 0x00, 0x00 };
    result = send(sock, selectGroupCmd, sizeof(selectGroupCmd), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    //
// ����ѡ����������Ӧ
    result = recv(sock, recvData, sizeof(recvData), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ���Ϳ�ʼɨ������
    char startScanCmd[] = { 0x65, 0x2a, 0x00, 0x00 };
    result = send(sock, startScanCmd, sizeof(startScanCmd), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ���տ�ʼɨ��������Ӧ
    result = recv(sock, recvData, sizeof(recvData), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // �ȴ�ɨ�����
    Sleep(10000);

    // ����ֹͣ����
    char stopCmd[] = { 0x6f, 0x70, 0x00, 0x00 };
    result = send(sock, stopCmd, sizeof(stopCmd), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // ����ֹͣ������Ӧ
    result = recv(sock, recvData, sizeof(recvData), 0);
    if (result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
        printf("recv failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // �ر��׽���
    closesocket(sock);

    // ����Winsock��
    WSACleanup();

    return 0;
}