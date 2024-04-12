#include <iostream>
#include <sstream>

#include "Server.h"

Server::Server()
	: mServer(nullptr)
{
	if (enet_initialize() != 0)
	{
		Log("Failed to Initialise ENet Server");
		return;
	}

	atexit(enet_deinitialize);

}

void Server::Host(int port)
{
	ENetAddress address;
	ENetEvent event;

	address.host = ENET_HOST_ANY;
	address.port = port;

	mServer = enet_host_create(&address, 32, 1, 0, 0);

	if (mServer == NULL)
	{
		Log("Failed to Host Server");
	}


	while (true)
	{
		while (enet_host_service(mServer, &event, 1000) > 0)
		{
			std::stringstream ss;
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				Log("Client Connected");
				//printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				//printf("A packet of length %u containing %s was received from %s on channel %u.\n",
				//	event.packet->dataLength,
				//	event.packet->data,
				//	event.peer->data,
				//	event.channelID);
				std::cout << "[MESSAGE] " << event.packet->data << std::endl;

				ss << event.packet->data;

				Broadcast(ss.str());
				enet_packet_destroy(event.packet);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				Log("Client Disconnected");
				//printf("%x:%u disconnected.\n", event.peer->address.host, event.peer->address.port);
				break;
			}
		}
	}
}

void Server::Shutdown()
{
	enet_host_destroy(mServer);
}

void Server::Broadcast(const std::string& message)
{
	const char* convertedMessage = message.c_str();

	ENetPacket* packet = enet_packet_create(convertedMessage, strlen(convertedMessage) + 1, ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast(mServer, 0, packet);
}

void Server::Log(const std::string& log)
{
	std::cout << "[SERVER] " << log << std::endl;
}