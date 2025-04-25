#include "missile.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 9000
#define SERVER_IP "127.0.0.1"

int main()
{
	// UDP 소켓 생성
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (socket_fd < 0)
	{
		perror("Socket creation failed");
		return -1;
	}

	// 서버 주소 설정
	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

	// MissileInfo 객체 생성 및 직렬화
	MissileInfo missile;
	missile.LS_pos_x = 10.5;
	missile.LS_pos_y = 20.5;
	missile.speed = 300;
	missile.degree = 45.0;

	std::vector<uint8_t> serialized_data = missile.toBytes();

	// 데이터 전송
	sendto(socket_fd, serialized_data.data(), serialized_data.size(), 0,
		   (const struct sockaddr *)&server_addr, sizeof(server_addr));
	std::cout << "MissileInfo sent to server." << std::endl;

	// 소켓 닫기
	close(socket_fd);
	return 0;
}