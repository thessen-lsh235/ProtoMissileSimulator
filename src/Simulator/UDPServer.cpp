#include "UDPServer.h"
#include <cstring>		// For memset
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h>		// For close()
#include <iostream>

UDPServer::UDPServer(unsigned short port)
	: port(port), server_socket(-1)
{
	// Create a UDP socket
	server_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_socket < 0)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	// Bind the socket to the specified port
	sockaddr_in server_addr{};
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);

	if (bind(server_socket, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Bind failed");
		close(server_socket);
		exit(EXIT_FAILURE);
	}
}

UDPServer::~UDPServer()
{
	if (server_socket >= 0)
	{
		close(server_socket);
	}
}

void UDPServer::start()
{
	std::cout << "UDP Server started on port " << port << std::endl;

	while (true)
	{
		char buffer[1024];
		sockaddr_in client_addr{};
		socklen_t client_len = sizeof(client_addr);

		// Receive data from the client
		ssize_t bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0,
										  (struct sockaddr *)&client_addr, &client_len);
		if (bytes_received > 0)
		{
			buffer[bytes_received] = '\0'; // Null-terminate the received data
			std::string msg(buffer);
			if (messageHandler) {
				messageHandler(msg);
			} else {
				std::cout << "Received: " << buffer << std::endl;
			}
		}
	}
}

void UDPServer::setMessageHandler(MessageHandler handler) {
    messageHandler = std::move(handler);
}