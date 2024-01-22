#pragma once
#pragma comment(lib, "WS2_32.lib")

#include "Core/Timer.h"

#include <iostream>
#include <thread>
#include <WinSock2.h>

#define MAX_CONNECTIONS 5
#define MAX_BUFFER_SIZE 1500

namespace CryptCore {

  struct SessionInfo {
    Timer sessionTime;
    int bytesRead = 0;
    int bytesWritten = 0;
  };

  class Network {
  public:
    Network() {
      if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
      }
    }
    ~Network() { WSACleanup(); }

  protected:
    WSADATA wsaData;
    SessionInfo session;
    char messageBuffer[MAX_BUFFER_SIZE];
    bool isRunning = true;
  };

  class Server : public Network {
  public:
    Server(int PORT) { createSocket(PORT); }
    ~Server() { stopSocket(); }

    void handleConnections();

  private:
    void createSocket(int PORT);
    void stopSocket();

    void handleClient(SOCKET clientSocket);

    SOCKET serverSocket;
    sockaddr_in serverAddr;
  };

  class Client : public Network {
  public:
    Client() { createSocket("127.0.0.1", 8161); }

    void sendMessages();
    void sendMessage(std::string message);
    void recieveMessages();

  private:
    void createSocket(std::string serverIP, int PORT);
    void stopSocket();

    SOCKET clientSocket;
    sockaddr_in clientAddr;
  };

}