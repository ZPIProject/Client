#include "NetworkHandler.h"


NetworkHandler::NetworkHandler(unsigned short port)
{
	this->port = port;
}

NetworkHandler::~NetworkHandler()
{
}

bool NetworkHandler::connect(std::string ip_addres)
{
	sf::IpAddress ip = ip_addres;
	return tcp_socket.connect(ip, port) == sf::Socket::Done;
}

void NetworkHandler::send_packet(sf::Packet packet_to_send)
{
	tcp_socket.send(packet_to_send);
}

sf::Packet NetworkHandler::recive_packet()
{
	sf::Packet recived_packet;
	if (tcp_socket.receive(recived_packet) != sf::Socket::Done)
		std::cout << "Faild to recive data\n";

	return recived_packet;
}
