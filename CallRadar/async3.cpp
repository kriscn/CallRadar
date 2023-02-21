// ��δ����������ִ�У������ԡ�
// ������Ҫ��ͷ�ļ��Ϳ�
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main_async3()
{
    // ��ʼ��Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // ����һ��TCP�׽���
    SOCKET radarSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ����������Ϣ
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.1.199");
    addr.sin_port = htons(5000);

    // ���ӵ��״�
    connect(radarSocket, (SOCKADDR*)&addr, sizeof(addr));

    // ���Ϳ�ʼ����
    char startCmd[] = { 0x73, 0x62 };
    send(radarSocket, startCmd, sizeof(startCmd), 0);

    // �����״﷢�͵�����
    char recvBuf[1024];
    int bytesRecv;
    while (true)
    {
        bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // �����״﷢�͵�����
            // ע�⣺������Ҫʹ���첽������������ʽ���������ݣ��Ա���ͻ��˱�����
            // �����￪ʼ =>
            // ����һ���¼�����
            WSAEVENT event = WSACreateEvent();

            // ���¼��������׽��ֹ���
            WSAEventSelect(radarSocket, event, FD_READ);

            // �ȴ��¼�֪ͨ
            while (true)
            {
                // �ȴ��¼�֪ͨ
                DWORD result = WaitForSingleObject((HANDLE)event, 1000);
                if (result == WAIT_OBJECT_0)
                {
                    // �¼��Ѿ����������Խ�������
                    char recvBuf[1024];
                    int bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
                    if (bytesRecv > 0)
                    {
                        // ��������
                        // ...
                        printf("deal data(line:65):%d\n", bytesRecv);
                    }
                    else if (bytesRecv == 0)
                    {
                        // �����Ѿ��Ͽ�
                        printf("Connection closed(line:70)\n");
                        break;
                    }
                    else
                    {
                        // ��������
                        // 2023.02.19(
                        //printf("recv start1(line:76): %d\n", WSAGetLastError());
                        //break;
                    }
                }
            }
            // <= ���������
        }
        else if (bytesRecv == 0)
        {
            // �����Ѿ��Ͽ�
            printf("Connection closed(line:86)\n");
            break;
        }
        else
        {
            // ��������
            printf("recv start2(line:92): %d\n", WSAGetLastError());
            break;
        }
    }
    printf("Press any key to send group command...\n");
    getchar();
    // ����ѡ��������
    try {
        char selectCmd[] = { 0x65, 0x31 };
        iResult = send(radarSocket, selectCmd, sizeof(selectCmd), 0);
        std::cerr << "sending select command result(line:101): " << iResult << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending select command(line:104): " << e.what() << std::endl;
    }

    // ���Ϳ�ʼɨ������
    try {
        char scanCmd[] = { 0x65, 0x2a };
        iResult = send(radarSocket, scanCmd, sizeof(scanCmd), 0);
        std::cerr << "sending scan command result(line:111): " << iResult << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error sending scan command(line:114): " << e.what() << std::endl;
    }    

    // �ȴ��״﷢������
    std::cerr << "�ȴ��״﷢������: " << std::endl;
    while (true)
    {
        bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
        if (bytesRecv > 0)
        {
            // �����״﷢�͵�����
            // ע�⣺������Ҫʹ���첽������������ʽ���������ݣ��Ա���ͻ��˱�����
            // �����￪ʼ =>
            // ����һ���¼�����
            WSAEVENT event = WSACreateEvent();

            // ���¼��������׽��ֹ���
            WSAEventSelect(radarSocket, event, FD_READ);

            // �ȴ��¼�֪ͨ
            while (true)
            {
                // �ȴ��¼�֪ͨ
                DWORD result = WaitForSingleObject((HANDLE)event, 1000);
                if (result == WAIT_OBJECT_0)
                {
                    //std::cerr << "3: " << std::endl;
                    // �¼��Ѿ����������Խ�������
                    char recvBuf[1024];
                    int bytesRecv = recv(radarSocket, recvBuf, sizeof(recvBuf), 0);
                    if (bytesRecv > 0)
                    {
                        // ��������
                        // ...
                    }
                    else if (bytesRecv == 0)
                    {
                        // �����Ѿ��Ͽ�
                        printf("Connection closed\n");
                        std::cerr << "1: " << std::endl;
                        break;
                    }
                    else
                    {
                        // ��������
                        //printf("recv failed2: %d\n", WSAGetLastError());
                        //std::cerr << "2: " << std::endl;
                        break;
                    }
                }
            }
            // <= ���������
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
            //printf("recv failed(176): %d\n", WSAGetLastError());
            break;
        }
    }
    std::cerr << "99: " << std::endl;
    // ����ֹͣ����
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

    // �ر��׽���
    closesocket(radarSocket);
    WSACleanup();

    // ����Winsock
    return 0;
}
