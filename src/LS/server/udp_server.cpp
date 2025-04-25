#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "missile.h"

#define PORT 9000

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in server_addr{}, client_addr{};
    socklen_t addr_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind error");
        close(sock);
        return 1;
    }

    std::cout << "[UDP server start] PORT: " << PORT << "\n";    

    while (true) {
        std::vector<uint8_t> buffer(sizeof(MissileInfo));
        ssize_t recv_len = recvfrom(sock, buffer.data(), buffer.size(), 0,
                                    (sockaddr*)&client_addr, &addr_len);
        if (recv_len > 0) {
            MissileInfo missile;
            missile.fromBytes(std::vector<uint8_t>(buffer.begin() + 1, buffer.end()));

            std::cout << "\nMissile Fire:\n";
            std::cout << "  ID: " << missile.missile_id << "\n";
            std::cout << "  POS: (" << missile.LS_pos_x << ", " << missile.LS_pos_y << ")\n";
            std::cout << "  SPD: " << missile.speed << "\n";
            std::cout << "  ANG: " << missile.degree << "\n";
        }
    }

    close(sock);
    return 0;
}
