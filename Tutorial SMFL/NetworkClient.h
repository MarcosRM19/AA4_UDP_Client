#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <string>

class NetworkClient
{
private:

	std::string ip;
	int port;
	std::string guid;
	std::shared_ptr<sf::TcpSocket> socket;

public:

	NetworkClient(const std::string& ip, int port, const std::string& guid);

	void HandleIncomingPackets();

	std::string& GetIp() { return ip; }
	int GetPort() const { return port; }
	std::string GetGuid() const { return guid; }
	sf::TcpSocket& GetSocket() { return *socket; }
	void SetSocket(const std::shared_ptr<sf::TcpSocket> newSocket) { socket = std::move(newSocket); }
	void SetPort(const int portValue) { port = portValue; }
};

