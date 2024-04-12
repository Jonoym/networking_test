#include <enet/enet.h>
#include <iostream>

#include "Client.h"

Client::Client()
	: mClient(nullptr), mPeer(nullptr)
{
	if (enet_initialize() != 0) {
		Log("Failed to Initialise ENet Client");
		return;
	}

	atexit(enet_deinitialize);

	mClient = enet_host_create(NULL, 1, 1, 0, 0);

	if (mClient == NULL)
	{
		Log("Failed to Create ENet Client");
		return;
	}
}

void Client::Connect(const std::string& ip, int port)
{
	ENetAddress address;
	ENetEvent event;

	enet_address_set_host(&address, ip.c_str());
	address.port = port;

	mPeer = enet_host_connect(mClient, &address, 1, 0);
	if (mPeer == NULL)
	{
		Log("Failed to Connect to Server");
	}

	if (enet_host_service(mClient, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		Log("Connection to 127.0.0.1:7777 succeeded");
	}
	else
	{
		enet_peer_reset(mPeer);
		Log("Connection to 127.0.0.1:7777 failed.");
	}
}

void Client::Message(const std::string& message)
{
	const char* convertedMessage = message.c_str();

	ENetPacket* packet = enet_packet_create(convertedMessage, strlen(convertedMessage) + 1, ENET_PACKET_FLAG_RELIABLE);

	if (enet_peer_send(mPeer, 0, packet) != 0)
	{
		Log("Error with sending Message");
		enet_packet_destroy(packet);
	}
}

void Client::ReceiveMessages()
{
	ENetEvent event;

	while (enet_host_service(mClient, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			//printf("A packet of length %u containing %s was received from %s on channel %u.\n",
			//	event.packet->dataLength,
			//	event.packet->data,
			//	event.peer->data,
			//	event.channelID);

			std::cout << "[SERVER] " << event.packet->data << std::endl;

			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);

			break;
		}
	}
}


void Client::Disconnect()
{
	ENetEvent event;

	enet_peer_disconnect(mPeer, 0);

	while (enet_host_service(mClient, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			Log("Successfully Disconnected from Server");
			break;
		}
	}

	enet_peer_reset(mPeer);
}

void Client::Log(const std::string& log)
{
	std::cout << "[LOG] " << log << std::endl;
}