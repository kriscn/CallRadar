#include <iostream>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <stdio.h>
#include <csignal>

#pragma comment(lib, "Ws2_32.lib")

class Socket {
private:
    SOCKET m_socket;
public:
    Socket() {
        m_socket = INVALID_SOCKET;
    }

    bool create() {
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        return m_socket != INVALID_SOCKET;
    }

    bool connect(const char* ip_address, int port) {
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(ip_address);
        server_address.sin_port = htons(port);

        return ::connect(m_socket, (SOCKADDR*)&server_address, sizeof(server_address)) == 0;
    }

    bool send(const char* buffer, int length) {
        try {
            return ::send(m_socket, buffer, length, 0) != SOCKET_ERROR;
        }
        catch (const std::exception& e) {
            std::cerr << "Error sending data: " << e.what() << std::endl;
        }
    }

    bool receive(char* buffer, int length) {
        int bytes_received = ::recv(m_socket, buffer, length, 0);
        if (bytes_received == SOCKET_ERROR) {
            return false;
        }
        else if (bytes_received == 0) {
            return false;
        }

        buffer[bytes_received] = '\0';
        return true;
    }

    bool close() {
        return ::closesocket(m_socket) != SOCKET_ERROR;
    }

    ~Socket() {
        if (m_socket != INVALID_SOCKET) {
            ::closesocket(m_socket);
        }
    }
};

class Lidar {
private:
    Socket m_socket;
    std::thread m_receive_thread;
    bool m_stop_receive_thread;

    void receive_data() {
        char buffer[1024];
        while (!m_stop_receive_thread) {
            if (m_socket.receive(buffer, sizeof(buffer))) {
                // 处理接收到的数据
                //std::cout << buffer << std::endl;
            }
        }
    }

public:
    Lidar() : m_stop_receive_thread(false) {}

    bool connect(const char* ip_address, int port) {
        if (!m_socket.create()) {
            std::cout << "Failed to create socket" << std::endl;
            return false;
        }

        if (!m_socket.connect(ip_address, port)) {
            std::cout << "Failed to connect to server" << std::endl;
            return false;
        }

        m_receive_thread = std::thread(&Lidar::receive_data, this);
        return true;
    }

    void disconnect() {
        m_stop_receive_thread = true;
        m_socket.close();
        m_receive_thread.join();
    }

    void start_receive() {
        m_receive_thread = std::thread(&Lidar::receive_data, this);
    }

    void stop_receive() {
        m_stop_receive_thread = true;
        if (m_receive_thread.joinable()) {
            m_receive_thread.join();
        }
    }

    bool send_start_command() {
        const char* start_command = "\x73\x62";
        return m_socket.send(start_command, 2);
    }

    bool send_stop_command() {
        const char* stop_command = "\x6f\x70";
        return m_socket.send(stop_command, 2);
    }

    bool send_select_group_command() {
        const char* select_group_command = "\x65\x31";
        return m_socket.send(select_group_command, 2);
    }

    bool send_scan_command() {
        const char* scan_command = "\x65\x2a";
        return m_socket.send(scan_command, 2);
    }

    ~Lidar() {
        stop_receive();
    }
};

int main() {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    Lidar lidar;

    // 连接雷达
    if (!lidar.connect("192.168.1.199", 5000)) {
        std::cerr << "Failed to connect to Lidar" << std::endl;
        return 1;
    }
    
    std::string command;
    while (true) {
        std::cout << "请输入指令（start、stop、scan或exit）: ";
        std::cin >> command;

        if (command == "start") {
            std::cout << "开始执行start指令" << std::endl;
            if (!lidar.send_start_command()) {
                std::cerr << "Failed to send start command to Lidar." << std::endl;
                return 1;
            }
        }
        else if (command == "stop") {
            std::cout << "开始执行stop指令" << std::endl;
            if (!lidar.send_stop_command()) {
                std::cerr << "Failed to send stop command." << std::endl;
                return 1;
            }
        }
        else if (command == "scan") {
            std::cout << "开始执行scan指令" << std::endl;
            if (!lidar.send_select_group_command()) {
                std::cerr << "Failed to send select group command." << std::endl;
                return 1;
            }
            Sleep(100);
            if (!lidar.send_scan_command()) {
                std::cerr << "Failed to send scan command." << std::endl;
                return 1;
            }
        }
        else if (command == "exit") {
            std::cout << "退出程序" << std::endl;
            lidar.disconnect();
            WSACleanup();
            break;
        }
    }

    return 0;
}
