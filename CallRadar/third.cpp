#include <iostream>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")//加载 ws2_32.dll

//添加此句，屏蔽"warning作为error", 提高兼容性
#pragma warning(disable:4996)

const char START_COMMAND[] = { 0x73, 0x62 };
const char STOP_COMMAND[] = { 0x6f, 0x70 };
const char SCAN_COMMAND[] = { 0x65, 0x2a };
const char* IP_ADDRESS = "192.168.1.199";

int main3()
{
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed with error: " << iResult << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the remote host
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.S_un.S_addr = inet_addr(IP_ADDRESS);
    serverAddress.sin_port = htons(5000);

    iResult = connect(ConnectSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (iResult == SOCKET_ERROR) {
        std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Send the start command
    iResult = send(ConnectSocket, START_COMMAND, sizeof(START_COMMAND), 0);
    if (iResult == SOCKET_ERROR) {
        if (WSAGetLastError() == 10054) {
            std::cerr << "The connection was reset by the remote host." << std::endl;
            std::cerr << "Trying to reconnect..." << std::endl;

            // Try to reconnect to the remote host
            iResult = connect(ConnectSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
            if (iResult == SOCKET_ERROR) {
                std::cerr << "connect failed with error: " << WSAGetLastError() << std::endl;
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            } else {
                std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }
        }
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Wait for a moment
    Sleep(1000);

    // Send the scan command
    iResult = send(ConnectSocket, SCAN_COMMAND, sizeof(SCAN_COMMAND), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket
        );
        WSACleanup();
        return 1;
    }
    // Wait for a moment
    Sleep(1000);

    // Send the stop command
    iResult = send(ConnectSocket, STOP_COMMAND, sizeof(STOP_COMMAND), 0);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "send failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Wait for a moment
    Sleep(1000);

    // Close the socket
    iResult = closesocket(ConnectSocket);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "closesocket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Cleanup Winsock
    WSACleanup();

    return 0;
}