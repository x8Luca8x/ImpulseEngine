#pragma once

#include "SocketsModule.h"

enum ESocketType : uint8
{
	SOCK_Unknown,
	SOCK_Stream,
	SOCK_Datagram,
};

namespace NetworkProtocolTypes
{
	extern SOCKETS_API const FName IPv4;
	extern SOCKETS_API const FName IPv6;
}