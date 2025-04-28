#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iomanip>
#include "missileInfo.h"

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
        // std::vector<uint8_t> buffer(sizeof(MissileInfo));
        uint8_t buffer[1024];
        ssize_t recv_len = recvfrom(sock, buffer, sizeof(buffer), 0,
                                    (sockaddr*)&client_addr, &addr_len);
        if (recv_len > 0) {
            MissileInfo missile;
            std::vector<uint8_t> received_data(buffer, buffer + recv_len);
            missile.fromBytes(received_data);
            std::cout << "\nrecv_len size: " << recv_len << "\n";
            std::cout << "\nMissile data size: " << sizeof(missile) << "\n";
            std::cout << "\nMissile Fire:\n";
            std::cout << "  ID: " << missile.missile_id << "\n";
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "  POS: (" << missile.LS_pos_x << ", " << missile.LS_pos_y << ")\n";
            std::cout << "  SPD: " << missile.speed << "\n";
            std::cout << "  ANG: " << missile.degree << "\n";
        }
    }

    close(sock);
    return 0;
}
