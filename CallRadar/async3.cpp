// ������Ҫ��ͷ�ļ��Ϳ�
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // ��ʼ��Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // ����һ��TCP�׽���
    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ����������Ϣ
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.199");
    addr.sin_port = htons(5000);

    // ���ӵ��״�
    connect(s, (SOCKADDR*)&addr, sizeof(addr));

    // ���Ϳ�ʼ����
    char startCmd[] = { 0x73, 0x62 };
    send(s, startCmd, sizeof(startCmd), 0);

    // �����״﷢�͵�����
    char recvBuf[1024];
    int bytesRecv;
    while (true)
    {
        bytesRecv = recv(s, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // �����״﷢�͵�����
            // ע�⣺������Ҫʹ���첽������������ʽ���������ݣ��Ա���ͻ��˱�����
        }
        else if (bytesRecv == 0)
        {
            // �����Ѿ��Ͽ�
            printf("Connection closed\n");
            break;
        }
        else
        {
            // ��������
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    // ����ѡ��������
    char selectCmd[] = { 0x65, 0x31 };
    send(s, selectCmd, sizeof(selectCmd), 0);

    // ���Ϳ�ʼɨ������
    char scanCmd[] = { 0x65, 0x2a };
    send(s, scanCmd, sizeof(scanCmd), 0);

    // �ȴ��״﷢������
    while (true)
    {
        bytesRecv = recv(s, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // �����״﷢�͵�����
            // ע�⣺������Ҫʹ���첽������������ʽ���������ݣ��Ա���ͻ��˱�����
        }
        else if (bytesRecv == 0)
        {
            // �����Ѿ��Ͽ�
            printf("Connection closed\n");
            break;
        }
        else
        {
            // ��������
            printf("recv failed: %d\n", WSAGetLastError());
            break;
        }
    }

    // ����ֹͣ����
    char stopCmd[] = { 0x6f, 0x70 };
    send(s, stopCmd, sizeof(stopCmd), 0);

    // �ر��׽���
    closesocket(s);
    WSACleanup();

    // ����Winsock
    return 0;
}
