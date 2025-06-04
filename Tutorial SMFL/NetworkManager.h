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
const sf::IpAddress SERVER_IP = sf::IpAddress(93, 176, 163, 135);
//const sf::IpAddress SERVER_IP = sf::IpAddress(192, 168, 1, 144);
//const sf::IpAddress SERVER_IP = sf::IpAddress(10,40,1,99);
//const sf::IpAddress SERVER_IP = sf::IpAddress(192,168,1,71);

const sf::Time intervalCriticPacket = sf::seconds(0.5f);
const sf::Time originalPingTime = sf::seconds(0.5f);
const sf::Time pingTimeExtra = sf::seconds(2.f);

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

    std::shared_ptr<sf::UdpSocket> udpSocket;

    std::thread networkThread;
    std::mutex stateMutex;
    std::mutex selectorMutex;

    sf::IpAddress udpServerIP = sf::IpAddress(10, 40, 1, 99);
    int udpServerPort;
    char udpBuffer[1024];
    std::size_t udpReceivedSize;

    sf::Clock pingClock;
    sf::Time currentPingTime = originalPingTime;
    bool isRunning;

    NetworkManager() {};
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    void HandleServerCommunication();
    void HandleUDPServerCommunication();
    void HandleCriticCommunication();

    sf::Clock sendCriticPackets;
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
    bool ConnectToServer();
    void DisconnectTCPServer();
    void DisconnectUDPServer();
    void ConnectToUDPServer(sf::IpAddress ip, int port);

    void ChangeState(NetworkState newState);
    void RefreshSelector();

    void ClearBuffer();

    std::shared_ptr<sf::TcpSocket> GetServerSocket() const { return serverSocket; }
    std::vector<std::shared_ptr<Client>>& GetClients() { return p2pClients; }
    int GetListeningPort() const { return listener.getLocalPort(); }
    NetworkState GetNetworkState();
    sf::TcpListener& GetListener() { return listener; }
    std::shared_ptr<Client> GetClientByGuid(const std::string& guid);
    std::mutex& GetSelectorMutex() { return selectorMutex; }
    sf::SocketSelector& GetSocketSelector() { return socketSelector; }
    void ResetClock() { pingClock.restart(); }

    inline std::shared_ptr<sf::UdpSocket> GetUDPSocket() const { return udpSocket; }
    inline int GetUDPPort() const { return udpServerPort; }
    inline sf::IpAddress GetUDPIPAdrres() const { return udpServerIP; }

};