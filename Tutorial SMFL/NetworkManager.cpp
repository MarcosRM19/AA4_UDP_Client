#include "NetworkManager.h"
#include "PacketManager.h"
#include "GameManager.h"
#include <optional>
#include "EventManager.h"



void NetworkManager::HandleServerCommunication()
{
	while (true)
	{
		NetworkState state;

		{
			std::lock_guard lock(stateMutex);
			state = currentState;
		}

		if (state != NetworkState::CONNECTED_TO_SERVER)
			break;

		if (socketSelector.wait(sf::seconds(0.1f)))
		{
			if (socketSelector.isReady(*serverSocket))
			{
				CustomPacket customPacket;

				sf::Socket::Status status = serverSocket->receive(customPacket.packet);

				if (status == sf::Socket::Status::Done)
				{
					PACKET_MANAGER.ProcessReceivedPacket(customPacket);
				}
				else if(status == sf::Socket::Status::Disconnected)
				{
					std::cerr << "The server has disconnected" << std::endl;
					serverSocket->disconnect();

					{
						std::lock_guard<std::mutex> lock(selectorMutex);
						socketSelector.remove(*serverSocket);
					}

					serverSocket.reset();
				}
			}
		}
	}
}

NetworkManager::~NetworkManager()
{
	Stop();
}

void NetworkManager::Init()
{
	serverSocket = std::make_shared<sf::TcpSocket>();
	serverIp = SERVER_IP;
	serverPort = SERVER_PORT;
	currentState = NetworkState::DISCONNECTED;
	isRunning = false;
}

void NetworkManager::Start()
{
	isRunning = true;
	networkThread = std::thread([this]() {
		while (isRunning) 
		{
			Update();
		}
		});	
}

void NetworkManager::Update()
{
	NetworkState state;

	{
		std::lock_guard lock(stateMutex);
		state = currentState;
	}

	switch (state) {
	case NetworkState::CONNECTED_TO_SERVER:
		HandleServerCommunication();
		break;
	default:
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		break;
	}
}

void NetworkManager::Stop()
{
	isRunning = false;
	if (networkThread.joinable())
		networkThread.join();

	DisconnectServer();
}

void NetworkManager::ChangeState(NetworkState newState)
{
	{
		std::lock_guard lock(stateMutex);
		std::cout << "state changed to :" << static_cast<int>(newState) << std::endl;
		currentState = newState;
	}
	RefreshSelector();
}

void NetworkManager::StartListening()
{
	if (listener.listen(0) == sf::Socket::Status::Done)
		std::cout << "Listening on port: " << listener.getLocalPort() << std::endl;
	else
		std::cerr << "Failed to start Listening" << std::endl;
}

bool NetworkManager::ConnectToServer()
{
	if (!serverSocket) // We check if we should Reinitialize the socket because we delete the socket when client disconnects from server
		serverSocket = std::make_shared<sf::TcpSocket>();

	std::cout << serverIp << " " << serverPort << std::endl;

	sf::Socket::Status status = serverSocket->connect(serverIp, serverPort);

	if (status == sf::Socket::Status::Done)
	{
		serverSocket->setBlocking(false);
		ChangeState(NetworkState::CONNECTED_TO_SERVER);
		std::cout << "Connected To server" << std::endl;

		StartListening();

		return true;
	}
	else if (status == sf::Socket::Status::NotReady)
	{
		if (socketSelector.wait())
		{
			if (socketSelector.isReady(*serverSocket))
			{
				ChangeState(NetworkState::CONNECTED_TO_SERVER);
				std::cout << "Connected To server" << std::endl;
			}
		}
	}

	std::cerr << "Can't connect to server because: " << static_cast<int>(status) << std::endl;
	return false;
}

void NetworkManager::DisconnectServer()
{
	if (serverSocket)
	{
		serverSocket->disconnect();
		{
			std::lock_guard lock(selectorMutex);
			socketSelector.remove(*serverSocket);
		}
		serverSocket.reset(); 	
	}

	ChangeState(NetworkState::DISCONNECTED);
	std::cout << "Disconnected from server" << std::endl;
}

void NetworkManager::RefreshSelector()
{
	std::lock_guard lock(selectorMutex);
	socketSelector.clear();

	if (currentState == NetworkState::CONNECTED_TO_SERVER)
	{
		socketSelector.add(*serverSocket);
	}
}

NetworkState NetworkManager::GetNetworkState()
{

	std::lock_guard lock(stateMutex);
	return currentState;
}

std::shared_ptr<Client> NetworkManager::GetClientByGuid(const std::string& guid)
{
	const auto clientIt = std::find_if(p2pClients.begin(), p2pClients.end(),
		[guid](const std::shared_ptr<Client>& client) {
			std::cerr << "Client with GUID " << guid << " found in p2pClients Vector" << std::endl;

			return client->GetNetwork().GetGuid() == guid;
		});

	if (clientIt != p2pClients.end())
		return *clientIt;

	std::cerr << "Client with GUID " << guid << " not found in p2pClients Vector" << std::endl;
	return nullptr;
}
