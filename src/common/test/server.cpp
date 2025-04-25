#include "missile.h"
#include "target.h"
#include "type.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define SERVER_PORT 9000
#define BUFFER_SIZE 1024

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
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);

	// 소켓 바인딩
	if (bind(socket_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(socket_fd);
		return -1;
	}

	std::cout << "Server is running on port " << SERVER_PORT << "..." << std::endl;

	// 데이터 수신
	uint8_t buffer[BUFFER_SIZE];
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	while (true)
	{
		ssize_t bytes_received = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0,
										  (struct sockaddr *)&client_addr, &client_len);
		if (bytes_received > 0)
		{
			std::vector<uint8_t> received_data(buffer, buffer + bytes_received);

			uint8_t type = received_data[0];
			if (static_cast<DataType>(type) == DataType::Missile)
			{
				MissileInfo missile;
				missile.fromBytes(std::vector<uint8_t>(received_data.begin() + 1, received_data.end()));
				std::cout << "Received MissileInfo - LS_pos_x: " << missile.LS_pos_x
						  << ", LS_pos_y: " << missile.LS_pos_y
						  << ", Speed: " << missile.speed
						  << ", Degree: " << missile.degree << std::endl;
			}
			else if (static_cast<DataType>(type) == DataType::Target)
			{
				TargetInfo target;
				target.fromBytes(std::vector<uint8_t>(received_data.begin() + 1, received_data.end()));
				std::cout << "Received TargetInfo - ID: " << target.id
						  << ", Pos_x: " << target.pos_x
						  << ", Pos_y: " << target.pos_y
						  << ", Speed: " << target.speed
						  << ", Degree: " << target.degree << std::endl;
			}
			else
			{
				std::cerr << "Unknown data type received: " << static_cast<int>(type) << std::endl;
			}
		}
	}

	// 소켓 닫기
	close(socket_fd);
	return 0;
}