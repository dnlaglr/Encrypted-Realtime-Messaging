#include "Network.h"

namespace CryptCore {
  
  void Network::initSocket(const char* ipAddr, int port) {
    std::cout << "Initializing socket" << std::endl;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      std::cerr << "Failed to initialize WinSock2." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
      std::cerr << "Failed to create socket." << std::endl;
      std::exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = inet_addr(ipAddr);
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
      std::cerr << "Failed to bind socket." << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_SOCKET_CONNECTIONS) == SOCKET_ERROR) {
      std::cerr << "Failed to listen for connections" << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }
  }

  void Network::startListening() {
    std::cout << "Server is listening for incoming connections on: " << serverAddr.sin_addr.S_un.S_addr << ": " << serverAddr.sin_port << std::endl;
    std::cout.flush();
  }

  void Network::stopSocket() {
    closesocket(serverSocket);
    WSACleanup();
  }
}