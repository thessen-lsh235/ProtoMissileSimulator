#include "missileInfo.h"
#include "targetInfo.h"
#include "type.h"
#include <cstring>
#include <iostream>
#include <list>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define SERVER_PORT 9000
#define BUFFER_SIZE 1024
#define MAX_TARGETS 50

int main() {
    // UDP 소켓 생성
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 서버 주소 설정
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // 소켓 바인딩
    if (bind(socket_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(socket_fd);
        return -1;
    }

    std::cout << "Server is running on port " << SERVER_PORT << "..." << std::endl;

    // Target 관리
    std::list<TargetInfo> targets;
    int next_target_id = 1;

    // 데이터 수신
    uint8_t buffer[BUFFER_SIZE];
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        ssize_t bytes_received = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received > 0) {
            std::vector<uint8_t> received_data(buffer, buffer + bytes_received);

            uint8_t type = received_data[0];
            if (static_cast<DataType>(type) == DataType::Missile) {
                MissileInfo missile;
                missile.deserializeImpl(received_data);
                std::cout << "Received MissileInfo - missile_id: " << missile.missile_id << ", LS_pos_x: " << missile.LS_pos_x
                          << ", LS_pos_y: " << missile.LS_pos_y << ", Speed: " << missile.speed << ", Degree: " << missile.degree
                          << std::endl;
            } else if (static_cast<DataType>(type) == DataType::Target) {
                if (targets.size() >= MAX_TARGETS) {
                    std::cerr << "Maximum number of targets reached. Cannot add more targets." << std::endl;
                    continue;
                }

                TargetInfo target;
                target.deserializeImpl(received_data);

                // 서버에서 ID 관리
                targets.push_back(target);

                std::cout << "Added Target - Name: " << target.name << ", Pos_x: " << target.pos_x << ", Pos_y: " << target.pos_y
                          << ", Speed: " << target.speed << ", Degree: " << target.degree << std::endl;
            } else {
                std::cerr << "Unknown data type received: " << static_cast<int>(type) << std::endl;
            }
        }
    }

    // 소켓 닫기
    close(socket_fd);
    return 0;
}