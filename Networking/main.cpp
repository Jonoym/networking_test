#include <enet/enet.h>
#include <iostream>
#include <thread>
#include <atomic>

#include "Client.h"

void ReceiveMessages(std::atomic<bool>& flag, Client& client)
{

	while (!flag)
	{
		client.ReceiveMessages();
	}

}

int main(int argc, char** argv)
{
	Client client;

	client.Connect("127.0.0.1", 7777);

	std::atomic<bool> flag(false);

	std::thread messageThread(ReceiveMessages, std::ref(flag), std::ref(client));

	std::string input;

	do
	{
		std::cout << "[LOCAL] ";

		std::getline(std::cin, input);

		client.Message(input);

	} while (input != "exit");

	flag = true;

	messageThread.join();

	client.Disconnect();
}