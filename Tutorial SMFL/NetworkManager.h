#pragma once
#include <SFML/Network.hpp>
#include <string>
#include <iostream>
#include "Client.h"

#include "CustomPacket.h"
#include <thread>
#include <mutex>
#include "NetworkState.h"

#define NETWORK NetworkManager::Instance()

const int SERVER_PORT = 55001;
//const sf::IpAddress SERVER_IP = sf::IpAddress(93, 176, 163, 135);
const sf::IpAddress SERVER_IP = sf::IpAddress(192, 168, 1, 144);
//const sf::IpAddress SERVER_IP = sf::IpAddress(10,40,1,99);
//const sf::IpAddress SERVER_IP = sf::IpAddress(192,168,1,71);

class NetworkManager
{
private:

    std::shared_ptr<sf::TcpSocket> serverSocket;
    std::vector<std::shared_ptr<Client>> p2pClients;

    sf::IpAddress serverIp = SERVER_IP;
    int serverPort;

    NetworkState currentState;
    sf::SocketSelector socketSelector;
    sf::TcpListener listener;

    std::thread networkThread;
    std::mutex stateMutex;
    std::mutex selectorMutex;

    bool isRunning;

    NetworkManager() {};
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    void HandleServerCommunication();
    void HandleP2PCommunication();

public:

    inline static NetworkManager& Instance()
    {
        static NetworkManager manager;
        return manager;
    }

    ~NetworkManager();

    void Init();
    void Start();
    void Update();
    void Stop();

    void StartListening();
    void StartClientConnections(const std::vector < std::shared_ptr<Client>>& newClients, int myIndex, int port);
    bool ConnectToServer();
    void DisconnectServer();
    void DisconnectAllPeers();

    void HandleNewConnections();
    void UpdateP2PClients();

    void ChangeState(NetworkState newState);
    void RefreshSelector();

    std::shared_ptr<sf::TcpSocket> GetServerSocket() const { return serverSocket; }
    std::vector<std::shared_ptr<Client>>& GetClients() { return p2pClients; }
    int GetListeningPort() const { return listener.getLocalPort(); }
    NetworkState GetNetworkState();
    sf::TcpListener& GetListener() { return listener; }
    std::shared_ptr<Client> GetClientByGuid(const std::string& guid);
    std::mutex& GetSelectorMutex() { return selectorMutex; }
    sf::SocketSelector& GetSocketSelector() { return socketSelector; }
};