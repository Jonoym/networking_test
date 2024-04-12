#pragma once

#include <enet/enet.h>
#include <string>

class Server
{
public:

	Server();

	void Host(int port);
	void Shutdown();
		
private:

	void Broadcast(const std::string& message);

	void Log(const std::string& message);

	ENetHost* mServer;
};