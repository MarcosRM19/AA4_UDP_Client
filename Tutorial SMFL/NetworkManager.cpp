#include "NetworkManager.h"
#include "PacketManager.h"
#include "GameManager.h"
#include <optional>



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
				else
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

void NetworkManager::HandleP2PCommunication()
{

	while (true)
	{
		NetworkState state;

		{
			std::lock_guard lock(stateMutex);
			state = currentState;
		}

		if (state != NetworkState::CONNECTED_TO_PEERS)
				break;

		if (socketSelector.wait(sf::seconds(0.1f)))
		{
			if (socketSelector.isReady(listener))
			{
				HandleNewConnections();
			}
			else
			{
				UpdateP2PClients();
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
	case NetworkState::CONNECTED_TO_PEERS:
		std::cout << "Update p2p" << std::endl;
		HandleP2PCommunication();
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
	DisconnectAllPeers();
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

void NetworkManager::StartClientConnections(const std::vector<std::shared_ptr<Client>>& newClients, int myIndex, int port)
{
	p2pClients = newClients;

	// Dirección IP local y puerto de escucha
	std::optional<sf::IpAddress> localIp = sf::IpAddress::getLocalAddress();
	int localPort = NETWORK.GetListeningPort();

	// Primero, los clientes con índices menores intentan conectarse a los mayores
	for (int i = 0; i < myIndex; ++i) // Índices menores
	{
		std::shared_ptr<Client>& newClient = p2pClients[i];
		NetworkClient& network = newClient->GetNetwork();
		std::optional<sf::IpAddress> ipAddress = sf::IpAddress::resolve(network.GetIp());

		if (!ipAddress)
			continue;

		// Intentamos conectar al cliente con índice mayor
		sf::Socket::Status status = network.GetSocket().connect(*ipAddress, network.GetPort());

		if (status == sf::Socket::Status::Done)
		{
			network.GetSocket().setBlocking(false);

			{
				std::lock_guard<std::mutex> lock(selectorMutex);
				socketSelector.add(network.GetSocket());
			}

			std::cout << "Connected to peer " << network.GetIp() << ":" << network.GetPort() << std::endl;

			// Enviar el paquete de handshake
			PACKET_MANAGER.SendHandshakeP2P(newClient);
		}
		else
		{
			std::cerr << "Failed to connect to peer " << network.GetIp() << std::endl;
		}
	}

	// Ahora, los clientes con índices mayores aceptan conexiones de los menores
	for (int i = myIndex + 1; i < p2pClients.size(); ++i) // Índices mayores
	{
		std::shared_ptr<Client>& newClient = p2pClients[i];
		NetworkClient& network = newClient->GetNetwork();

		// Usamos listener para aceptar conexiones entrantes
		sf::TcpListener& listener = NETWORK.GetListener();

		std::shared_ptr<sf::TcpSocket> newSocket = std::make_shared<sf::TcpSocket>();
		if (listener.accept(*newSocket) == sf::Socket::Status::Done)
		{
			newSocket->setBlocking(false);
			
			{
				std::lock_guard<std::mutex> lock(selectorMutex);
				socketSelector.add(*newSocket);
			}

			// Asignar el socket al cliente
			network.SetSocket(newSocket);

			std::cout << "Listening for connections from " << newClient->GetNetwork().GetIp() << std::endl;

			// Procesar handshake
			PACKET_MANAGER.SendHandshakeP2P(newClient);
		}
		else
		{
			std::cerr << "Failed to accept connection for peer " << newClient->GetNetwork().GetIp() << std::endl;
		}
	}

	ChangeState(NetworkState::CONNECTED_TO_PEERS);
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

void NetworkManager::DisconnectAllPeers()
{
	for (std::shared_ptr<Client> client : p2pClients)
	{
		if (client)
		{
			client->GetNetwork().GetSocket().disconnect();
			socketSelector.remove(client->GetNetwork().GetSocket());
			client->GetNetwork().GetSocket();
		}
	}

	p2pClients.clear();


	ChangeState(NetworkState::DISCONNECTED);
	std::cout << "Disconnected from all peers" << std::endl;
}

void NetworkManager::HandleNewConnections()
{
	std::shared_ptr<sf::TcpSocket> newSocket = std::make_shared<sf::TcpSocket>();
	if (listener.accept(*newSocket) == sf::Socket::Status::Done)
	{
		std::optional<sf::IpAddress> remoteIp = newSocket->getRemoteAddress();
		unsigned short remotePort = newSocket->getRemotePort();

		if (!remoteIp)
		{
			std::cerr << "Failed top get remote IP adress" << std::endl;

		}
		std::cout << "Incoming connection from: " << *remoteIp << ":" << remotePort << std::endl;

		bool linked = false;

		for (std::shared_ptr<Client>& client : p2pClients)
		{
			std::string expectedIp = client->GetNetwork().GetIp();
			int expectedPort = client->GetNetwork().GetPort();

			if (remoteIp->toString() == expectedIp && remotePort == expectedPort)
			{
				std::cout << " Link connection with existing client: " << expectedIp << ":" << expectedPort << std::endl;

				sf::TcpSocket& oldSocket = client->GetNetwork().GetSocket();

				{
					std::lock_guard<std::mutex> selectorLock(selectorMutex);
					socketSelector.remove(oldSocket); // Quita socket viejo
				}
				oldSocket.disconnect(); // Cierra el viejo

				client->GetNetwork().SetSocket(newSocket); // Asigna el nuevo
				newSocket->setBlocking(false);

				{
					std::lock_guard<std::mutex> selectorLock(selectorMutex);
					socketSelector.add(*newSocket); // Añade el nuevo
				}

				linked = true;
				break;
			}
		}

		if (!linked)
		{
			std::cerr << " Unrecognized connection attempt from: " << *remoteIp << ":" << remotePort << std::endl;
			newSocket->disconnect();
		}
	}
}

void NetworkManager::UpdateP2PClients()
{
	for (std::shared_ptr<Client>& client : p2pClients)
	{
		if (!client)
			continue;

		sf::TcpSocket& socket = client->GetNetwork().GetSocket();
		if (!NETWORK.GetSocketSelector().isReady(socket)) 
			continue;

		if (socket.getRemoteAddress() == sf::IpAddress::Any)
			continue;

		CustomPacket customPacket;
		sf::Socket::Status status = socket.receive(customPacket.packet);

		switch (status)
		{
		case sf::Socket::Status::Done :
			PACKET_MANAGER.ProcessReceivedPacket(customPacket);
			break;

		case sf::Socket::Status::Disconnected:
		{
			std::cerr << "[P2P] Client disconnected: "<< *socket.getRemoteAddress() << ":" << socket.getRemotePort() << std::endl;

				socket.disconnect();
			{
				std::lock_guard<std::mutex> lock(selectorMutex);
				socketSelector.remove(socket);
			}
			break;
		}
		default:
			std::cerr << "Error receiving packet: " << static_cast<int>(status) << std::endl;
			break;
		}
	}
}

void NetworkManager::RefreshSelector()
{
	std::lock_guard lock(selectorMutex);
	socketSelector.clear();

	if (currentState == NetworkState::CONNECTED_TO_SERVER)
	{
		socketSelector.add(*serverSocket);
	}
	else if (currentState == NetworkState::CONNECTED_TO_PEERS)
	{
		for (std::shared_ptr<Client> client : p2pClients)
			socketSelector.add(client->GetNetwork().GetSocket());

		socketSelector.add(listener);
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
