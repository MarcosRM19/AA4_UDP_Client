#include "PacketManager.h"
#include <iostream>
#include "EventManager.h"
#include "NetworkManager.h"
#include "SceneManager.h"
#include "GameManager.h"

void PacketManager::HandleHandshake(sf::Packet& packet)
{
	std::string messageFromServer;
	packet >> messageFromServer;

	std::cout << "Messages received from server: " << messageFromServer << std::endl;
}

void PacketManager::HandleTest(sf::Packet& packet)
{
	std::string message;
	packet >> message;

	std::cout << "Message received from server: " << message << std::endl;
}

void PacketManager::SendHandshake(const std::string guid)
{
	CustomTCPPacket customPacket(HANDSHAKE);
	customPacket.packet << "Hello Server, I'm the new client";

	SendPacketToTCPServer(customPacket);
}

void PacketManager::Init()
{
	EVENT_MANAGER.TCPSubscribe(HANDSHAKE, [this](CustomTCPPacket& customPacket) {
		HandleHandshake(customPacket.packet);
		});

	EVENT_MANAGER.TCPSubscribe(REGISTER, [this](CustomTCPPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToTCPServer(customPacket);

		});

	EVENT_MANAGER.TCPSubscribe(REGISTER_ERROR, [this](CustomTCPPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;
		
		std::cout << "Register error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.TCPSubscribe(REGISTER_SUCCESS, [this](CustomTCPPacket& customPacket) {
		std::cout << "Register succes" << std::endl;
		SCENE.ChangeScene(new MatchMackingScene());
		});

	EVENT_MANAGER.TCPSubscribe(LOGIN, [this](CustomTCPPacket& customPacket) {

		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToTCPServer(customPacket);
		});

	EVENT_MANAGER.TCPSubscribe(LOGIN_ERROR, [this](CustomTCPPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Login error: " << responseMessage << std::endl;

		});

	EVENT_MANAGER.TCPSubscribe(LOGIN_SUCCESS, [this](CustomTCPPacket& customPacket) {
		std::cout << "Login succes" << std::endl;
		SCENE.ChangeScene(new MatchMackingScene());
		});

	EVENT_MANAGER.TCPSubscribe(START_GAME, [this](CustomTCPPacket& customPacket) {
		std::cout << "Start Game" << std::endl;		
		NETWORK.DisconnectTCPServer();

		std::string ipString;
		int port;

		customPacket.packet >> ipString;
		customPacket.packet >> port;

		std::optional<sf::IpAddress> ipAddress = sf::IpAddress::resolve(ipString);

		NETWORK.ConnectToUDPServer(*ipAddress, port);
		});


	EVENT_MANAGER.TCPSubscribe(DISCONNECT, [this](CustomTCPPacket& customPacket) {

		
		});

	EVENT_MANAGER.TCPSubscribe(START_QUEUE, [this](CustomTCPPacket& customPacket) {
		std::cout << "Start Queue" << std::endl;
		SendPacketToTCPServer(customPacket);
		});

	EVENT_MANAGER.TCPSubscribe(START_QUEUE_RESULT, [this](CustomTCPPacket& customPacket) {
		std::string result;
		customPacket.packet >> result;
		
		std::cout << result << std::endl;
		});

	EVENT_MANAGER.TCPSubscribe(CANCEL_QUEUE_RESULT, [this](CustomTCPPacket& customPacket) {
		std::string result;
		customPacket.packet >> result;

		std::cout << result << std::endl;
		});

	EVENT_MANAGER.TCPSubscribe(CANCEL_QUEUE, [this](CustomTCPPacket& customPacket) {
		std::cout << "Cancel Queue" << std::endl;
		SendPacketToTCPServer(customPacket);
		});

	EVENT_MANAGER.TCPSubscribe(ASK_MAP, [this](CustomTCPPacket& customPacket) {
		std::cout << "Ask Map" << std::endl;
		SendPacketToTCPServer(customPacket);
		});

	EVENT_MANAGER.TCPSubscribe(RECEIVE_MAP, [this](CustomTCPPacket& customPacket) {
		std::cout << "Map Received" << std::endl;

		std::string jsonContent;
		customPacket.packet >> jsonContent;

		GAME.SetJson(jsonContent);
		SCENE.ChangeScene(new RegisterScene());
		SCENE.SetLauncherFinished(true);
		});	

	EVENT_MANAGER.UDPSubscribe(START_UDP_GAME, [this](CustomUDPPacket& customPacket) {
		SCENE.ChangeScene(new GameScene());
		});

	EVENT_MANAGER.UDPSubscribe(SEND_POSITION, [this](CustomUDPPacket& customPacket) {
		std::cout << "Position Send"<<std::endl;
		SendPacketToUDPServer(customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(ACK, [this](CustomUDPPacket& customPacket) {
		std::cout << "Send ACK packet" << std::endl;
		SendPacketToUDPServer(customPacket);
		});
}

void PacketManager::ProcessTCPReceivedPacket(CustomTCPPacket& customPacket)
{
	customPacket.packet >> customPacket.type;
	std::cout << customPacket.type << std::endl;
	EVENT_MANAGER.TCPEmit(customPacket.type, customPacket);
}

void PacketManager::ProcessUDPReceivedPacket(CustomUDPPacket& customPacket)
{
	std::cout << customPacket.type << std::endl;
	EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);
}

void PacketManager::SendPacketToUDPServer(CustomUDPPacket& responsePacket)
{
	if (NETWORK.GetUDPSocket()->send(responsePacket.buffer, responsePacket.bufferSize, NETWORK.GetUDPIPAdrres(), NETWORK.GetUDPPort()) == sf::Socket::Status::Done)
	{
		std::cout << "Packet send to UDP server" << std::endl;
	}
	else
	{
		std::cout << "Error sending packet to UDP server" << std::endl;
	}
}

void PacketManager::SendPacketToTCPServer(CustomTCPPacket& customPacket)
{
	if (NETWORK.GetServerSocket()->send(customPacket.packet) == sf::Socket::Status::Done)
	{
		std::cout << "Packet send to server" << std::endl;
	}
	else
		std::cout << "Error sending packet to server" << std::endl;
}

