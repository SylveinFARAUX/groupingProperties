#include "Socket.h"


Socket::Socket(const char * name, const char* address, int port, int timeoutSec) :name(name), port(port)
{
	WSADATA WSAData = {0};
	int status = WSAStartup(MAKEWORD(2,0), &WSAData);
	
	if (status != 0)
	{
		std::cout << "[ERROR]: " << status << " Unable to start Winsock." << std::endl;
		return;
	}

	this->adresse= address;
	char port_str[6] = {0};
	_itoa(port, port_str, 10);


	struct addrinfo hints = {0};
	struct addrinfo *res = NULL;

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	status = getaddrinfo(NULL, port_str, &hints, &res);
	if (status != 0)
	{
		std::cout << "[ERROR]: " << status << " Unable to get address info for Port " << port << "." << std::endl;
		return;
	}

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		wprintf(L"socket function failed with error: %u\n", WSAGetLastError());
		return;
	}

	if (strcmp(address, "INADDR_ANY")==0) // Si on d�sire cr�er un serveur
		sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		sin.sin_addr.s_addr	= inet_addr(this->adresse);

	sin.sin_family		= AF_INET;
	sin.sin_port		= htons(this->port);
	
	if (strcmp(address, "INADDR_ANY")==0)
	{
		if (_WINSOCK2API_::bind(sock, (SOCKADDR *)&sin, sizeof(sin)) == SOCKET_ERROR) 
		
		std::cout << "--> [" << name << "] Server socket for safe connection created on " << sin.sin_addr.s_addr << ":" << port << "." << std::endl;
	}

	timeout.tv_sec = timeoutSec;
	timeout.tv_usec = 0;

	std::cout << this->name << " : Socket created successfully" << std::endl;
}

Socket::~Socket(){
	close();
} 

void Socket::close()
{
	closesocket(sock);
	closesocket(sock_client);
		
	WSACleanup();
}

void Socket::setTimeout(int timeoutSec){
	timeout.tv_sec = timeoutSec;
}

bool Socket::connection()
{
	std::cout << name << " : try connection with " << inet_ntoa(sin.sin_addr) << ":"<<htons(sin.sin_port)<<" ..."<< std::endl;
	if (_WINSOCK2API_::connect(sock, (SOCKADDR *)&sin, sizeof(sin)) == 0)
		std::cout << name << " : connected" << std::endl;
	else
	{
		std::cout << name << " : /!\\ unable to connect"<< std::endl;
		return false;
	}
	return true;
}

const char* Socket::receive()
{
	char buffer[1024] = {0}; // j'ai grug� pour modifier la taille du buffer en fonction des besoins
	int max_size = sizeof(buffer)-1;
	std::string result = "";
	bool full_message = false;
	int n = 0;

	fd_set set;
	FD_ZERO(&set); /* clear the set */
	FD_SET(sock, &set); /* add our file descriptor to the set */

	n = recv(sock, buffer, max_size, 0);
	buffer[n] = '\0';

	result += buffer;

	if (n < max_size)
		full_message = true;

	while ((n >= max_size) && !full_message) // Si le message est plus long que 1023 caract�res
	{
		int rv = select(sock+1, &set, NULL, NULL, &timeout);

		if (rv == SOCKET_ERROR) {
			std::cout <<this->name << " /!\\ Error when reading the socket"<<std::endl;
			full_message = true;
		}
		else if (rv == 0)//timeout, pas d'activit� sur la socket
			full_message = true;
		else //activit� il y a eu, on peut lire...
		{
			n = recv(sock, buffer, max_size, 0);
			buffer[n] = '\0';

			result += buffer;

			if (n < max_size)
				full_message = true;
		}
	}

	char* r = new char[result.length() + 1];
	strcpy(r, result.c_str());
	return r;
}

const char* Socket::receive(SOCKET reception_sock)
{
	char buffer[1024] = {0}; // j'ai grug� pour modifier la taille du buffer en fonction des besoins
	int max_size = sizeof(buffer)-1;
	std::string result = "";
	bool full_message = false;
	int n = 0;

	fd_set set;
	FD_ZERO(&set); /* clear the set */
	FD_SET(reception_sock, &set); /* add our file descriptor to the set */

	n = recv(reception_sock, buffer, max_size, 0);
	buffer[n] = '\0';
	
	result += buffer;

	if (n < max_size)
		full_message = true;

	while ((n >= max_size) && !full_message) // Si le message est plus long que 1023 caract�res
	{
		int rv = select(reception_sock+1, &set, NULL, NULL, &timeout);

		if (rv == SOCKET_ERROR) {
			std::cout <<this->name << " /!\\ Error when reading the socket"<<std::endl;
			full_message = true;
		}
		else if (rv == 0)//timeout, pas d'activit� sur la socket
			full_message = true;
		else //activit� il y a eu, on peut lire...
		{
			n = recv(reception_sock, buffer, max_size, 0);
			buffer[n] = '\0';

			result += buffer;

			if (n < max_size)
				full_message = true;
		}
	}
	
	char *r = new char[result.length()+1];
	strcpy(r, result.c_str());
	return r;
}

void Socket::Send(const char* msg, bool show)
{
	if (show)
		std::cout << name << " : Sending to " << inet_ntoa(sin.sin_addr) << ":" << htons(sin.sin_port) << " the msg ("<<strlen(msg)<<"): " << std::endl<< msg << std::endl;
	
	int numberOfBytesWritten = send(sock, msg, strlen(msg), 0);

	if(numberOfBytesWritten == SOCKET_ERROR)
		std::cout << "--> Client is disconnected." << std::endl;
	
	else if(numberOfBytesWritten != (int)strlen(msg))
		std::cout << "--> Message not sent completely "<< std::endl;		
}

void Socket::Send(SOCKET sock, const char* msg, bool show)
{
	if (show)
		std::cout << name << " : Sending to " << inet_ntoa(sin_client.sin_addr) << ":" << htons(sin.sin_port) << " the msg ("<<strlen(msg)<<"): " << msg << std::endl; //A faire : gestion des SOCKET et SINADDR_IN en tab
	send(sock, msg, strlen(msg), 0);
}

bool Socket::accept()
{
	struct sockaddr_in clientAddr = { 0 };
	int opt = sizeof(clientAddr);

	std::cout << "--> [" << name << "] Entering listening mode." << std::endl;
		
	if (_WINSOCK2API_::listen(sock, 1) == SOCKET_ERROR)
	{
		printf( "Listen failed with error: %ld\n", WSAGetLastError() );
		return false;
	}

	std::cout << "--> [" << name << "] Waiting for an incoming safe connection." << std::endl;
	
	sock_client = _WINSOCK2API_::accept(sock, (struct sockaddr *)&clientAddr, &opt);
	sin_client = clientAddr;

	u_long arg = 1;

	int result = _WINSOCK2API_::setsockopt(sock_client,
							IPPROTO_TCP,
							TCP_NODELAY,
							(char*)&arg,
							sizeof(int));

	if(result < 0)
	{
		std::cout << "/!\\ [" << name << "] Error while modifying the delay of the socket." << std::endl;
		return false;
	}

	std::cout << "--> [" << name << "] Client connection accepted from " << inet_ntoa(sin_client.sin_addr) << std::endl;
	return true;
}

