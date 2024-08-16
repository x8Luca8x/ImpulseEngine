#pragma once

#include "CoreMinimal.h"
#include "SocketTypes.h"

#if PLATFORM_WINDOWS

#include <winsock2.h>
#include <ws2tcpip.h>

typedef int32 SOCKLEN;

#define PLATFORM_HAS_BSD_SOCKETS 1
#define PLATFORM_HAS_BSD_IPV6_SOCKETS 1

#pragma comment(lib, "Ws2_32.lib")

#endif

#if PLATFORM_HAS_BSD_SOCKETS

inline int32 GetProtocolFamily(const FName InFamily)
{
	return InFamily == NetworkProtocolTypes::IPv4 ? AF_INET : (InFamily == NetworkProtocolTypes::IPv6 ? AF_INET6 : AF_UNSPEC);
}

inline int32 GetSocketType(ESocketType InSocketType)
{
	return InSocketType == SOCK_Unknown ? 0 : (InSocketType == SOCK_Stream ? SOCK_STREAM : SOCK_DGRAM);
}

inline int32 GetProtocolType(ESocketType InSocketType)
{
	return InSocketType == SOCK_Unknown ? -1 : (InSocketType == SOCK_Stream ? IPPROTO_TCP : IPPROTO_UDP);
}

#endif