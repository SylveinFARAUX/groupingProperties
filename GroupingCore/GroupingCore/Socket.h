#pragma once

//Socket libraries
#include <winsock2.h>
#include <ws2tcpip.h>

//I/O libraries
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

//Other libraries
#include "framework.h"
#include <string>
#include <vector>
#include <stdexcept>

//link with ws2_32.lib
#pragma comment(lib, "ws2_32.lib")


class GROUPINGCORE_API Socket
{
private:
	Socket() {}
	struct timeval timeout;
public:
	SOCKADDR_IN  sin, sin_client;
	std::vector<SOCKET*> m_sock;
	int port;
	std::string name;
	char const *address;

	Socket(const char * name, const char * address, int port, int timeoutSec = 5);
	~Socket();

	SOCKET* getSock(int number);
	void close();
	void setTimeout(int timeoutSec);
	void Send(const char* msg, bool show = false);
	void Send(int socketNumber, const char* msg, bool show = false);
	const char* receive();
	const char* receive(int socketNumber);
	bool connection();
	int accept();
};
