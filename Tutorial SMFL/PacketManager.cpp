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
	CustomPacket customPacket(HANDSHAKE);
	customPacket.packet << "Hello Server, I'm the new client";

	SendPacketToServer(customPacket);
}

void PacketManager::Init()
{
	EVENT_MANAGER.Subscribe(HANDSHAKE, [this](CustomPacket& customPacket) {
		HandleHandshake(customPacket.packet);
		});

	EVENT_MANAGER.Subscribe(REGISTER, [this](CustomPacket& customPacket) {
		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToServer(customPacket);

		});

	EVENT_MANAGER.Subscribe(REGISTER_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;
		
		std::cout << "Register error: " << responseMessage << std::endl;
		});

	EVENT_MANAGER.Subscribe(REGISTER_SUCCES, [this](CustomPacket& customPacket) {
		std::cout << "Register succes" << std::endl;
		SCENE.ChangeScene(new MatchMackingScene());
		});

	EVENT_MANAGER.Subscribe(LOGIN, [this](CustomPacket& customPacket) {

		std::string username;
		std::string password;
		customPacket.packet >> username >> password;

		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(LOGIN_ERROR, [this](CustomPacket& customPacket) {
		std::string responseMessage;

		customPacket.packet >> responseMessage;

		std::cout << "Login error: " << responseMessage << std::endl;

		});

	EVENT_MANAGER.Subscribe(LOGIN_SUCCESS, [this](CustomPacket& customPacket) {
		std::cout << "Login succes" << std::endl;
		SCENE.ChangeScene(new MatchMackingScene());
		});

	EVENT_MANAGER.Subscribe(START_GAME, [this](CustomPacket& customPacket) {
		std::cout << "Start Game" << std::endl;

		

		});


	EVENT_MANAGER.Subscribe(DISCONNECT, [this](CustomPacket& customPacket) {

		
		});

	EVENT_MANAGER.Subscribe(CANCEL_QUEUE, [this](CustomPacket& customPacket) {
		std::cout << "Cancel Queue" << std::endl;
		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(START_FRENDLY_QUEUE, [this](CustomPacket& customPacket) {
		std::cout << "Start Friendly Queue" << std::endl;
		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(ASK_MAP, [this](CustomPacket& customPacket) {
		std::cout << "Ask Map" << std::endl;
		SendPacketToServer(customPacket);
		});

	EVENT_MANAGER.Subscribe(RECEIVE_MAP, [this](CustomPacket& customPacket) {
		std::cout << "Map Received" << std::endl;

		std::string jsonContent;
		customPacket.packet >> jsonContent;

		SCENE.ReadMap(jsonContent);
		SCENE.InitScenes(new RegisterScene());
		});	
}

void PacketManager::ProcessReceivedPacket(CustomPacket& customPacket)
{
	customPacket.packet >> customPacket.type;
	//std::cout << customPacket.type << std::endl;
	EVENT_MANAGER.Emit(customPacket.type, customPacket);
}

void PacketManager::SendPacketToClient(const std::shared_ptr<Client> client, CustomPacket& responsePacket)
{
	sf::Socket::Status status = client->GetNetwork().GetSocket().send(responsePacket.packet);
	if (status == sf::Socket::Status::Done)
		std::cout << "Message sent to client: " <<client->GetNetwork().GetIp()<<" "<<client->GetNetwork().GetPort() << std::endl;
	else
	{
		std::cout << "The error is: " << static_cast<int>(status) << std::endl;	}
}

void PacketManager::SendPacketToServer(CustomPacket& customPacket)
{
	if (NETWORK.GetServerSocket()->send(customPacket.packet) == sf::Socket::Status::Done)
	{
		std::cout << "Packet send to server" << std::endl;
	}
	else
		std::cout << "Error sending packet to server" << std::endl;
}

