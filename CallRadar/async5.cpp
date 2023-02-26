#include <iostream>
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <stdio.h>
#include <csignal>
#include <chrono>
#include <atomic>
#include <vector>
#include <cstdint>

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
    std::atomic<std::chrono::time_point<std::chrono::system_clock>> m_last_receive_time;
    
    void receive_data() {        
        //std::cout << "receive_data" << std::endl;
        char buffer[1024];
        while (!m_stop_receive_thread) {
            if (m_socket.receive(buffer, sizeof(buffer))) {
                m_last_receive_time = std::chrono::system_clock::now();
                // 处理接收到的数据
                //std::cout << buffer[1010] << std::endl;
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

        m_last_receive_time = std::chrono::system_clock::now();
        m_receive_thread = std::thread(&Lidar::receive_data, this);
        return true;
    }

    void disconnect() {
        m_stop_receive_thread = true;
        m_socket.close();
        m_receive_thread.join();
    }

    void start_receive() {
        m_last_receive_time = std::chrono::system_clock::now();
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

    bool send_select_groupA_command() {
        const char* select_group_command = "\x65\x31";
        return m_socket.send(select_group_command, 2);
    }

    // start_angle<270;end_agnle>90
    bool download_groupE(uint16_t start_angle, uint16_t end_angle, uint8_t buffer_pos, uint16_t speed_ratio) {
        char buf[9];
        buf[0] = 0x59;
        //start_angle
        buf[1] = (start_angle >> 8) & 0xFF;
        buf[2] = start_angle & 0xFF;
        //end_agnle
        buf[3] = (end_angle >> 8) & 0xFF;
        buf[4] = end_angle & 0xFF;
        //buffer_pos
        buf[5] = buffer_pos & 0xFF;
        //speed_ratio
        buf[6] = (speed_ratio >> 8) & 0xFF;
        buf[7] = speed_ratio & 0xFF;

        buf[8] = 0xaa;

        return m_socket.send((char*)buf, 9);
    }

    bool send_select_groupE_command() {
        const char* select_groupE_command = "\x65\x35";
        return m_socket.send(select_groupE_command, 2);
    }

    bool send_scan_command() {
        const char* scan_command = "\x65\x2a";
        return m_socket.send(scan_command, 2);
    }

    bool is_recent_data_received() {
        auto now = std::chrono::system_clock::now();
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - m_last_receive_time.load());

        return diff.count() < 1;
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
        std::cout << "请输入指令（start、stop、scan、de或exit）: ";
        std::cin >> command;

        if (command == "start") {
            if (lidar.is_recent_data_received()) {
                std::cout << "雷达正在运行，不需要启动" << std::endl;
            }
            else {
                std::cout << "开始执行start指令" << std::endl;
                if (!lidar.send_start_command()) {
                    std::cerr << "Failed to send start command to Lidar." << std::endl;
                    return 1;
                }
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
            if (!lidar.send_select_groupA_command()) {
                std::cerr << "Failed to send select group command." << std::endl;
                return 1;
            }
            Sleep(500);
            if (!lidar.send_scan_command()) {
                std::cerr << "Failed to send scan command." << std::endl;
                return 1;
            }
            /*Sleep(500);
            if (!lidar.send_stop_command()) {
                std::cerr << "Failed to send stop command." << std::endl;
                return 1;
            }*/
        }
        else if (command == "de") {
            std::cout << "0.开始下载E组数据" << std::endl;
            //SendCommand Start
            if (!lidar.is_recent_data_received()) {
                if (!lidar.send_start_command()) {
                    std::cerr << "Failed to send start command to Lidar." << std::endl;
                    return 1;
                }
            }
            Sleep(500);
            //SendCommand DownloadGroupE
            // start_angle<270;end_agnle>90
            if (!lidar.download_groupE(270, 90, 2, 300)) {
                std::cerr << "Failed to send download groupe command." << std::endl;
                return 1;
            }
            // Todo .....
            // Todo .....
            //SendCommand SelectGroupESingle
            Sleep(500);
            std::cout << "1.选择E组指令" << std::endl;
            if (!lidar.send_select_groupE_command()) {
                std::cerr << "Failed to send select group command." << std::endl;
                return 1;
            }
            Sleep(500);
            //SendCommand ScanSingle
            std::cout << "2.发送扫描E组指令" << std::endl;
            if (!lidar.send_scan_command()) {
                std::cerr << "Failed to send scan command." << std::endl;
                return 1;
            }
            /*Sleep(500);
            if (!lidar.send_stop_command()) {
                std::cerr << "Failed to send stop command." << std::endl;
                return 1;
            }*/
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
