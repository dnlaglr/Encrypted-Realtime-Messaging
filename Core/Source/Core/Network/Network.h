#pragma once

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "Ws2_32.lib")

#define MAX_SOCKET_CONNECTIONS 5

namespace CryptCore {

  class Network {
  public:
    Network(const char* ipAddr, int port) {
      std::cout << "Network Object Constructed." << std::endl;
      initSocket(ipAddr, port);
    }
    ~Network() { stopSocket(); }

    void startListening();

  private:
    void initSocket(const char* ipAddr, int port);
    void stopSocket();

    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;
  };

}