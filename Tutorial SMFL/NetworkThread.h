//enum class NetworkMode {
//    SERVER_COMMUNICATION,
//    PEER_TO_PEER
//};
//
//class NetworkThread {
//public:
//    void Start(NetworkMode mode);
//    void Stop();
//
//private:
//    void Run();
//    void HandleServerCommunication();
//    void HandlePeerToPeer();
//
//    std::thread thread;
//    std::atomic<bool> running = false;
//    NetworkMode currentMode;
//};
//
//void NetworkThread::Start(NetworkMode mode) {
//    currentMode = mode;
//    running = true;
//    thread = std::thread(&NetworkThread::Run, this);
//}
//
//void NetworkThread::Run() {
//    if (currentMode == NetworkMode::SERVER_COMMUNICATION) {
//        HandleServerCommunication(); // TCP: login, sala, etc.
//    }
//    else if (currentMode == NetworkMode::PEER_TO_PEER) {
//        HandlePeerToPeer(); // UDP/TCP con clientes
//    }
//}
//
//void NetworkThread::Stop() {
//    running = false;
//    if (thread.joinable()) thread.join();
//}
//
