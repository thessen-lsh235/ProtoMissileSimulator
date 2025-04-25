#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include "ini.h"
#include "launch_protocol.h"

// 설정 구조체
struct LauncherConfig {
    std::string id;
    double x;
    double y;
    double angle = 0.0;  // 기본 발사각: 진북
};

// INI 파서 콜백
static int handler(void* user, const char* section, const char* name, const char* value) {
    LauncherConfig* config = (LauncherConfig*)user;

    if (std::string(section) == "LAUNCHER") {
        if (std::string(name) == "ID") config->id = value;
        else if (std::string(name) == "X") config->x = std::stod(value);
        else if (std::string(name) == "Y") config->y = std::stod(value);
        else if (std::string(name) == "ANGLE") config->angle = std::stod(value); // 선택적
    }
    return 1;
}

// INI 읽기
LauncherConfig read_launcher_config(const std::string& filename) {
    LauncherConfig config;
    if (ini_parse(filename.c_str(), handler, &config) < 0) {
        std::cerr << "INI file read error: " << filename << std::endl;
        exit(1);
    }
    return config;
}

int main() {
    LauncherConfig config = read_launcher_config("../launcher_config.ini");
    
    // 구조체 구성
    LaunchCommand cmd{};
    strncpy(cmd.msg_type, "LAUNCH", sizeof(cmd.msg_type));
    strncpy(cmd.launcher_id, config.id.c_str(), sizeof(cmd.launcher_id));
    cmd.x = config.x;
    cmd.y = config.y;
    cmd.angle = config.angle;

    // UDP 소켓 생성
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket creation failed");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 구조체 송신
    int sent = sendto(sock, &cmd, sizeof(cmd), 0,
                      (sockaddr*)&server_addr, sizeof(server_addr));
    if (sent < 0) {
        perror("sendto failed");
    } else {
        std::cout << "[FIRE MESSAGE IS SENT]\n";
    }

    close(sock);
    return 0;
}
