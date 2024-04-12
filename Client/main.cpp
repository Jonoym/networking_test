#include "Server.h"

int main(int argc, char** argv)
{

	Server server;

	server.Host(7777);

	server.Shutdown();

	return 0;
}