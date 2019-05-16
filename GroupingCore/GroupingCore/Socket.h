#pragma once
/*
#ifndef WIN32
#define WIN32
#endif
*/
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
	SOCKET sock, sock_client;
	SOCKADDR_IN  sin, sin_client;
	int port;
	std::string name;
	char const *adresse;

	Socket(const char * name, const char * address, int port, int timeoutSec = 5);
	~Socket();

	void close();
	void setTimeout(int timeoutSec);
	void Send(const char* msg, bool show = false);
	void Send(SOCKET sock, const char* msg, bool show = false);
	const char* receive();
	const char* receive(SOCKET reception_sock);
	bool connection();
	bool accept();
};
