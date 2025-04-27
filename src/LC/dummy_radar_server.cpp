// dummy_radar_server.cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    // 1. 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "[RadarServer] 소켓 생성 실패" << std::endl;
        return -1;
    }

    // 2. 주소 설정
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(5001);  // 5001 포트

    // 3. 바인드
    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[RadarServer] 바인드 실패" << std::endl;
        close(server_fd);
        return -1;
    }

    // 4. 리슨
    if (listen(server_fd, 1) < 0) {
        std::cerr << "[RadarServer] 리슨 실패" << std::endl;
        close(server_fd);
        return -1;
    }

    std::cout << "[RadarServer] 5001 포트에서 접속 대기중..." << std::endl;

    // 5. accept()
    client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
    if (client_fd < 0) {
        std::cerr << "[RadarServer] 클라이언트 수락 실패" << std::endl;
        close(server_fd);
        return -1;
    }

    std::cout << "[RadarServer] 클라이언트 연결 성공!" << std::endl;

    // 연결 유지
    while (true) {
        sleep(1);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
