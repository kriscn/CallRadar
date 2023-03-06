#include <iostream>
#include <winsock2.h>

#define BUFFER_SIZE 1460

int main_gpt() {
    // ����socket���󶨶˿�
    SOCKET s;
    sockaddr_in addr;
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9000);
    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    bind(s, (sockaddr*)&addr, sizeof(addr));

    // ��ʼ��������
    char buffer[BUFFER_SIZE];
    int bytes_received;
    while (true) {
        bytes_received = recv(s, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            // �������ݰ�
            if (bytes_received == 6120) { // ÿ�����ݰ�Ϊ5840�ֽ�
                for (int i = 0; i < 1020; i++) { // ÿ�����ݰ���1020��������
                    // �����ͷ�Ͱ�β
                    if (i == 0 || i == 1019) {
                        continue;
                    }
                    // �����������
                    else if (i >= 1 && i <= 1000) {
                        // ÿ��������Ϊ3��16λ���ݣ����α�ʾ���롢�ǶȺ����ǿ��
                        int distance = (buffer[i * 6] << 8) + buffer[i * 6 + 1];
                        int angle = (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3];
                        int intensity = (buffer[i * 6 + 4] << 8) + buffer[i * 6 + 5];
                        // ��������о��롢�Ƕȡ����ǿ�ȵĴ���
                        std::cout << "Point: (" << distance << ", " << angle << ", " << intensity << ")" << std::endl;
                    }
                    // ����״̬��Ϣ��Ԥ������
                    else if (i >= 1001 && i <= 1018) {
                        // �����ﴦ��״̬��Ϣ��Ԥ������
                    }
                    // �����������
                    else if (i >= 1001 && i <= 1010) {
                        if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 0x0000) {
                            // �����״�ֹͣ״̬
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 200 &&     //��ʼ�ǽǶ�
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 160) {      //�����ǽǶ�
                            // �����״�����ɨ��
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8)+ buffer[i * 6 + 3] == 0xaa55) {
                            // �����״����ڷ���
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 0x002a) {
                            // �����״�ѡ�����
                        }
                        else if ((buffer[i * 6] << 8) + buffer[i * 6 + 1] == 0x55aa &&
                            (buffer[i * 6 + 2] << 8) + buffer[i * 6 + 3] == 0x002a) {
                            // �����״�ɨ�����
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