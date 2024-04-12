#pragma once
#include <string>

class Client
{

public:

	Client();
	void Connect(const std::string& ip, int port);
	void Message(const std::string& message);
	void ReceiveMessages();
	void Disconnect();

private:

	void Log(const std::string& log);

	ENetHost* mClient;
	ENetPeer* mPeer;

};