#include "missile.h"
#include "target.h"
#include "type.h"
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

	while (true)
	{
		std::cout << "Enter message type (1 for Missile, 2 for Target, 0 to exit): ";
		int message_type;
		std::cin >> message_type;

		if (message_type == 0)
		{
			std::cout << "Exiting client..." << std::endl;
			break;
		}

		std::vector<uint8_t> serialized_data;

		if (message_type == static_cast<int>(DataType::Missile))
		{
			// MissileInfo 객체 생성 및 직렬화
			MissileInfo missile;
			std::cout << "Enter missile_id: ";
			std::cin >> missile.missile_id;
			std::cout << "Enter LS_pos_x: ";
			std::cin >> missile.LS_pos_x;
			std::cout << "Enter LS_pos_y: ";
			std::cin >> missile.LS_pos_y;
			std::cout << "Enter speed: ";
			std::cin >> missile.speed;
			std::cout << "Enter degree: ";
			std::cin >> missile.degree;

			serialized_data = missile.toBytes();
		}
		else if (message_type == static_cast<int>(DataType::Target))
		{
			// TargetInfo 객체 생성 및 직렬화
			TargetInfo target;
			std::cout << "Enter ID: ";
			std::cin >> target.id;
			std::cout << "Enter pos_x: ";
			std::cin >> target.pos_x;
			std::cout << "Enter pos_y: ";
			std::cin >> target.pos_y;
			std::cout << "Enter speed: ";
			std::cin >> target.speed;
			std::cout << "Enter degree: ";
			std::cin >> target.degree;

			serialized_data = target.toBytes();
		}
		else
		{
			std::cout << "Invalid message type. Please enter 1 for Missile, 2 for Target, or 0 to exit." << std::endl;
			continue;
		}

		// 데이터 전송
		sendto(socket_fd, serialized_data.data(), serialized_data.size(), 0,
			   (const struct sockaddr *)&server_addr, sizeof(server_addr));
		std::cout << "Message sent to server." << std::endl;
	}

	// 소켓 닫기
	close(socket_fd);
	return 0;
}