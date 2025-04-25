#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>
#include <cmath>

#include "defineSIM.h"
#include "defineMsgPacket.h"
#include "defineAlgorithm.h"
#include "loaderINI.h"

using namespace std;

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // Message msg = 
    // {
    //     {1001, 37.5665f, 126.9780f, 300.0f},
    //     {2001, 1200.0f, 0.0f, 0.0f, 37.5600f, 126.9700f, 100.0f}
    // };

    Message msg;
    loadMessageFromIni("config.ini", msg);

    while (true)
    {
        msg.target.latitude += ((rand() % 100) - 50) * 0.00001f;
        msg.target.longitude += ((rand() % 100) - 50) * 0.00001f;
        msg.target.altitude += ((rand() % 20) - 10) * 0.5f;

        msg.missile.heading = calculateHeading
        (
            msg.missile.latitude, msg.missile.longitude,
            msg.target.latitude, msg.target.longitude
        );

        msg.missile.distanceToTarget = calculateDistance
        (
            msg.missile.latitude, msg.missile.longitude,
            msg.target.latitude, msg.target.longitude
        );

        float moveStep = msg.missile.speed / 1000.0f / 111000.0f;
        float dx = msg.target.latitude - msg.missile.latitude;
        float dy = msg.target.longitude - msg.missile.longitude;
        float dist = sqrt(dx * dx + dy * dy);

        if (dist > 0.00001f) {
            msg.missile.latitude += dx / dist * moveStep;
            msg.missile.longitude += dy / dist * moveStep;
            msg.missile.altitude += (msg.target.altitude - msg.missile.altitude) * 0.05f;
        }

        ssize_t sent = sendto(clientSocket, &msg, sizeof(msg), 0,
                              (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (sent > 0) {
            std::cout << "[전송] 타겟(" << msg.target.latitude << ", " << msg.target.longitude
                      << "), 미사일(" << msg.missile.latitude << ", " << msg.missile.longitude
                      << "), 거리: " << msg.missile.distanceToTarget << " m\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(clientSocket);
    return 0;
}
