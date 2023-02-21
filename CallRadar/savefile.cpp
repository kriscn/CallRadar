#include <iostream>
#include <fstream>
#include <string>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define MAX_PACKET_SIZE 2048 // ������ݰ���С
#define HEADER_BYTE 0x59 // �������ݵ�ָ��ͷ
#define TAIL_BYTE 0xaa // �������ݵ�ָ��β

// 3���״�״̬
enum RadarStatus {
    STOPPED,
    SCANNING,
    RETURNING
};

// ����һ��������
void parseDataGroup(unsigned char* data, int& distance, float& angle, int& intensity) {
    unsigned char byte1 = data[0];
    unsigned char byte2 = data[1];
    unsigned char byte3 = data[2];
    unsigned char byte4 = data[3];

    // �������룬��λΪmm
    distance = byte1 | (byte2 << 8);

    // �����Ƕȣ���λΪ0.0625��
    short sAngle = byte3 | (byte4 << 8);
    angle = (float)sAngle * 0.0625f;

    // �������ǿ��
    intensity = data[4] | (data[5] << 8);
}

// �����������
void parseSpecialParams(unsigned char* data) {
    // TODO: �����¶ȡ��豸Ψһʶ��š��̼��汾�š��������ʡ���������ʼ�Ƕȡ������ǽ����ǶȵȲ���
}

// ������ת��Ϊ���Ƹ�ʽ
void convertToPointCloud(unsigned char* data, int dataSize, string fileName) {
    ofstream fout(fileName);
    fout << "# .PCD v.7 - Point Cloud Data file format\n";
    fout << "VERSION .7\n";
    fout << "FIELDS x y z\n";
    fout << "SIZE 4 4 4\n";
    fout << "TYPE F F F\n";
    fout << "COUNT 1 1 1\n";
    fout << "WIDTH " << dataSize << endl;
    fout << "HEIGHT 1\n";
    fout << "VIEWPOINT 0 0 0 1 0 0 0\n";
    fout << "POINTS " << dataSize << endl;
    fout << "DATA ascii\n";

    for (int i = 0; i < dataSize; i += 3) {
        int distance1, distance2, distance3;
        float angle1, angle2, angle3;
        int intensity1, intensity2, intensity3;

        parseDataGroup(data + i, distance1, angle1, intensity1);
        parseDataGroup(data + i + 1, distance2, angle2, intensity2);
        parseDataGroup(data + i + 2, distance3, angle3, intensity3);

        // ֻ�������벻Ϊ0�ĵ�
        if (distance1 > 0) {
            fout << distance1 / 1000.0f << " " << distance1 / 1000.0f * sin(angle1) << " " << distance1 / 1000.0f * cos(angle1) << endl;
        }
        if (distance2 > 0) {
            fout << distance2 / 1000.0f << " " << distance2 / 1000.0f * sin(angle2) << " " << distance2 / 1000.0f * cos(angle2) << endl;
        }
        if (distance3 > 0) {
            fout << distance3 / 1000.0f << " " << distance3 / 1000.0f * sin(angle3) << " " << distance3 / 1000.0f * cos(angle3) << endl;
        }
    }
    fout.close();
}
// �����״�����
void receiveRadarData(SOCKET& socket, sockaddr_in& serverAddr, RadarStatus& status) {
    unsigned char packet[MAX_PACKET_SIZE];
    int dataSize = 0;
    while (true) {
        int addrLen = sizeof(serverAddr);
        int n = recvfrom(socket, (char*)packet, MAX_PACKET_SIZE, 0, (SOCKADDR*)&serverAddr, &addrLen);

        if (n == SOCKET_ERROR) {
            cerr << "��������ʧ�ܣ�������룺" << WSAGetLastError() << endl;
            break;
        }

        // �ж������Ƿ�Ϊ��������
        if (packet[0] == HEADER_BYTE && packet[n - 1] == TAIL_BYTE) {
            dataSize = (packet[1] << 8) | packet[2];
            break;
        }
    }

    // ������������
    int receivedSize = 0;
    unsigned char* data = new unsigned char[dataSize];
    while (receivedSize < dataSize) {
        int addrLen = sizeof(serverAddr);
        int n = recvfrom(socket, (char*)data + receivedSize, dataSize - receivedSize, 0, (SOCKADDR*)&serverAddr, &addrLen);

        if (n == SOCKET_ERROR) {
            cerr << "��������ʧ�ܣ�������룺" << WSAGetLastError() << endl;
            break;
        }

        receivedSize += n;
    }

    // ��������
    if (dataSize > 0) {
        if (status == STOPPED) {
            status = SCANNING;
        }
        else if (status == RETURNING) {
            convertToPointCloud(data, dataSize, "result.pcd");
            status = STOPPED;
        }
    }

    delete[] data;
}
// ������
int main_save() {
    WSADATA wsaData;
    int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (ret != 0) {
        cerr << "��ʼ��WinSock2��ʧ�ܣ�������룺" << ret << endl;
        return 1;
    }
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "�����׽���ʧ�ܣ�������룺" << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.100");

    RadarStatus status = STOPPED;

    while (true) {
        receiveRadarData(sock, serverAddr, status);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}