#include <iostream>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#include "defineMFR.h"
#include "defineMsgPacket.h"

using namespace std;

int main(void)
{
    int serverSocket;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverSocket < 0)
    {
        perror("[serverSocket < 0]");
        exit(EXIT_FAILURE);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("[if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)]");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    cout<<"MFR Server Running ... "<<"\n";

    while(true)
    {
        memset(buffer, 0, BUF_SIZE);
        int recvLen = recvfrom(serverSocket, buffer, BUF_SIZE, 0, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if (recvLen < 0)
        {
            perror("[if (recvLen < 0)]");
            continue;
        }
        if (recvLen != sizeof(Message)) 
        {
            std::cerr << "[if (recvLen != sizeof(Message))]" << recvLen << std::endl;
            continue;
        }
    
        Message msg;
        memcpy(&msg, buffer, sizeof(Message));
    
        cout << "[수신] 클라이언트 "
             << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";
    
        cout << "타겟 ID: " << msg.target.targetId
             << " | 위치: (" << msg.target.latitude << ", "
             << msg.target.longitude << "), 고도: " << msg.target.altitude << "\n";
    
        cout << "미사일 ID: " << msg.missile.missileId
             << " | 위치: (" << msg.missile.latitude << ", "
             << msg.missile.longitude << "), 고도: " << msg.missile.altitude << "\n"
             << "    속도: " << msg.missile.speed << " m/s, 방향: "
             << msg.missile.heading << "°, 거리: " << msg.missile.distanceToTarget << " m\n";
        cout << "================"<<"\n";
    }

    close(serverSocket);

    return 0;
}