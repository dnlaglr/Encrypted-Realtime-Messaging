#include "Core/Core.h"
#include "Core/Network/Network.h"

int main() {
	CryptCore::PrintHelloWorld();

	CryptCore::Network server("127.0.0.1", 8181);
	server.startListening();

	return 0;
}