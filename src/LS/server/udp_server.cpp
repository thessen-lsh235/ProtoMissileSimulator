#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iomanip>
#include "launch_protocol.h"

#define PORT 9000

int main() {
    int sock;
    sockaddr_in server_addr{}, client_addr{};
    socklen_t addr_len = sizeof(client_addr);

    // 소켓 생성
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket error");
        return 1;
    }

    // 바인딩
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
        LaunchCommand cmd{};
        int recv_len = recvfrom(sock, &cmd, sizeof(cmd), 0,
                                (sockaddr*)&client_addr, &addr_len);

        if (recv_len == sizeof(cmd)) {
            std::cout << "\n FIRE COMMAND is successfully received:\n";
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "\tMSG_TYPE       : " << cmd.msg_type << "\n";
            std::cout << "\tLAUNCHER_ID    : " << cmd.launcher_id << "\n";
            std::cout << "\tLAUNCHER_X     : " << cmd.x << "\n";
            std::cout << "\tLAUNCHER_Y     : " << cmd.y << "\n";
            std::cout << "\tLAUNCHER_ANGLE : " << cmd.angle << "\n";
        } else {
            std::cerr << "received data size error ("
                      << recv_len << " bytes)\n";
        }
    }

    close(sock);
    return 0;
}
