#include "missile.h" // MissileInfo 구조체 포함
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iostream>

#define UDP_SERVER_IP "127.0.0.1"
#define UDP_SERVER_PORT 9000

void sendMissile(const MissileInfo& missile) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_SERVER_PORT);
    inet_pton(AF_INET, UDP_SERVER_IP, &server_addr.sin_addr);

    std::vector<uint8_t> packet = missile.toBytes(); // MissileInfo 직렬화 (toBytes)

    ssize_t sent = sendto(sock, packet.data(), packet.size(), 0,
                          (sockaddr*)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        perror("sendto failed");
    } else {
        std::cout << "[Missile Fire Command is sent]\n";
    }

    close(sock);
}
