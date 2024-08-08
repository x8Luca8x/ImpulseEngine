#pragma once

#include "SocketsModule.h"

enum class ESocketType : uint8
{
	Invalid,
	Stream,
	Datagram,
};

enum class ESocketFamily : uint8
{
	Invalid,
	IPv4,
	IPv6,
};