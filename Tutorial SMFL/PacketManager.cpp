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

	EVENT_MANAGER.TCPSubscribe(MATCH_FOUND, [this](CustomTCPPacket& customPacket) {
		std::cout << "Match Found" << std::endl;
		NETWORK.DisconnectTCPServer();

		std::string ipString;
		int port;
		int enemyId;

		customPacket.packet >> globalIdPlayer;
		customPacket.packet >> enemyId;
		customPacket.packet >> ipString;
		customPacket.packet >> port;

		std::optional<sf::IpAddress> ipAddress = sf::IpAddress::resolve(ipString);

		NETWORK.ConnectToUDPServer(*ipAddress, port);

		if (globalIdPlayer < enemyId)
			localIdPlayer = 0;
		else
			localIdPlayer = 1;

		std::cout << "I'm the player with the ID: " << localIdPlayer << std::endl;

		CustomUDPPacket _customPacket(UdpPacketType::CRITIC, SEND_CLIENT_INFORMATION, globalIdPlayer);
		_customPacket.WriteVariable(0);
		criticsPacketsClient.push_back(_customPacket);

		SendPacketToUDPServer(_customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(START_GAME, [this](CustomUDPPacket& customPacket) {
		SCENE.ChangeScene(new GameScene());
		
		SCENE.GetCurrentScene()->SetCurrentPlayer(localIdPlayer);
		GAME.GetReferencePlayer()->AddIdCritic();
		});

	EVENT_MANAGER.UDPSubscribe(SEND_POSITION, [this](CustomUDPPacket& customPacket) {
		std::cout << "Position Send"<<std::endl;
		SendPacketToUDPServer(customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(VALIDATION_BACK, [this](CustomUDPPacket& customPacket) {
		std::cout << "Return to a valid Position" << std::endl;

		int returnId = 0;
		customPacket.ReadVariable(returnId, customPacket.payloadOffset);

		GAME.GetReferencePlayer()->BacktToValidPosition(returnId);
		});

	EVENT_MANAGER.UDPSubscribe(VALIDATION_OK, [this](CustomUDPPacket& customPacket) {
		std::cout << "All movement are OK" << std::endl;
		GAME.GetReferencePlayer()->ResetPositionsPackets();
		});

	EVENT_MANAGER.UDPSubscribe(INTERPOLATION_POSITION, [this](CustomUDPPacket& customPacket) {
		std::cout << "Get enemy position" << std::endl;
		
		int id = 0;
		sf::Vector2f _position;

		size_t size = customPacket.payloadOffset;

		customPacket.ReadVariable(id, size);
		customPacket.ReadVariable(_position.x, size);
		customPacket.ReadVariable(_position.y, size);

		GAME.GetEnemyPlayer()->AddEnemyPosition(_position, id);

		if (customPacket.udpType == UdpPacketType::CRITIC)
		{
			GAME.GetEnemyPlayer()->RestartElapsedTime();
			GAME.GetEnemyPlayer()->SetStartInterpolate(true);
		}

		});

	EVENT_MANAGER.UDPSubscribe(SEND_START_SHOOT, [this](CustomUDPPacket& customPacket) {
		std::cout << "Send Start Shoot" << std::endl;
		criticsPacketsClient.push_back(customPacket);
		SendPacketToUDPServer(customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(SEND_STOP_SHOOT, [this](CustomUDPPacket& customPacket) {
		std::cout << "Send Stop Shoot" << std::endl;
		criticsPacketsClient.push_back(customPacket);
		SendPacketToUDPServer(customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(SEND_MOCKERY, [this](CustomUDPPacket& customPacket) {
		std::cout << "Send Mockery" << std::endl;
		criticsPacketsClient.push_back(customPacket);
		SendPacketToUDPServer(customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(RECEIVE_START_SHOOT, [this](CustomUDPPacket& customPacket) {
		std::cout << "Start Shoot" << std::endl;
		GAME.GetEnemyPlayer()->SetShootRequested(true);
		});

	EVENT_MANAGER.UDPSubscribe(RECEIVE_STOP_SHOOT, [this](CustomUDPPacket& customPacket) {
		std::cout << "Stop Shoot" << std::endl;
		//Activar que el otro jugador pare de disparar
		GAME.GetEnemyPlayer()->SetShootRequested(false);
		});

	EVENT_MANAGER.UDPSubscribe(RECEIVE_MOCKERY, [this](CustomUDPPacket& customPacket) {
		std::cout << "Do Mockery" << std::endl;
		GAME.GetEnemyPlayer()->SetMockeryRequested(true);
		});

	EVENT_MANAGER.UDPSubscribe(SEND_ACK, [this](CustomUDPPacket& customPacket) {
		std::cout << "Send ACK packet" << std::endl;

		//Create UDP Packet 
		CustomUDPPacket _customPacket(UdpPacketType::NORMAL, RECEIVE_ACK, globalIdPlayer);

		//Read the ID from the customPacket buffer
		int idMessage = 0;
		size_t size = customPacket.payloadOffset;
		customPacket.ReadVariable(idMessage, size);

		//Reload the buffer with the ID from the customPacket
		_customPacket.WriteVariable(idMessage);

		//EL paquete es (NORMAL, RECEIVE_ACK, PLAYERID, ID del paquete critico
		SendPacketToUDPServer(_customPacket);
		});

	EVENT_MANAGER.UDPSubscribe(RECEIVE_ACK, [this](CustomUDPPacket& customPacket) {
		std::cout << "Receive ACK packet" << std::endl;

		//Read the critic id 
		int incomingId = 0;
		size_t offset = customPacket.payloadOffset;
		customPacket.ReadVariable(incomingId, offset);
		
		auto it = std::remove_if(criticsPacketsClient.begin(), criticsPacketsClient.end(),
			[this, incomingId](const CustomUDPPacket& pkt) {
				int pktId = 0;
				size_t pktOffset = pkt.payloadOffset;
				pkt.ReadVariable(pktId, pktOffset);
				return pktId == incomingId;
			});

		if (it != criticsPacketsClient.end()) {
			criticsPacketsClient.erase(it, criticsPacketsClient.end());
			std::cout << "Removed ACK packet with ID " << incomingId << std::endl;
		}
		else {
			std::cout << "No matching packet found to remove" << std::endl;
		}

		});
}

int PacketManager::GetCriticId(CustomUDPPacket packet)
{
	int idMessage = 0;
	size_t offset = packet.payloadOffset;
	packet.ReadVariable(idMessage, offset);
	return idMessage;
}

void PacketManager::ProcessTCPReceivedPacket(CustomTCPPacket& customPacket)
{
	customPacket.packet >> customPacket.type;
	std::cout << customPacket.type << std::endl;
	EVENT_MANAGER.TCPEmit(customPacket.type, customPacket);
}

void PacketManager::ProcessUDPReceivedPacket(CustomUDPPacket& customPacket)
{
	switch (customPacket.udpType)
	{
	case UdpPacketType::NORMAL:
		EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);
		break;
	case UdpPacketType::URGENT:
		break;
	case UdpPacketType::CRITIC:
		EVENT_MANAGER.UDPEmit(SEND_ACK, customPacket);

		int incomingId = GetCriticId(customPacket);

		auto it = std::find_if(criticsPacketsServer.begin(), criticsPacketsServer.end(),
			[this, incomingId](const CustomUDPPacket& pkt) {
				return this->GetCriticId(pkt) == incomingId;
			});

		if (it == criticsPacketsServer.end())
		{
			EVENT_MANAGER.UDPEmit(customPacket.type, customPacket);
			criticsPacketsServer.push_back(customPacket);
		}
		else
			std::cout << "Critic Packet processed" << std::endl;
		break;
	}
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

void PacketManager::SendCriticsPackets()
{
	for (int i = 0; i < criticsPacketsClient.size(); i++)
	{
		SendPacketToUDPServer(criticsPacketsClient[i]);
	}
}

