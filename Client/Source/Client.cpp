#include "Core/Core.h"
#include "Core/Network/Network.h"

int main() {
	CryptCore::PrintHelloWorld();

	CryptCore::Server server(8161);
	std::thread serverThread([&server] {
		server.handleConnections();
	});

	std::this_thread::sleep_for(std::chrono::seconds(1));

	CryptCore::Client client;
	std::thread clientThread([&client] {
		client.recieveMessages();
	});

	serverThread.join();
	clientThread.join();

	return 0;
}