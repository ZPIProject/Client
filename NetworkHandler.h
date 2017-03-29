#pragma once
#include <SFML\Network.hpp>
#include <iostream>
class NetworkHandler
{

public:
	NetworkHandler(unsigned short port);
	~NetworkHandler();

	bool connect(std::string ip_addres);
	void send_packet(sf::Packet packet_to_send);
	sf::Packet recive_packet();

	private:
		sf::TcpSocket tcp_socket;
		unsigned short port;
};

