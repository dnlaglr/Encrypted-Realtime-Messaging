#include "Network.h"

namespace CryptCore {

  void Server::createSocket(int PORT) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
      std::cerr << "Failed to create socket" << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
      std::cerr << "Failed to bind socket" << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, MAX_CONNECTIONS) == SOCKET_ERROR) {
      std::cerr << "Failed to listen for connections" << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }

    std::thread connectionsThread(&Server::handleConnections, this);
    connectionsThread.detach();
  }

  void Server::stopSocket() {
    closesocket(serverSocket);
    WSACleanup();
  }

  void Server::handleConnections() {
    while (isRunning) {
      sockaddr_in clientAddr;
      int clientAddrSize = sizeof(clientAddr);
      
      SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrSize);
      if (clientSocket != INVALID_SOCKET) {
        std::cout << "Client connected" << std::endl;

        std::thread clientThread(&Server::handleClient, this, clientSocket);
        clientThread.detach();
      }
    }
  }

  void Server::handleClient(SOCKET clientSocket) {
    while (isRunning) {
      int bytesRead = recv(clientSocket, messageBuffer, MAX_BUFFER_SIZE, 0);
      if (bytesRead <= 0) {
        std::cout << "Client disconnected." << std::endl;
        closesocket(clientSocket);
        std::exit(EXIT_FAILURE);
      }
      session.bytesRead += bytesRead;
      std::cout << "Client: " << messageBuffer << std::endl;
    }
  }

  void Client::createSocket(std::string serverIP, int PORT) {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
      std::cerr << "Failed to create socket" << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.S_un.S_addr = inet_addr(serverIP.c_str());
    clientAddr.sin_port = htons(PORT);

    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&clientAddr), sizeof(clientAddr)) == SOCKET_ERROR) {
      std::cerr << "Failed to connect to server" << std::endl;
      stopSocket();
      std::exit(EXIT_FAILURE);
    }

    std::cout << "Connected to server" << std::endl;

    std::thread connectionThread(&Client::recieveMessages, this);
    connectionThread.detach();

    std::thread messageThread(&Client::sendMessages, this);
    messageThread.detach();
  }

  void Client::stopSocket() {
    closesocket(clientSocket);
    WSACleanup();
  }

  void Client::recieveMessages() {
    while (isRunning) {
      int bytesRead = recv(clientSocket, messageBuffer, MAX_BUFFER_SIZE, 0);
      if (bytesRead <= 0) {
        std::cerr << "Server disconnected." << std::endl;
        stopSocket();
        std::exit(EXIT_FAILURE);
      }

      std::cout << "Server: " << messageBuffer << std::endl;
    }
  }

  void Client::sendMessages() {
    std::string message;
    while (isRunning) {
      std::cout << "Client: ";
      std::getline(std::cin, message);

      sendMessage(message);
    }
  }

  void Client::sendMessage(std::string message) {
    int messageSize = sizeof(message);
    send(clientSocket, message.c_str(), messageSize, 0);
  }

}