#include "CommonTools.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include "Socket.h"


#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

struct tm* CommonTools::getTime(){
	//get timestamp
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	//info = gmtime(&rawtime );					
	info = localtime ( &rawtime );
	return info;
}

std::string const CommonTools::getStrTime()
{
	struct tm * m_time = getTime();
	int h = m_time->tm_hour,
		m = m_time->tm_min, 
		s = m_time->tm_sec;

	std::string str_h = (h < 10) ? "0" + std::to_string(h) : std::to_string(h);
	std::string str_m = (m < 10) ? "0" + std::to_string(m) : std::to_string(m);
	std::string str_s = (s < 10) ? "0" + std::to_string(s) : std::to_string(s);
			
	m_time = NULL;
	return  str_h + ":" + str_m + ":" + str_s;
}

std::string const CommonTools::getFullTimeStr()
{
	struct tm * m_time = getTime();
	int day = m_time->tm_mday,
		month = m_time->tm_mon,
		year = 1900 + m_time->tm_year,
		h = m_time->tm_hour,
		m = m_time->tm_min, 
		s = m_time->tm_sec;

	std::string str_month = (month < 10) ? "0" + std::to_string(month) : std::to_string(month);
	std::string str_day = (day < 10) ? "0" + std::to_string(day) : std::to_string(day);
	std::string str_h = (h < 10) ? "0" + std::to_string(h) : std::to_string(h);
	std::string str_m = (m < 10) ? "0" + std::to_string(m) : std::to_string(m);
	std::string str_s = (s < 10) ? "0" + std::to_string(s) : std::to_string(s);
			
	m_time = NULL;
	return  str_day + "/" + str_month + "/" + std::to_string(year) + "-" + str_h + ":" + str_m + ":" + str_s;
}

std::string const CommonTools::getStrTimeFromStruct(tm * m_time)
{
	int h = m_time->tm_hour,
			m = m_time->tm_min, 
			s = m_time->tm_sec;

		std::string str_h = (h < 10) ? "0" + std::to_string(h) : std::to_string(h);
		std::string str_m = (m < 10) ? "0" + std::to_string(m) : std::to_string(m);
		std::string str_s = (s < 10) ? "0" + std::to_string(s) : std::to_string(s);
			
		return  str_h + ":" + str_m + ":" + str_s;
}

int const CommonTools::getFullDayNumber()
{
	struct tm * m_time = getTime();
	int num = m_time->tm_yday;	
		
	m_time = NULL;
	return  num;
}

bool CommonTools::isExistingName(std::string target, std::vector<std::string> base)
{
	if(std::find(base.begin(), base.end(), target) != base.end())
		return true;
	else
		return false;
}

void CommonTools::showAddr()
{
	IP_ADAPTER_ADDRESSES* adapter_addresses(NULL);
	IP_ADAPTER_ADDRESSES* adapter(NULL);

	// Start with a 16 KB buffer and resize if needed -
	// multiple attempts in case interfaces change while
	// we are in the middle of querying them.
	DWORD adapter_addresses_buffer_size = 16 * 8;
	for (int attempts = 0; attempts != 3; ++attempts)
	{
		adapter_addresses = (IP_ADAPTER_ADDRESSES*)malloc(adapter_addresses_buffer_size);

		DWORD error = ::GetAdaptersAddresses(
			AF_UNSPEC,
			GAA_FLAG_SKIP_ANYCAST |
			GAA_FLAG_SKIP_MULTICAST |
			GAA_FLAG_SKIP_DNS_SERVER |
			GAA_FLAG_SKIP_FRIENDLY_NAME,
			NULL,
			adapter_addresses,
			&adapter_addresses_buffer_size);

		if (ERROR_SUCCESS == error)
		{
			// We're done here, people!
			break;
		}
		else if (ERROR_BUFFER_OVERFLOW == error)
		{
			// Try again with the new size
			free(adapter_addresses);
			adapter_addresses = NULL;

			continue;
		}
		else
		{
			// Unexpected error code - log and throw
			free(adapter_addresses);
			adapter_addresses = NULL;
		}
	}

	// Iterate through all of the adapters
	for (adapter = adapter_addresses; NULL != adapter; adapter = adapter->Next)
	{
		// Skip loopback adapters
		if (IF_TYPE_SOFTWARE_LOOPBACK == adapter->IfType)
		{
			continue;
		}

		// Parse all IPv4 and IPv6 addresses
		for (
			IP_ADAPTER_UNICAST_ADDRESS* address = adapter->FirstUnicastAddress;
			NULL != address;
			address = address->Next)
		{
			auto family = address->Address.lpSockaddr->sa_family;
			if (AF_INET == family)
			{
				// IPv4
				SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(address->Address.lpSockaddr);

				char str_buffer[INET_ADDRSTRLEN] = { 0 };
				inet_ntop(AF_INET, &(ipv4->sin_addr), str_buffer, INET_ADDRSTRLEN);
				std::cout << "IPV4 address : " << str_buffer << std::endl;
			}
			else if (AF_INET6 == family)
			{
				// IPv6
				SOCKADDR_IN6* ipv6 = reinterpret_cast<SOCKADDR_IN6*>(address->Address.lpSockaddr);

				char str_buffer[INET6_ADDRSTRLEN] = { 0 };
				inet_ntop(AF_INET6, &(ipv6->sin6_addr), str_buffer, INET6_ADDRSTRLEN);

				std::string ipv6_str(str_buffer);

				// Detect and skip non-external addresses
				bool is_link_local(false);
				bool is_special_use(false);

				if (0 == ipv6_str.find("fe"))
				{
					char c = ipv6_str[2];
					if (c == '8' || c == '9' || c == 'a' || c == 'b')
					{
						is_link_local = true;
					}
				}
				else if (0 == ipv6_str.find("2001:0:"))
				{
					is_special_use = true;
				}

				if (!(is_link_local || is_special_use))
					std::cout << "IPV6 adress : " << ipv6_str << std::endl;
			}
			else
			{
				// Skip all other types of addresses
				continue;
			}
		}
	}

	// Cleanup
	free(adapter_addresses);
	adapter_addresses = NULL;

	// Cheers!
}